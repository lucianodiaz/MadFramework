#version 120
uniform sampler2D texture;     // base del humo
uniform sampler2D u_noise;     // ruido
uniform vec2  u_texSize;       // tamaño px de 'texture'
uniform vec2  u_noiseSize;     // tamaño px de 'u_noise'

uniform float u_time;
uniform float u_strength;      // 0.01–0.03
uniform float u_scale;         // 1–5
uniform float u_speed;         // 0–1
uniform float u_dissolve;      // 0–1
uniform vec3  u_tint;          // 0..1

varying vec2 v_uv;             // en píxeles (de SFML)
varying vec4 v_color;

void main() {
    vec2 uv     = v_uv / u_texSize;
    vec2 uvN    = v_uv / u_noiseSize;
    vec2 flow   = vec2(u_time * u_speed, -u_time * 0.5 * u_speed);

    // dos capas de ruido offset
    float n1 = texture2D(u_noise, uvN * u_scale + flow).r * 2.0 - 1.0;
    float n2 = texture2D(u_noise, uvN * (u_scale * 0.97) + flow + vec2(0.37, 0.61)).r * 2.0 - 1.0;
    vec2  offset = vec2(n1, n2) * u_strength;

    // muestra base (UV normalizados)
    vec4 base = texture2D(texture, uv + offset) * v_color;

    // *** clave: pre-multiplicar para evitar halos/bordes ***
    base.rgb *= base.a;

    // máscara suave para disolver
    float mask  = texture2D(u_noise, uvN * (u_scale * 0.5) + flow * 0.7 + vec2(7.0, 3.0)).r;
    float alpha = base.a * smoothstep(u_dissolve, 1.0, mask);

    vec3 rgb = base.rgb * u_tint;
    gl_FragColor = vec4(rgb, alpha);
}