#include "UIManager.h"

bool UIManager::is_ui_visible = true;

UIManager::UIManager(GLFWwindow* window_) noexcept : window(window_)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsClassic();

    ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
    ImGui_ImplOpenGL3_Init("#version 460");


    // Load our font
    io.Fonts->AddFontFromFileTTF("assets/fonts/MartianMonoNerdFontMono-Regular.ttf", 20.0f);

    // Setup style
    auto& style              = ImGui::GetStyle();
    style.WindowBorderSize   = 0.0f;
    style.WindowPadding.x    = 10.f;
    style.WindowPadding.y    = 10.f;
    style.ItemSpacing.x      = 10.f;
    style.ItemInnerSpacing.x = 5.f;
    style.IndentSpacing      = 30.f;
    style.ScrollbarSize      = 15.f;

    vertex_shader_source = R"(#version 460 core

layout (location = 0) in vec3 Pos;  // Input vertex position

uniform vec2 in_resolution;   // Viewport resolution (in pixels)

out vec2 fragCoord;
out vec2 iResolution;

void main()
{
    gl_Position = vec4(Pos, 1.0);

    fragCoord.x = in_resolution.x * (gl_Position.x + 1.0) / 2.0;
    fragCoord.y = in_resolution.y * (gl_Position.y + 1.0) / 2.0;

    iResolution = in_resolution;

})";

    fragment_shader_source = R"(#version 460 core

in vec2 fragCoord;
in vec2 iResolution;
uniform float iTime;        // Elapsed time in seconds
uniform float iTimeDelta;  // Frame time delta
uniform int iFrame;         // Current frame number
uniform vec4 iMouse;        // Mouse input (position and click)

out vec4 fragColor;

void main()
{
	fragColor = vec4( sin(fragCoord.x) , 0.f,0.f,0.f);
})";

    fragment_shader_source.resize(1024 * 100);
}

UIManager::~UIManager() noexcept { Shutdown(); }

void UIManager::PrepareFrame() const noexcept
{
    if (!is_ui_visible) { return; }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIManager::RenderFrame() noexcept
{
    if (!is_ui_visible) { return; }

    ImGui::SetNextWindowPos(ImVec2(0, 0));

    int width, height;
    glfwGetWindowSize(window, &width, &height);  // Get window size
    ImGui::SetNextWindowSize(ImVec2((float)width, (float)height));

    ImGui::SetNextWindowBgAlpha(0.5f);

    ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    // Tab 1
    if (ImGui::BeginTabBar("TabBar"))
    {
        if (ImGui::BeginTabItem("Vertex Shader"))
        {
            ImGui::Text(vertex_shader_source.c_str());
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Fragment Shader"))
        {
            // we wanted childs to be transparent
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.3f, 0.75f));

            // get parent window size
            ImVec2 parent_window_size = ImGui::GetWindowSize();

            // here we wanna create multitext input  for fragment shader
            ImGui::InputTextMultiline("Fragment Shader",
                                      fragment_shader_source.data(),
                                      fragment_shader_source.size(),
                                      ImVec2(-FLT_MIN, parent_window_size.y),
                                      ImGuiInputTextFlags_AllowTabInput);

            ImGui::PopStyleColor();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Output"))
        {
            ImGui::Text("Hello from Output");

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();


    static bool show_demo_window = true;
    if (show_demo_window) { ImGui::ShowDemoWindow(&show_demo_window); }

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

std::string_view UIManager::GetFragmentShaderSource() const noexcept
{
    return fragment_shader_source;
}

std::string_view UIManager::GetVertexShaderSource() const noexcept { return vertex_shader_source; }

void UIManager::SetUIVisibility(bool visibility) noexcept { is_ui_visible = visibility; }

bool UIManager::GetUIVisibility() noexcept { return is_ui_visible; }

void UIManager::Shutdown() noexcept
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
