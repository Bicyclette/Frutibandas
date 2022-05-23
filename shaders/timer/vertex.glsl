#version 440 core

layout (location = 0) in vec2 aPos;

uniform bool orange;

void main()
{
	if(orange){
		gl_Position = vec4(188, 728-95, 0, 1.0f);
	} else {
		gl_Position = vec4(858, 728-95, 0, 1.0f);	
	}
}
