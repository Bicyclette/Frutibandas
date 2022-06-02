#ifndef TIMER_HPP
#define TIMER_HPP

#include <GL/glew.h>
#include "shader_light.hpp"

struct Timer
{
	Timer() :
		m_shader("shaders/timer/vertex.glsl", "shaders/timer/geometry.glsl", "shaders/timer/fragment.glsl")
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		float data[2] = { 0.0f, 0.0f };

		glBufferData(GL_ARRAY_BUFFER, 2, data, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	void draw(float ratio, glm::vec3 color, float on_off, int fruit, bool flickering)
	{
		glBindVertexArray(m_vao);
		m_shader.use();
		m_shader.setFloat("ratio", ratio);
		m_shader.setBool("flickering", flickering);
		m_shader.setFloat("on_off", on_off);
		m_shader.setVec3f("color", color);
		m_shader.setBool("orange", (fruit == 0) ? true : false);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	}

	void cleanup()
	{
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_vbo);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_vao);
	}

	GLuint m_vao;
	GLuint m_vbo;
	Shader m_shader;
};

#endif