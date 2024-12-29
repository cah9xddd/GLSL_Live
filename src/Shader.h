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
    bool CompileFromCurrentCode(ShaderType type_) noexcept;

    GLuint GetID() const noexcept;

    std::string_view GetCodeConst() const noexcept;
    std::string&     GetCode() noexcept;

    void DeleteShader() noexcept;

    std::string_view GetCompilationError() const noexcept;

    bool IsGood() const noexcept;

protected:

    void Compile(ShaderType type_) noexcept;

    bool CheckCompileErrors(GLuint shader, ShaderType type_) noexcept;

protected:
    std::string code;              /**< Shader code */
    std::string compilation_error; /**< Compilation error message */
    GLuint      id;                /**< Shader ID */
    bool        is_good;           /**< Flag indicating if the shader is compiled */
};
