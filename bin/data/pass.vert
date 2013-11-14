varying vec2 vTexCoord0;
varying vec2 vTexCoord1;
varying vec4 vColor;
void main(void) {
    vColor = gl_Color;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    vTexCoord0 = gl_MultiTexCoord0.st;
    vTexCoord1 = gl_MultiTexCoord0.st;
}