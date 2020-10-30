#version 410

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
//lec5
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outColor;
layout(location = 2) out vec3 outNormal;

uniform mat4 MVP;
//lec 5
uniform mat4 View;
uniform mat4 Model;
uniform vec3 LightPos;


void main() {

    gl_Position = MVP * vec4(inPosition, 1.0);

    //lec 5
    //pass vertex pos in world space to frag shader
    outPos = (Model * vec4(inPosition, 1.0)).xyz;

    //normals
    outNormal = (Model * vec4(inNormal, 0.0)).xyz;

    ///////////////

    outColor = inColor;

}