#pragma once

#include "PCH.h"

#include "ShaderManager.h"

/**
 * @brief UIManager class responsible for setting up and managing ImGui.
 */
struct UIManager {
    explicit UIManager(GLFWwindow* window_, ShaderManager& shader_manager_) noexcept;
    ~UIManager() noexcept;

    /**
     * @brief Starts a new ImGui frame.
     */
    void PrepareFrame() const noexcept;

    /**
     * @brief Renders the ImGui frame.
     */
    void RenderFrame() noexcept;

    static void SetUIVisibility(bool visibility) noexcept;

    static bool GetUIVisibility() noexcept;

    void ShowSavePopup() noexcept;

private:
    /**
     * @brief Cleans up ImGui resources.
     */
    void Shutdown() noexcept;

    void DrawSavePopup() noexcept;

private:
    GLFWwindow*    window;
    ShaderManager& shader_manager;

    static bool is_ui_visible;

    // Helpers
    ImVec4 text_color      = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    bool   show_save_popup = false;
};
