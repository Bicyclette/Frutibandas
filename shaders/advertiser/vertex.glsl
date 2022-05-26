#version 440 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTex;

uniform mat4 proj;
uniform float timer;

out vec2 texCoords;

const float animation_time = 1.5f;

void main()
{
	float ratio;
	vec2 shift = vec2(1100.0f, 0.0f);
	if(timer <= 0.25f){
		ratio = timer * 4.0f;
	} else if (timer <= 1.25f) {
		ratio = 1.0f;
	} else if (timer >= 1.25f) {
		ratio = 1.0f + ((timer - 1.25f) * 4.0f);
	}

	gl_Position = proj * vec4(aPos + shift * ratio, 0.0f, 1.0f);
	texCoords = aTex;
}