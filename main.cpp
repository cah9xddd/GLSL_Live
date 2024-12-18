#include "PCH.h"

#include "UIManager.h"
#include "ShaderProgram.h"

void SetupAsyncLogger()
{
    spdlog::init_thread_pool(8192, 1);  // max queue size is 8192 and 1 worker thread

    // Create console sink
    auto console_logger = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_logger->set_pattern("[%Y.%m.%d %H:%M:%S.%e][%^%s:%#:%v%$]");

    // Create file sink
    auto file_logger = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        "logs/latest.log",
        true);  // `true` for truncating the file
    file_logger->set_pattern("[%Y.%m.%d %H:%M:%S.%e][%^%l%$][%s:%#:%v]");

    // Combine sinks into one logger
    std::vector<spdlog::sink_ptr> sinks { console_logger, file_logger };
    auto                          async_logger =
        std::make_shared<spdlog::async_logger>("async_logger",
                                               sinks.begin(),
                                               sinks.end(),
                                               spdlog::thread_pool(),
                                               spdlog::async_overflow_policy::block);

    spdlog::register_logger(async_logger);

    // Set the async logger as the default
    spdlog::set_default_logger(async_logger);

    // Set the logging level based on the build type
#ifdef RELEASE
    spdlog::set_level(spdlog::level::info);
#elif DEBUG
    spdlog::set_level(spdlog::level::trace);
#endif

    // The pattern for the logger is set to include the source file and line number
    // Example:
    // [2024.10.04 18:42:55.699][Game.cpp:68:trace message]
    // [2024.10.04 18:42:55.699][Game.cpp:69:debug message]
    // [2024.10.04 18:42:55.699][Game.cpp:70:info message]
    // [2024.10.04 18:42:55.699][Game.cpp:71:warn message]
    // [2024.10.04 18:42:55.699][Game.cpp:72:error message]
    // [2024.10.04 18:42:55.699][Game.cpp:73:critical message]
}

// Window dimensions
int32_t SCREEN_WIDTH  = 1600;
int32_t SCREEN_HEIGHT = 1200;

float cursor_x = 0.0f;
float cursor_y = 0.0f;

bool is_scene_playing = true;

bool CheckLinkingErrors(unsigned int id)
{
    // Check for linking errors
    GLint success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        char info_log[512];
        glGetProgramInfoLog(id, 512, nullptr, info_log);
        LOG_WARN("Shader program {} linking error: {}", id, info_log);
        return false;
    }
    return true;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void MouseCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    cursor_x = xpos;
    cursor_y = ypos;
}

void HandleInput(GLFWwindow* window)
{

    static bool space_key_pressed = false;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS
        && glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS
             && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (!space_key_pressed)  // Check if SPACE was not already pressed
        {
            is_scene_playing  = !is_scene_playing;  // Toggle the flag
            space_key_pressed = true;               // Mark as pressed
        }
    }
    else
    {
        space_key_pressed = false;  // Reset the key state when key is released
    }
}

