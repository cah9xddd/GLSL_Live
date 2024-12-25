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

    vertex_shader_source   = ReadTextFromFile("shaders/default/default_vertex.glsl");
    fragment_shader_source = ReadTextFromFile("shaders/default/default_fragment.glsl");

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

    ImGui::SetNextWindowBgAlpha(0.25f);

    ImGui::PushStyleColor(ImGuiCol_Text, text_color);

    ImGui::Begin("Editor",
                 nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                     | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoTitleBar);

    // Tab 1
    if (ImGui::BeginTabBar("TabBar"))
    {
        if (ImGui::BeginTabItem("Hotkeys"))
        {
            ImGui::Text("Ctrl + Q - Quit");
            ImGui::Text("Ctrl + H - Hide/Show UI");
            ImGui::EndTabItem();
        }

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

    // Pop the style change to revert to the previous style settings
    ImGui::PopStyleColor();

    ImGui::End();

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
