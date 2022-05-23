#version 440 core

layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * vec4(aPos, 1.0f);
}
