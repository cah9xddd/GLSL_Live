#version 460 core

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

}