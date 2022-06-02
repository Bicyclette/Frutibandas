#ifndef ANVIL_HPP
#define ANVIL_HPP

#include "user_interface.hpp"

struct Anvil
{
	Anvil() :
		m_active(false),
		m_sprite(0, glm::vec2(-1, -1), glm::vec2(49, 49), 1050, 728),
		m_pos(glm::vec2(-1, -1)),
		m_timer(0.0f),
		m_tex(createTexture("assets/anvil.tga", TEXTURE_TYPE::DIFFUSE, true))
	{
		m_sprite.set_background_img_gl(m_tex.id);
		m_sprite.use_background_img_gl();
	}

	void fall(float delta, int line = -1, int col = -1)
	{
		if (line == -1 && col == -1) { return; }
		m_timer += delta;
		m_pos = glm::vec2(start_x + col * 49, 728 - m_timer * 650);
		m_sprite.set_pos(m_pos);
		m_sprite.draw();
		// keep falling or not ?
		float arrive = (start_y - 49 * line);
		if (m_pos.y <= (728 - (728 - arrive)))
		{
			m_active = false;
			m_pos = glm::vec2(-1, -1);
			m_timer = 0.0f;
		}
	}

	float m_timer;
	glm::vec2 m_pos;
	Sprite m_sprite;
	bool m_active;
	Texture m_tex;

	// board boundaries
	const int start_x = 525 - (49 * 4);
	const int end_x = 525 + (49 * 4);
	const int start_y = 645;
	const int end_y = 645 - (49 * 8);
};

#endif