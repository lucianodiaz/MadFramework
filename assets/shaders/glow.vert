#version 120
void main()
{
    // Pasa color y UV al fragment
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_FrontColor  = gl_Color;

    // Transforma correctamente a clip space
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}