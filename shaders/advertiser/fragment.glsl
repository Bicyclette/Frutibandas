#version 410 core

out vec4 frag_color;

in vec2 texCoords;

uniform sampler2D card;

void main()
{
	frag_color = texture(card, texCoords);
}