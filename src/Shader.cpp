#include "Shader.h"

Shader::Shader() noexcept : id(0), is_good(false)
{
    compilation_error.reserve(1024);
    code.reserve(1024 * 10);
}

Shader::~Shader() { DeleteShader(); }

bool Shader::CompileFromText(std::string_view text_, ShaderType type_) noexcept
{
    code = text_;
    Compile(type_);
    return is_good;
}

bool Shader::CompileFromCurrentCode(ShaderType type_) noexcept
{
    Compile(type_);
    return is_good;
}

GLuint Shader::GetID() const noexcept { return id; }

std::string_view Shader::GetCodeConst() const noexcept { return code; }

std::string& Shader::GetCode() noexcept { return code; }

void Shader::DeleteShader() noexcept
{
    if (id)
    {
        glDeleteShader(id);
        id = 0;
    }
}

std::string_view Shader::GetCompilationError() const noexcept { return compilation_error; }

bool Shader::IsGood() const noexcept { return is_good; }

void Shader::Compile(ShaderType type_) noexcept
{
    // If the shader is already compiled and existed, delete it
    if (id != 0) { DeleteShader(); }

    switch (type_)
    {
        case ShaderType::VERTEX :
            id = glCreateShader(GL_VERTEX_SHADER);
            break;
        case ShaderType::FRAGMENT :
            id = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case ShaderType::GEOMETRY :
            id = glCreateShader(GL_GEOMETRY_SHADER);
            break;
        default :
            LOG_WARN("Unknown shader type");
            is_good = false;
    }

    const char* shader_code_cstr = code.c_str();
    glShaderSource(id, 1, &shader_code_cstr, nullptr);
    glCompileShader(id);

    is_good = CheckCompileErrors(id, type_);
}

bool Shader::CheckCompileErrors(GLuint shader, ShaderType type_) noexcept
{
    GLint success;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char error_info[1024];
        glGetShaderInfoLog(shader, 1024, NULL, error_info);
        compilation_error = error_info;
        // LOG_WARN("Shader compilation error: {}", compilation_error);
        return false;
    }
    return true;
}
