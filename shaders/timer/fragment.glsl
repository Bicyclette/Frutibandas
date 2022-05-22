#version 460 core

out vec4 frag_color;

uniform float on_off;
uniform bool flickering;
uniform vec3 color;

void main()
{
	if(flickering) {
		frag_color = vec4(color * on_off, 1.0f);
	} else {
		frag_color = vec4(color, 1.0f);
	}
}
