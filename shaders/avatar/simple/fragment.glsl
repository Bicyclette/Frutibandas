#version 440 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D image;

void main()
{
	vec4 image_sample = texture(image, texCoords);
	fragColor = image_sample;
}