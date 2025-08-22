#version 120
varying vec2 v_uv;    // en PIXELES (como los pasa SFML)
varying vec4 v_color;
void main() {
    v_uv    = gl_TexCoord[0].xy;       // SFML entrega coords en px
    v_color = gl_FrontColor;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}