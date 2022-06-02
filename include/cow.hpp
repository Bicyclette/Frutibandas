#ifndef COW_HPP
#define COW_HPP

#include <array>
#include "user_interface.hpp"

struct Cow
{
	Cow() :
		m_timer(0.0f),
		m_pos_cow(glm::vec2(-1, -1)),
		m_active(false),
		m_vachette(0, glm::vec2(-1, -1), glm::vec2(49, 84), 1050, 728),
		m_tex_cow{
			createTexture("assets/vachette1.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/vachette2.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/vachette3.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/vachette4.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/vachette5.tga", TEXTURE_TYPE::DIFFUSE, true)
	}
	{}

	void charge(float delta, int line_start = -1, int line_end = -1, int col = -1)
	{
		if (col == -1) { return; }

		// set proper image for animation
		m_timer += delta;
		float ratio = std::fmod(m_timer, 0.3f);
		int frame = (ratio / 0.3f) * 5;
		m_vachette.set_background_img_gl(m_tex_cow[frame].id);
		m_vachette.use_background_img_gl();

		// set pos of the cow
		m_pos_cow = glm::vec2(start_x + col * 49, 728 - m_timer * 320);
		m_vachette.set_pos(m_pos_cow);
		m_vachette.draw();

		// keep charging or not ?
		float arrive = (start_y - 49 * line_end);
		if (m_pos_cow.y <= (728 - (728 - arrive)))
		{
			m_active = false;
			m_timer = 0.0f;
		}
	}

	float m_timer;
	glm::vec2 m_pos_cow;
	Sprite m_vachette;
	bool m_active;
	std::array<Texture, 5> m_tex_cow;

	// board boundaries
	const int start_x = 525 - (49 * 4);
	const int end_x = 525 + (49 * 4);
	const int start_y = 645;
	const int end_y = 645 - (49 * 8);
};

#endif