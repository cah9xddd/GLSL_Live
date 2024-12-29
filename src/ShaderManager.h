#pragma once

#include "PCH.h"

#include "Shader.h"
#include "ShaderProgram.h"

struct ShaderManager {
    explicit ShaderManager() noexcept;
    ~ShaderManager();

    Shader&        GetVertexShader() noexcept;
    Shader&        GetFragmentShader() noexcept;
    ShaderProgram& GetShaderProgram() noexcept;


    bool SaveFragmentShaderToPath(std::string_view fragment_shader_path_);

private:
private:

    Shader vertex_shader;
    Shader fragment_shader;

    ShaderProgram shader_program;
};