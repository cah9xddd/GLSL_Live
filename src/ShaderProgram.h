#pragma once

#include "PCH.h"

#include "Shader.h"

template<typename T>
concept AllowedUniformType =
    std::is_same_v<T, glm::vec2> || std::is_same_v<T, glm::vec3> || std::is_same_v<T, glm::vec4>
    || std::is_same_v<T, glm::mat2> || std::is_same_v<T, glm::mat3> || std::is_same_v<T, glm::mat4>
    || std::is_same_v<T, int> || std::is_same_v<T, float> || std::is_same_v<T, bool>;

struct ShaderProgram {
    explicit ShaderProgram() noexcept;
    explicit ShaderProgram(Shader& vertex_, Shader& fragment_) noexcept;

    explicit ShaderProgram(const ShaderProgram&)    = delete;
    ShaderProgram& operator= (const ShaderProgram&) = delete;

    explicit ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator= (ShaderProgram&& other) noexcept;

    ~ShaderProgram();

    void Use() noexcept;

    GLuint GetID() const noexcept;

    template<AllowedUniformType T>
    void SetUniform(std::string_view name, const T& value) const;
    // Specializations for basic types
    void SetUniform(std::string_view name, const bool& value) const;
    void SetUniform(std::string_view name, const int& value) const;
    void SetUniform(std::string_view name, const float& value) const;
    // Specializations for vector types
    void SetUniform(std::string_view name, const glm::vec2& value) const;
    void SetUniform(std::string_view name, const glm::vec3& value) const;
    void SetUniform(std::string_view name, const glm::vec4& value) const;
    // Specializations for matrix types
    void SetUniform(std::string_view name, const glm::mat2& value) const;
    void SetUniform(std::string_view name, const glm::mat3& value) const;
    void SetUniform(std::string_view name, const glm::mat4& value) const;

    /**
     * @brief Template function which gets the uniform value from the shader program.
     * @tparam T The type of the uniform value.
     * @param name The name of the uniform.
     * @return The uniform value.
     */
    template<AllowedUniformType T>
    void GetUniform(std::string_view name, T& storage_) const;
    // Specializations for basic types
    void GetUniform(std::string_view name, bool& storage_) const;
    void GetUniform(std::string_view name, int& storage_) const;
    void GetUniform(std::string_view name, float& storage_) const;
    // Specializations for vector types
    void GetUniform(std::string_view name, glm::vec2& storage_) const;
    void GetUniform(std::string_view name, glm::vec3& storage_) const;
    void GetUniform(std::string_view name, glm::vec4& storage_) const;
    // Specializations for matrix types
    void GetUniform(std::string_view name, glm::mat2& storage_) const;
    void GetUniform(std::string_view name, glm::mat3& storage_) const;
    void GetUniform(std::string_view name, glm::mat4& storage_) const;


private:

    void AttachShader(const Shader& shader);

    bool Link() noexcept;

    void DeleteProgram() noexcept;

private:
    GLuint              id;               /**< The ID of the shader program. */
    std::vector<GLuint> attached_shaders; /**< The list of attached shaders. */
};
