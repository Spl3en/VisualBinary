#version 150
// It was expressed that some drivers required this next line to function properly
precision highp float;

in  vec4 ex_Color;
out vec4 gl_FragColor;

void main (void) {
    gl_FragColor = vec4 (ex_Color);
}
