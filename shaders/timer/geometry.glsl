#version 410 core

layout (points) in;
layout (triangle_strip, max_vertices = 200) out;

uniform float ratio;

void main()
{
	float aspect_ratio = 1050.0f / 728.0f;
	vec4 center = gl_in[0].gl_Position;
	// normalize center
	center.x = (center.x / 1050.0f) * 2.0f - 1.0f;
	center.y = (center.y / 728.0f) * 2.0f - 1.0f;

	float angle_step = radians(6.0f);
	float max_angle = radians(360.0f * ratio);
	float halfPI = radians(90.0f);

	for(float a = halfPI; a <= max_angle + halfPI; a += angle_step)
	{
		gl_Position = center;
		EmitVertex();
		gl_Position = center + 0.05f * vec4(cos(a), sin(a) * aspect_ratio, 0, 0);
		EmitVertex();
		gl_Position = center + 0.05f * vec4(cos(a+angle_step), sin(a+angle_step) * aspect_ratio, 0, 0);
		EmitVertex();
		EndPrimitive();
	}
}
