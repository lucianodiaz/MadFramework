// GLSL #version 120
uniform sampler2D u_texture;
uniform vec4 u_color1;
uniform vec4 u_color2;
uniform float u_threshold;   // 0..sqrt(3) aprox si en RGB lineal
uniform float u_intensity;   // fuerza del tinte
uniform float u_opacity;     // 0..1
uniform vec4 u_glowColor;    // color del glow
varying vec2 v_texCoord;     // pásalo desde el vertex shader [0..1]

void main() {
    vec4 pixelColor = texture2D(u_texture, v_texCoord);

    float d1 = distance(pixelColor.rgb, u_color1.rgb);
    float d2 = distance(pixelColor.rgb, u_color2.rgb);

    float mask = (d1 < u_threshold || d2 < u_threshold) ? 1.0 : 0.0;

    // cantidad de mezcla (no toques alpha del glow)
    float w = u_opacity * mask;

    vec3 mixed = mix(pixelColor.rgb, u_glowColor.rgb * u_intensity, w);

    gl_FragColor = vec4(mixed, pixelColor.a);
}