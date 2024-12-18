#include "Shader.h"

Shader::Shader() noexcept : id(0) {}

Shader::~Shader() { DeleteShader(); }

bool Shader::CompileFromText(std::string_view text_, ShaderType type_) noexcept
{
    code = text_;
    return Compile(type_);
}

GLuint Shader::GetID() const noexcept { return id; }

std::string_view Shader::GetCode() const noexcept { return code; }

void Shader::DeleteShader() noexcept
{
    if (id)
    {
        glDeleteShader(id);
        id = 0;
    }
}

bool Shader::Compile(ShaderType type_) noexcept
{
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
            return false;
    }

    const char* shader_code_cstr = code.c_str();
    glShaderSource(id, 1, &shader_code_cstr, nullptr);
    glCompileShader(id);

    CheckCompileErrors(id, type_);

    return true;
}

bool Shader::CheckCompileErrors(GLuint shader, ShaderType type_) noexcept
{
    GLint success;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLchar info_log[1024];
        glGetShaderInfoLog(shader, 1024, NULL, info_log);
        LOG_WARN("{} shader compilation error: {} ", magic_enum::enum_name(type_), info_log);
        return false;
    }
    return true;
}
