varying vec4 vColor;

void main() {
    
    if(gl_Color.a >= 0.0) {
        gl_FragColor = vec4(vec3(0.0), 1.0);
    }
    else {
        gl_FragColor = vec4(vec3(1.0), 0.0);
    }
}