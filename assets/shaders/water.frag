#version 120

uniform sampler2D texture;
uniform float time;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    
    // Parámetros para controlar la distorsión
    float waveStrength = 0.02;
    float waveSpeed = 2.0;
    
    // Crear ondas usando función seno
    float distortionX = sin(uv.y * 15.0 + time * waveSpeed) * waveStrength;
    float distortionY = sin(uv.x * 15.0 + time * waveSpeed) * waveStrength;
    
    // Aplicar distorsión a las coordenadas UV
    vec2 distortedUV = uv + vec2(distortionX, distortionY);
    
    // Obtener color de la textura con las coordenadas distorsionadas
    vec4 color = texture2D(texture, distortedUV);
    
    // Añadir un ligero tinte azulado para efecto de agua
    color.rgb += vec3(0.0, 0.1, 0.2) * 0.1;
    
    gl_FragColor = color;
}