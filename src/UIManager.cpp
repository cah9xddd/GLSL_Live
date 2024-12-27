#include "UIManager.h"
#include "Utils.h"

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

    vertex_shader_source = ReadTextFromFile("shaders/latest_vertex.glsl");
    if (vertex_shader_source.empty())
    {
        vertex_shader_source = ReadTextFromFile("shaders/default/default_vertex.glsl");
    }

    fragment_shader_source = ReadTextFromFile("shaders/latest_fragment.glsl");
    if (fragment_shader_source.empty())
    {
        fragment_shader_source = ReadTextFromFile("shaders/default/default_fragment.glsl");
    }

    fragment_shader_source.resize(1024 * 100);
}

UIManager::~UIManager() noexcept { Shutdown(); }

void UIManager::PrepareFrame() const noexcept
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIManager::RenderFrame() noexcept
{
    if (!is_ui_visible) { return; }

    PrepareFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));

    int width, height;
    glfwGetWindowSize(window, &width, &height);  // Get window size
    ImGui::SetNextWindowSize(ImVec2((float)width, (float)height));

    ImGui::SetNextWindowBgAlpha(0.25f);

    ImGui::PushStyleColor(ImGuiCol_Text, text_color);

    ImGui::Begin("Editor",
                 nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                     | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar
                     | ImGuiWindowFlags_NoTitleBar);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save"))
            {
                show_save_popup = true;
                WriteTextToFile("shaders/latest_fragment.glsl", fragment_shader_source);
                WriteTextToFile("shaders/latest_vertex.glsl", vertex_shader_source);
            }

            if (ImGui::MenuItem("Quit", "Ctrl+Q"))
            {
                WriteTextToFile("shaders/latest_fragment.glsl", fragment_shader_source);
                WriteTextToFile("shaders/latest_vertex.glsl", vertex_shader_source);
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Hide/Show UI", "Ctrl+H")) { is_ui_visible = !is_ui_visible; }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }


    if (ImGui::BeginTabBar("TabBar"))
    {
        if (ImGui::BeginTabItem("Vertex Shader"))
        {
            ImGui::Text(vertex_shader_source.c_str());


            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Fragment Shader"))
        {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.3f, 0.0f));

            ImVec2 parent_window_size = ImGui::GetWindowSize();

            ImGui::InputTextMultiline(" ",
                                      fragment_shader_source.data(),
                                      fragment_shader_source.size(),
                                      ImVec2(-FLT_MIN, parent_window_size.y * 0.90f),
                                      ImGuiInputTextFlags_AllowTabInput);

            ImGui::PopStyleColor();

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Settings"))
        {
            if (ImGui::ColorEdit3("Text Color", (float*)&text_color)) {}

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    if (show_save_popup) { DrawSavePopup(); }


    // Pop the style change to revert to the previous style settings
    ImGui::PopStyleColor();

    ImGui::End();

    // Rendering demo window
    // static bool show_demo_window = true;
    // if (show_demo_window) { ImGui::ShowDemoWindow(&show_demo_window); }

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

void UIManager::DrawSavePopup() noexcept
{
    ImGui::OpenPopup("SavePopup");
    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("SavePopup",
                               NULL,
                               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        ImGui::Text("Enter the file name");
        ImGui::Separator();

        static char        buf[256]      = "";
        static bool        show_error    = false;
        static std::string error_message = "";

        if (ImGui::InputText("##save", buf, IM_ARRAYSIZE(buf)))
        {
            show_error = false;  // Reset error state when user modifies input
        }

        if (ImGui::Button("OK", ImVec2(200, 0)))
        {
            if (strlen(buf) == 0)
            {
                show_error    = true;
                error_message = "Filename cannot be empty";
            }
            else
            {
                std::string out_path = GetApplicationPath() + "/shaders/" + std::string(buf) + "/"
                                     + std::string(buf) + "_fragment.glsl";
                LOG_INFO("Saving to: {}", out_path);

                auto res = WriteTextToFile(out_path, fragment_shader_source);
                if (res)
                {
                    memset(buf, 0, sizeof(buf));  // Cleaning buffer

                    show_error = false;

                    ImGui::CloseCurrentPopup();
                    show_save_popup = false;
                }
                else
                {
                    show_error    = true;
                    error_message = "Failed to save file";
                }
            }
        }

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(200, 0)))
        {
            ImGui::CloseCurrentPopup();
            show_save_popup = false;
        }

        if (show_error)  // Display persistent error message if needed
        {
            ImGui::Separator();
            ImGui::TextColored(ImVec4(1, 0, 0, 1), error_message.c_str());
        }

        ImGui::EndPopup();
    }
}
