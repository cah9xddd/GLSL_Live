#include "UIManager.h"
#include "Utils.h"

bool UIManager::is_ui_visible = true;

UIManager::UIManager(GLFWwindow* window_, ShaderManager& shader_manager_) noexcept :
    window(window_),
    shader_manager(shader_manager_)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsClassic();

    ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
    ImGui_ImplOpenGL3_Init("#version 460");


    // Load our font
    io.Fonts->AddFontFromFileTTF("assets/fonts/MartianMonoNerdFontMono-Regular.ttf", 22.0f);

    // Setup style
    auto& style              = ImGui::GetStyle();
    style.WindowBorderSize   = 0.0f;
    style.WindowPadding.x    = 10.f;
    style.WindowPadding.y    = 10.f;
    style.ItemSpacing.x      = 10.f;
    style.ItemInnerSpacing.x = 5.f;
    style.IndentSpacing      = 30.f;
    style.ScrollbarSize      = 15.f;
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

    auto vertex_shader_source = shader_manager.GetVertexShader().GetCodeConst();

    auto&        fragment_shader        = shader_manager.GetFragmentShader();
    std::string& fragment_shader_source = fragment_shader.GetCode();


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
            if (ImGui::MenuItem("Save", "Ctrl+S")) { show_save_popup = true; }

            if (ImGui::MenuItem("Quit", "Ctrl+Q"))
            {
                shader_manager.SaveFragmentShaderToPath("shaders/latest_fragment.glsl");
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
        if (ImGui::BeginTabItem("Fragment Shader"))
        {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.3f, 0.0f));

            ImVec2 parent_window_size = ImGui::GetWindowSize();

            ImGui::InputTextMultiline(" ",
                                      &fragment_shader_source,
                                      ImVec2(-FLT_MIN, parent_window_size.y * 0.90f),
                                      ImGuiInputTextFlags_AllowTabInput);

            ImGui::PopStyleColor();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Vertex Shader"))
        {
            ImGui::Text(vertex_shader_source.data());


            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Compilation Errors"))
        {

            if (!fragment_shader.IsGood())
            {
                ImGui::TextColored(ImVec4(1, 0, 0, 1),
                                   fragment_shader.GetCompilationError().data());
            }

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


    ImGui::PopStyleColor();  // Pop the style change to revert text colorq

    ImGui::End();

    // Rendering demo window
    // static bool show_demo_window = true;
    // if (show_demo_window) { ImGui::ShowDemoWindow(&show_demo_window); }

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::SetUIVisibility(bool visibility) noexcept { is_ui_visible = visibility; }

bool UIManager::GetUIVisibility() noexcept { return is_ui_visible; }

void UIManager::ShowSavePopup() noexcept { show_save_popup = true; }

void UIManager::Shutdown() noexcept
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::DrawSavePopup() noexcept
{
    auto fragment_shader_source = shader_manager.GetFragmentShader().GetCode();

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

        static std::string buf;
        static bool        show_error    = false;
        static std::string error_message = "";

        if (ImGui::InputText("##save", &buf))
        {
            show_error = false;  // Reset error state when user modifies input
        }

        if (ImGui::Button("OK", ImVec2(200, 0)))
        {
            if (buf.empty())
            {
                show_error    = true;
                error_message = "Filename cannot be empty";
            }
            else
            {
                std::string out_path =
                    "/shaders/" + std::string(buf) + "/" + std::string(buf) + "_fragment.glsl";
                LOG_INFO("Saving to: {}", out_path);


                auto res = shader_manager.SaveFragmentShaderToPath(out_path);
                if (res)
                {
                    buf.clear();

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
