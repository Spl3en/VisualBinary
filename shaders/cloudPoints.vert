#version 150
// in_Position was bound to attribute index 0 and in_Color was bound to attribute index 1
in  vec3 in_Position;
in  vec4 in_Color;

uniform mat4 MVMatrix;
uniform mat4 MPMatrix;

// We output the ex_Color variable to the next shader in the chain
out vec4 ex_Color;
void main(void) {
	gl_PointSize = 2.0;

	vec4 pos = vec4(in_Position, 1.0);
    gl_Position = MPMatrix * MVMatrix * pos;
    ex_Color = in_Color;
}
