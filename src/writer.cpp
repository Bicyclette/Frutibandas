#include "writer.hpp"

void Writer::write(std::string character, std::bitset<10> userInputs, float delta, int boundX, glm::vec3 cursor_shape)
{
	if (character.size() > 1) {
		return;
	}

	WRITE_ACTION writeAction;
	if (userInputs.test(6))
		writeAction = WRITE_ACTION::ERASE;
	else if (userInputs.test(8))
		writeAction = WRITE_ACTION::CURSOR_LEFT;
	else if (userInputs.test(7))
		writeAction = WRITE_ACTION::CURSOR_RIGHT;
	else if (character == "")
		writeAction = WRITE_ACTION::NOTHING;
	else
		writeAction = WRITE_ACTION::CHARACTER;

	write_aux(writeAction, character, delta, boundX, cursor_shape);
}

Writer::~Writer()
{
	m_cursor.cleanup();
}

void Writer::write_aux(WRITE_ACTION writeAction, std::string& character, float delta, int boundX, glm::vec3 cursor_shape)
{
	if (writeAction == WRITE_ACTION::NOTHING)
	{
		m_lastCharacter = "";
		m_deltaWrite = 0.0f;
		m_lastWriteAction = writeAction;
		return;
	}
	if (m_lastWriteAction == writeAction || (writeAction == WRITE_ACTION::CHARACTER && character == m_lastCharacter))
	{
		m_deltaWrite += delta;
		if (m_deltaWrite < 1.0f)
			return;
		else
		{
			if (m_deltaWrite < 1.05f)
				return;
			else
				m_deltaWrite = 1.0f;
		}
	}
	if (writeAction == WRITE_ACTION::CHARACTER)
	{
		if (character != m_lastCharacter)
		{
			m_lastCharacter = character;
			m_deltaWrite = 0.0f;
		}
		if (boundX > (cursor_shape.x + cursor_shape.z))
		{
			m_textInput[m_cursor.m_focus].insert(m_cursor.m_pos, character.data(), character.size());
			m_textSectionsWidth[m_cursor.m_focus].insert(m_textSectionsWidth[m_cursor.m_focus].begin() + m_cursor.m_pos, character.size());
			m_cursor.m_pos++;
		}
	}
	else if (writeAction == WRITE_ACTION::ERASE)
	{
		if (m_textInput[m_cursor.m_focus].size() > 0 && m_cursor.m_pos > 0)
		{
			int sectionWidth = m_textSectionsWidth[m_cursor.m_focus][m_cursor.m_pos - 1];
			m_textSectionsWidth[m_cursor.m_focus].erase(m_textSectionsWidth[m_cursor.m_focus].begin() + m_cursor.m_pos - 1);
			m_textInput[m_cursor.m_focus].erase(m_cursor.m_pos - sectionWidth, sectionWidth);
			m_cursor.m_pos -= sectionWidth;
		}
	}
	else if (writeAction == WRITE_ACTION::CURSOR_LEFT)
	{
		m_cursor.m_pos = my_max(0, m_cursor.m_pos - 1);
	}
	else if (writeAction == WRITE_ACTION::CURSOR_RIGHT)
	{
		if (m_cursor.m_pos < m_textInput[m_cursor.m_focus].size()) {
			m_cursor.m_pos = my_min(static_cast<int>(m_textInput[m_cursor.m_focus].size()), m_cursor.m_pos + 1);
		}
	}

	m_lastWriteAction = writeAction;
}