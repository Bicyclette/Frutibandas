#version 440 core

layout (location = 0) in vec2 aPos;

uniform mat4 proj;

void main()
{
	gl_Position = proj * vec4(aPos, 0.0f, 1.0f);
}