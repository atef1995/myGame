#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform mat4 model; // Model transformation matrix
uniform mat4 projection; // Projection matrix

void main() {
    gl_Position = projection * model * vec4(aPos, 1.0);  // Transform the vertex position
    vertexColor = aColor;
}
