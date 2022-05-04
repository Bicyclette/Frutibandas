#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTex;

out vec2 texCoords;

uniform mat4 proj;
uniform vec2 translate;

void main()
{
	gl_Position = proj * vec4(aPos + translate, 0.0f, 1.0f);
    texCoords = aTex;
}
