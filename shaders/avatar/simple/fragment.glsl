#version 410 core

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 fragColorMirrorX;

in vec2 texCoords;

uniform sampler2D image;

void main()
{
	vec4 image_sample = texture(image, texCoords);
	fragColor = image_sample;

	float diff = texCoords.x - 0.5f;
	float x = 0.5f - diff;
	image_sample = texture(image, vec2(x, texCoords.y));
	fragColorMirrorX = image_sample;
}