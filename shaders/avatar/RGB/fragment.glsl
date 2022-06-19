#version 440 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D image;
uniform vec3 color; // RGB color space

void main()
{
	vec4 image_sample = texture(image, texCoords);
	fragColor = vec4(color * image_sample.rgb, image_sample.a);
}