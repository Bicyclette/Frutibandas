#version 440 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D image;
uniform vec3 color; // RGB color space
uniform bool mirrorX;

void main()
{
	vec4 image_sample;
	if(!mirrorX)
	{
		image_sample = texture(image, texCoords);
	}
	else
	{
		float x = ((texCoords.x - 0.5) * 2.0f) * -1.0f;
		x = (x + 1.0f) * 0.5f;
		image_sample = texture(image, vec2(x, texCoords.y));
	}
	
	fragColor = vec4(color * image_sample.rgb, image_sample.a);
}