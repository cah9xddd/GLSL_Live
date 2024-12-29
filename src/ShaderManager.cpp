#include "ShaderManager.h"

#include "Utils.h"

ShaderManager::ShaderManager() noexcept
{
    auto vertex_shader_source = ReadTextFromFile("shaders/default/default_vertex.glsl");
    if (vertex_shader_source.empty())
    {
        LOG_CRITICAL("Failed to load vertex shader from path: {}",
                     "shaders/default/default_vertex.glsl");

        vertex_shader_source = R"(#version 460 core

layout (location = 0) in vec3 Pos;  // Input vertex position

uniform vec2 in_resolution;   // Viewport resolution (in pixels)

out vec2 fragCoord;
out vec2 iResolution;

void main()
{
    gl_Position = vec4(Pos, 1.0);

    fragCoord.x = in_resolution.x * (gl_Position.x + 1.0) / 2.0;
    fragCoord.y = in_resolution.y * (gl_Position.y + 1.0) / 2.0;

    iResolution = in_resolution;

})";
        WriteTextToFile("shaders/default/default_vertex.glsl", vertex_shader_source);
    }

    auto fragment_shader_source = ReadTextFromFile("shaders/latest_fragment.glsl");

    if (fragment_shader_source.empty())
    {
        LOG_CRITICAL("Failed to load latest fragment shader from path: {}",
                     "shaders/latest_fragment.glsl");

        LOG_INFO("Loading default fragment shader");
        fragment_shader_source = ReadTextFromFile("shaders/default/default_fragment.glsl");

        if (fragment_shader_source.empty())
        {
            LOG_CRITICAL("Failed to load default fragment shader from path: {}",
                         "shaders/default/default_fragment.glsl");

            fragment_shader_source = R"(#version 460 core

in vec2 fragCoord;          // Fragment Coordinate from 0 to iResolutin x or y
in vec2 iResolution;        // Window resolution
uniform float iTime;        // Elapsed time in seconds
uniform float iTimeDelta;   // Frame time delta
uniform int iFrame;         // Current frame number
uniform vec4 iMouse;        // Mouse input (position and click)

out vec4 fragColor;


void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = fragCoord.xy / iResolution.xy;

    vec3 col = 0.5 + 0.5 * cos(iTime + uv.xyx + vec3(0, 2, 4)); // Simple color gradient

    fragColor = vec4(col, 1.0);
}

void main()
{
    mainImage(fragColor, fragCoord);
}
)";
            WriteTextToFile("shaders/default/default_fragment.glsl", fragment_shader_source);
        }
    }


    vertex_shader.CompileFromText(vertex_shader_source, ShaderType::VERTEX);

    fragment_shader.CompileFromText(fragment_shader_source, ShaderType::FRAGMENT);

    shader_program = ShaderProgram(vertex_shader, fragment_shader);
}

ShaderManager::~ShaderManager() {}

Shader& ShaderManager::GetVertexShader() noexcept { return vertex_shader; }

Shader& ShaderManager::GetFragmentShader() noexcept { return fragment_shader; }

ShaderProgram& ShaderManager::GetShaderProgram() noexcept { return shader_program; }

bool ShaderManager::SaveFragmentShaderToPath(std::string_view fragment_shader_path_)
{
    LOG_INFO("CODE : {}", fragment_shader.GetCode());
    return WriteTextToFile(fragment_shader_path_, fragment_shader.GetCode());
}
