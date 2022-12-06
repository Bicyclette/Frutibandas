#version 410 core

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 fragColorMirrorX;

in vec2 texCoords;

uniform sampler2D image;
uniform vec3 color; // RGB color space

void main()
{
	vec4 image_sample = texture(image, texCoords);
	fragColor = vec4(color * image_sample.rgb, image_sample.a);

	float diff = texCoords.x - 0.5f;
	float x = 0.5f - diff;
	image_sample = texture(image, vec2(x, texCoords.y));
	fragColorMirrorX = vec4(color * image_sample.rgb, image_sample.a);
}