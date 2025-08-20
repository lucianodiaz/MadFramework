#version 330 core

out vec4 FragColor;

uniform float u_time;
uniform vec3 u_glowColor;
uniform vec4 u_baseColor;  // definido desde SFML

void main()
{
    float glowIntensity = 0.5 + 0.5 * sin(u_time * 5.0);

    vec3 finalColor = u_baseColor.rgb + u_glowColor * glowIntensity;

    FragColor = vec4(finalColor, u_baseColor.a);
}