#version 410

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 1) out outColor;

void main() {
	// vertex position in clip space
	gl_Position = vec4(inPosition, 1.0);

	outColor = inColor;
}
