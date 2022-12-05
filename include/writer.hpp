#ifndef WRITER_HPP
#define WRITER_HPP

#include <bitset>
#include <vector>
#include <array>
#include "shader_light.hpp"
#include "helpers.hpp"

enum class WRITE_ACTION
{
	ERASE,
	CURSOR_LEFT,
	CURSOR_RIGHT,
	CHARACTER,
	NOTHING
};

struct Cursor
{
	Cursor(int screenW, int screenH) :
		m_pos(0),
		m_focus(2),
		m_blink(0.0f),
		m_blink_ctrl(false),
		m_projection(glm::ortho(0.0f, static_cast<float>(screenW), 0.0f, static_cast<float>(screenH))),
		m_shader("shaders/cursor/vertex.glsl", "shaders/cursor/fragment.glsl")
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		float data[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	void draw(glm::vec3 cursor_shape, float delta)
	{
		// blink
		m_blink += delta;
		if (m_blink >= 1.0f) {
			m_blink = 0.0f;
			m_blink_ctrl = false;
		}
		else if (m_blink >= 0.5f) {
			m_blink_ctrl = true;
		}

		// create geometry data
		float data[4] = {
			cursor_shape.x, cursor_shape.y,
			cursor_shape.x, cursor_shape.y + cursor_shape.z
		};
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(float), data);

		// draw
		glBindVertexArray(m_vao);
		m_shader.use();
		m_shader.setMatrix("proj", m_projection);
		m_shader.setBool("blink", m_blink_ctrl);
		glDrawArrays(GL_LINE_STRIP, 0, 2);
	}

	void cleanup()
	{
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_vbo);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_vao);
	}

	int m_focus; // 0 = pseudo, 1 = chat, 2 = not writing
	int m_pos;
	float m_blink;
	bool m_blink_ctrl;
	Shader m_shader;
	glm::mat4 m_projection;
	GLuint m_vao;
	GLuint m_vbo;
};

class Writer
{
public:
	Writer(int screenW, int screenH) :
		m_cursor(screenW, screenH)
	{
		m_deltaWrite = 0.0f;
		m_lastWriteAction = WRITE_ACTION::NOTHING;
		m_lastCharacter = "";
	}
	void init(std::string data)
	{
		m_textInput[0] = data;
		std::vector<int>& sections = m_textSectionsWidth[0];
		for(char c : m_textInput[0])
		{
			sections.push_back(1);
		}
		m_lastCharacter = m_textInput[0][m_textInput[0].size()-1];
		m_cursor.m_pos = m_textInput[0].size();
		m_deltaWrite = 0.007f;
	}
	~Writer();

	void write(std::string character, std::bitset<10> userInputs, float delta, int boundX, glm::vec3 cursor_shape);

	std::array<std::string, 2> m_textInput = { "", "" };
	std::array<std::vector<int>, 2> m_textSectionsWidth;
	std::vector<std::string> m_chatLog;
	Cursor m_cursor;
	float m_deltaWrite;
	WRITE_ACTION m_lastWriteAction;
	std::string m_lastCharacter;

private:
	void write_aux(WRITE_ACTION writeAction, std::string& character, float delta, int boundX, glm::vec3 cursor_shape);
};

#endif
