#pragma once

#include "PCH.h"

enum class ShaderType {
    VERTEX,   /**< Vertex shader */
    FRAGMENT, /**< Fragment shader */
    GEOMETRY, /**< Geometry shader */
    COUNT     /**< Total number of shader types */
};

struct Shader {
    explicit Shader() noexcept;

    ~Shader();

    bool CompileFromText(const std::string_view text_, ShaderType type_) noexcept;

    GLuint GetID() const noexcept;

    std::string_view GetCode() const noexcept;

    void DeleteShader() noexcept;

protected:

    bool Compile(ShaderType type_) noexcept;

    bool CheckCompileErrors(GLuint shader, ShaderType type_) noexcept;

protected:
    std::string code; /**< Shader code */
    GLuint      id;   /**< Shader ID */
};
