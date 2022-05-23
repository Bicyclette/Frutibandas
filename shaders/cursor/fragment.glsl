#version 440 core

out vec4 color;

uniform bool blink; // true = invisble

void main()
{
	if(blink){
		color = vec4(0.0f);
	} else {
		color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}