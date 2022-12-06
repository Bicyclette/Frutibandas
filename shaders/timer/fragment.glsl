#version 410 core

out vec4 frag_color;

uniform float lightness;
uniform vec3 color;

void main()
{
	frag_color = vec4(color * lightness, 1.0f);
}
