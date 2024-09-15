#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;

out VS_OUT
{
	vec2 texCoords;
	vec3 normal;
	vec3 fragPos;
	mat4 viewMatrix;
	mat4 projMatrix;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	vs_out.texCoords = aTex;
	vec4 position = vec4(aPos, 1.0f);
	vec4 normal = vec4(aNorm, 0.0f);
	gl_Position = proj * view * model * position;
	vs_out.normal = vec3(transpose(inverse(model)) * normal);
	vs_out.fragPos = vec3(model * position);
	vs_out.viewMatrix = view;
	vs_out.projMatrix = proj;
}
