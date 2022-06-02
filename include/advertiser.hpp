#ifndef ADVERTISER_HPP
#define ADVERTISER_HPP

#include <GL/glew.h>
#include <array>
#include <mutex>
#include "shader_light.hpp"

struct ADVERTISER
{
	ADVERTISER() :
		m_projection(glm::ortho(0.0f, 1050.0f, 0.0f, 728.0f)),
		m_shader("shaders/advertiser/vertex.glsl", "shaders/advertiser/fragment.glsl"),
		m_tex_g{
			createTexture("assets/advertiser/enclume_g.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/celerite_g.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/confiscation_g.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/renfort_g.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/desordre_g.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/petrification_g.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/vachette_g.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/conversion_g.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/charge_g.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/entracte_g.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/solo_g.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/piege_g.tga", TEXTURE_TYPE::DIFFUSE, true)
	},
		m_tex_r{
			createTexture("assets/advertiser/enclume_r.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/celerite_r.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/confiscation_r.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/renfort_r.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/desordre_r.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/petrification_r.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/vachette_r.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/conversion_r.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/charge_r.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/entracte_r.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/solo_r.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/advertiser/piege_r.tga", TEXTURE_TYPE::DIFFUSE, true)
	},
		m_timer(0.0f),
		m_chosen_card(-1),
		m_grid_select(false),
		m_fruit_select(false),
		m_show(false),
		m_enemy(false),
		m_delete_enemy_card(false)
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glm::vec2 m_pos(-1150.0f, 728.0f * 0.5f - 30.0f);
		glm::vec2 m_size(1150.0f, 60.0f);
		float data[24] = {
			m_pos.x, m_pos.y + m_size.y, 0.0f, 1.0f,
			m_pos.x, m_pos.y, 0.0f, 0.0f,
			m_pos.x + m_size.x, m_pos.y, 1.0f, 0.0f,
			m_pos.x, m_pos.y + m_size.y, 0.0f, 1.0f,
			m_pos.x + m_size.x, m_pos.y, 1.0f, 0.0f,
			m_pos.x + m_size.x, m_pos.y + m_size.y, 1.0f, 1.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
	}

	void draw(float delta)
	{
		g_show_mutex.lock();
		int show = m_show;
		g_show_mutex.unlock();

		if (show)
		{
			g_chosen_card_mutex.lock();
			int card_id = m_chosen_card;
			g_chosen_card_mutex.unlock();
			m_timer += delta;
			glBindVertexArray(m_vao);
			m_shader.use();
			m_shader.setMatrix("proj", m_projection);
			glActiveTexture(GL_TEXTURE0);
			if (m_enemy) {
				glBindTexture(GL_TEXTURE_2D, m_tex_r[card_id].id);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, m_tex_g[card_id].id);
			}
			m_shader.setInt("card", 0);
			m_shader.setFloat("timer", m_timer);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);

			if (m_timer >= 1.5f)
			{
				m_timer = 0.0f;
				g_show_mutex.lock();
				m_show = false;
				g_show_mutex.unlock();
			}
		}
	}

	GLuint m_vao;
	GLuint m_vbo;
	glm::mat4 m_projection;
	Shader m_shader;
	std::array<Texture, 12> m_tex_g;
	std::array<Texture, 12> m_tex_r;
	float m_timer;

	// LOGIC
	int m_chosen_card; // if m_chosen_card != -1 and player click on another card, nothing happens
	bool m_grid_select;
	bool m_fruit_select;
	bool m_show;
	bool m_enemy; // enemy card is shown
	bool m_delete_enemy_card; // after card has been shown, delete the slot it is resting on
};

#endif