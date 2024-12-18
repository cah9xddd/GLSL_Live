#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(Shader& vertex_, Shader& fragment_) noexcept : id(0)
{
    id = glCreateProgram();

    if (id != 0)
    {
        AttachShader(vertex_);
        AttachShader(fragment_);

        if (!Link())
        {
            glDeleteProgram(id);
            id = 0;
        }
    }
}

ShaderProgram::~ShaderProgram() { DeleteProgram(); }

void ShaderProgram::Use() noexcept { glUseProgram(id); }

void ShaderProgram::AttachShader(const Shader& shader)
{
    glAttachShader(id, shader.GetID());

    attached_shaders.push_back(shader.GetID());
}

bool ShaderProgram::Link() noexcept
{
    glLinkProgram(id);

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

void ShaderProgram::DeleteProgram() noexcept
{
    if (id) { glDeleteProgram(id); }

    for (const auto& shader_id : attached_shaders)
    {
        glDetachShader(id, shader_id);  // Detach shaders if needed
    }
}

GLuint ShaderProgram::GetID() const noexcept { return id; }

void ShaderProgram::SetUniform(std::string_view name, const bool& value) const
{
    glUniform1i(glGetUniformLocation(id, name.data()), (int)value);
}

void ShaderProgram::SetUniform(std::string_view name, const int& value) const
{
    glUniform1i(glGetUniformLocation(id, name.data()), value);
}

void ShaderProgram::SetUniform(std::string_view name, const float& value) const
{
    glUniform1f(glGetUniformLocation(id, name.data()), value);

    // TODO Correct way? should i use it? maybe not cause we know what name we are looking for
    // GLint location = glGetUniformLocation(program_id, name.data());
    // if (location != -1) { glUniform1f(location, value); }
}

void ShaderProgram::SetUniform(std::string_view name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(id, name.data()), 1, glm::value_ptr(value));
}

void ShaderProgram::SetUniform(std::string_view name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(id, name.data()), 1, glm::value_ptr(value));
}

void ShaderProgram::SetUniform(std::string_view name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(id, name.data()), 1, glm::value_ptr(value));
}

void ShaderProgram::SetUniform(std::string_view name_, const glm::mat2& value_) const
{
    glUniformMatrix2fv(glGetUniformLocation(id, name_.data()), 1, GL_FALSE, &value_[0][0]);
}

void ShaderProgram::SetUniform(std::string_view name_, const glm::mat3& value_) const
{
    glUniformMatrix3fv(glGetUniformLocation(id, name_.data()), 1, GL_FALSE, &value_[0][0]);
}

void ShaderProgram::SetUniform(std::string_view name_, const glm::mat4& value_) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name_.data()), 1, GL_FALSE, &value_[0][0]);
}

void ShaderProgram::GetUniform(std::string_view name, bool& storage_) const
{
    int value;
    glGetUniformiv(id, glGetUniformLocation(id, name.data()), &value);
    storage_ = value;
}

void ShaderProgram::GetUniform(std::string_view name, int& storage_) const
{
    glGetUniformiv(id, glGetUniformLocation(id, name.data()), &storage_);
}

void ShaderProgram::GetUniform(std::string_view name, float& storage_) const
{
    glGetUniformfv(id, glGetUniformLocation(id, name.data()), &storage_);
}

void ShaderProgram::GetUniform(std::string_view name, glm::vec2& storage_) const
{
    glGetUniformfv(id, glGetUniformLocation(id, name.data()), glm::value_ptr(storage_));
}

void ShaderProgram::GetUniform(std::string_view name, glm::vec3& storage_) const
{
    glGetUniformfv(id, glGetUniformLocation(id, name.data()), glm::value_ptr(storage_));
}

void ShaderProgram::GetUniform(std::string_view name, glm::vec4& storage_) const
{
    glGetUniformfv(id, glGetUniformLocation(id, name.data()), glm::value_ptr(storage_));
}

void ShaderProgram::GetUniform(std::string_view name, glm::mat2& storage_) const
{
    glGetUniformfv(id, glGetUniformLocation(id, name.data()), &storage_[0][0]);
}

void ShaderProgram::GetUniform(std::string_view name, glm::mat3& storage_) const
{
    glGetUniformfv(id, glGetUniformLocation(id, name.data()), &storage_[0][0]);
}

void ShaderProgram::GetUniform(std::string_view name, glm::mat4& storage_) const
{
    glGetUniformfv(id, glGetUniformLocation(id, name.data()), &storage_[0][0]);
}
