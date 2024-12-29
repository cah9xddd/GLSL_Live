#version 460 core

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