#pragma once

#include "PCH.h"

/**
 * @brief UIManager class responsible for setting up and managing ImGui.
 */
struct UIManager {
    explicit UIManager(GLFWwindow* window_) noexcept;
    ~UIManager() noexcept;

    /**
     * @brief Starts a new ImGui frame.
     */
    void PrepareFrame() const noexcept;

    /**
     * @brief Renders the ImGui frame.
     */
    void RenderFrame() noexcept;

    /**
     * @brief Gets the fragment shader source code.
     * @return The fragment shader source code.
     */
    [[nodiscard]] std::string_view GetFragmentShaderSource() const noexcept;

    /**
     * @brief Gets the vertex shader source code.
     * @return The vertex shader source code.
     */
    [[nodiscard]] std::string_view GetVertexShaderSource() const noexcept;


    static void SetUIVisibility(bool visibility) noexcept;

    static bool GetUIVisibility() noexcept;

private:
    /**
     * @brief Cleans up ImGui resources.
     */
    void Shutdown() noexcept;

    void DrawSavePopup() noexcept;

private:
    GLFWwindow* window;

    std::string fragment_shader_source;
    std::string vertex_shader_source;

    static bool is_ui_visible;


    // Helpers
    ImVec4 text_color      = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    bool   show_save_popup = false;
};