int main()
{
    // Setup the async logger
    SetupAsyncLogger();

    glfwSetErrorCallback(
        [](int error, const char* description)
        {
            LOG_CRITICAL("GLFW Error {}: {}", error, description);
        });

    // Initialize GLFW
    if (!glfwInit())
    {
        LOG_CRITICAL("Failed to initialize GLFW");
        return -1;
    }
    {

        // Set GLFW options
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef RELEASE
        // Gets the screen resolution of the primary monitor
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        SCREEN_HEIGHT           = mode->height;
        SCREEN_WIDTH            = mode->width;
        // Create a fullscreen window with the same resolution as the primary monitor
        GLFWwindow* window = glfwCreateWindow(mode->width,
                                              mode->height,
                                              "GLSL Live",
                                              glfwGetPrimaryMonitor(),
                                              nullptr);
#else
        GLFWwindow* window =
            glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GLSL Live", nullptr, nullptr);

#endif

        if (!window)
        {
            LOG_CRITICAL("Failed to create GLFW window");
            glfwTerminate();
            return -1;
        }

        // Make the window's context current
        glfwMakeContextCurrent(window);

        // Load OpenGL using GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_CRITICAL("Failed to initialize GLAD");
            return -1;
        }

        // Set the viewport
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        // Register the framebuffer size callback
        glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
        glfwSetCursorPosCallback(window, MouseCursorCallback);

        UIManager ui_manager(window);

        // Define rectangle vertices
        float vertices[] = {
            // Positions
            1.f,  1.f,  0.0f,  // Top right
            1.f,  -1.f, 0.0f,  // Bottom right
            -1.f, -1.f, 0.0f,  // Bottom left
            -1.f, 1.f,  0.0f   // Top left
        };
        unsigned int indices[] = {
            0, 1, 3,  // First triangle
            1, 2, 3   // Second triangle
        };

        // Generate buffers and arrays
        unsigned int VBO, VAO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Bind the Vertex Array Object
        glBindVertexArray(VAO);

        // Bind and set vertex buffer(s)
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Configure vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Unbind the VAO
        glBindVertexArray(0);

        Shader vertex_shader;
        vertex_shader.CompileFromText(ui_manager.GetVertexShaderSource(), ShaderType::VERTEX);

        // Initialize previous time for delta time calculation
        std::vector<float> frame_times;
        float              last_frame       = 0.0f;
        constexpr float    FPS_LOG_INTERVAL = 1.0f;  // Log every 1 second
        float              elapsed_time     = 0.0f;
        float              current_time     = 0.0f;
        float              delta_time       = 0.0f;

        // turn off vsync for now
        glfwSwapInterval(1);

        int32_t frame = 0;
        // Render loop
        while (!glfwWindowShouldClose(window))
        {
            // Get current time
            float current_time = glfwGetTime();

            // Calculate delta time (time elapsed since last frame)
            delta_time = current_time - last_frame;

            // Update previous_time for next frame
            last_frame = current_time;

            // LOG_INFO("FPS: {}", 1.0 / delta_time);


            ui_manager.PrepareFrame();

            HandleInput(window);


            // Rendering commands
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Main logic
            if (is_scene_playing)
            {
                Shader fragment_shader;
                auto   fragment_shader_source = ui_manager.GetFragmentShaderSource();
                bool   is_fragment_shader_good =
                    fragment_shader.CompileFromText(fragment_shader_source, ShaderType::FRAGMENT);


                if (is_fragment_shader_good)
                {
                    ShaderProgram shader_program(vertex_shader, fragment_shader);
                    shader_program.Use();
                    shader_program.SetUniform("in_resolution",
                                              glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
                    shader_program.SetUniform("iTime", (float)current_time);
                    shader_program.SetUniform("iTimeDelta", (float)delta_time);
                    shader_program.SetUniform("iFrame", frame);

                    float normalized_x = static_cast<float>(cursor_x) / SCREEN_WIDTH;
                    float normalized_y = static_cast<float>(cursor_y) / SCREEN_HEIGHT;
                    shader_program.SetUniform("iMouse",
                                              glm::vec4(normalized_x, normalized_y, 0.0f, 0.0f));


                    glBindVertexArray(VAO);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }
            }


            ui_manager.RenderFrame();

            // Swap buffers and poll for events
            glfwSwapBuffers(window);
            glfwPollEvents();

            ++frame;

            // Log the frame time
            frame_times.push_back(delta_time);
            elapsed_time += delta_time;
            if (elapsed_time >= FPS_LOG_INTERVAL)
            {


                // Calculate average FPS
                float avg_fps = 1.0f
                              / (std::accumulate(frame_times.begin(), frame_times.end(), 0.0f)
                                 / frame_times.size());

                // Calculate 1% low FPS
                std::vector<float> sorted_frame_times = frame_times;
                std::sort(sorted_frame_times.begin(), sorted_frame_times.end());
                float one_percent_low_fps =
                    1.0f
                    / sorted_frame_times[static_cast<size_t>(sorted_frame_times.size() * 0.99f)];

                // Calculate 0.1% low FPS
                float point_one_percent_low_fps =
                    1.0f
                    / sorted_frame_times[static_cast<size_t>(sorted_frame_times.size() * 0.999f)];

                // Log the FPS metrics
                LOG_INFO("Average FPS: {}", avg_fps);
                LOG_INFO("1% Low FPS: {}", one_percent_low_fps);
                LOG_INFO("0.1% Low FPS: {}", point_one_percent_low_fps);

                // Reset the elapsed time and frame times
                elapsed_time = 0.0f;
                frame_times.clear();


                glfwSetWindowTitle(window, fmt::format("GLSL Live | FPS: {:.2f}", avg_fps).c_str());
            }
        }

        vertex_shader.DeleteShader();

        // Clean up
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}