#version 440 core

layout (location = 0) in vec4 vertex; // vec2 pos & vec2 tex

out vec2 texCoords;

uniform mat4 proj;

void main()
{
    gl_Position = proj * vec4(vertex.xy, 0.0f, 1.0f);
    texCoords = vertex.zw;
}
