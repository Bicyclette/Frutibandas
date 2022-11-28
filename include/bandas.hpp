#ifndef BANDAS_HPP
#define BANDAS_HPP

#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include "avatar.hpp"
#include "user_interface.hpp"
#include "mouse.hpp"
#include "graphics.hpp"
#include "writer.hpp"
#include "window.hpp"
#include "communication.hpp"
#include "network_client.hpp"
#include "music.hpp"
#include "logic.hpp"
#include "card.hpp"

constexpr int c_avatar_width{ 512 };
constexpr int c_avatar_height{ 512 };

constexpr glm::ivec2 c_pseudo_input_bottom_left_corner(450, 728 - 465);
constexpr glm::ivec2 c_pseudo_input_size(150, 30);

constexpr glm::vec2 c_orange_chrono_pos(188, 728 - 95);
constexpr glm::vec2 c_banana_chrono_pos(858, 728 - 95);

constexpr glm::vec3 c_green(0.0f, 0.694f, 0.082f);
constexpr glm::vec3 c_red(1,0,0);

constexpr glm::vec2 c_shift_up(0, 45);
constexpr glm::vec2 c_shift_down(0, -45);
constexpr glm::vec2 c_shift_right(44, 0);
constexpr glm::vec2 c_shift_left(-44, 0);

struct Chrono
{
	float m_time;
	float m_second;
	glm::vec3 m_color;
	float m_lightness;
	Shader m_chrono_shader;
	GLuint m_vao[2];
	GLuint m_vbo[2];

	Chrono() :
		m_time(600.0f),
		m_second(0.0f),
		m_lightness(1.0f),
		m_chrono_shader("shaders/timer/vertex.glsl", "shaders/timer/geometry.glsl", "shaders/timer/fragment.glsl")
	{
		glGenVertexArrays(2, m_vao);
		glGenBuffers(2, m_vbo);

		glBindVertexArray(m_vao[0]);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2), glm::value_ptr(c_orange_chrono_pos), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		glBindVertexArray(m_vao[1]);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2), glm::value_ptr(c_banana_chrono_pos), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	void cleanup()
	{
		for (int i = 0; i < 2; ++i)
		{
			glBindVertexArray(m_vao[i]);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDeleteBuffers(1, &m_vbo[i]);
			glBindVertexArray(0);
			glDeleteVertexArrays(1, &m_vao[i]);
		}
	}

	void reset()
	{
		m_time = 600.0f;
		m_lightness = 1.0f;
	}

	std::string to_string()
	{
		float minutes = std::floor(m_time / 60.0f);
		float secondes = m_time - (minutes * 60.0f);
		std::string min_str = std::to_string(static_cast<int>(minutes));
		std::string sec_str = std::to_string(static_cast<int>(secondes));
		if (sec_str.length() == 1) {
			sec_str = "0" + sec_str;
		}
		if (min_str.length() == 1) {
			min_str = " " + min_str;
		}
		return std::string(min_str + " : " + sec_str);
	}

	void draw(int team, Logic& logic, float delta)
	{
		// update look
		if (logic.turn == team && logic.move.dir == -1 && !logic.change_turn) {
			m_time = (m_time > 0.0f) ? m_time - delta : 0.0f;
			m_second += delta;
			if (m_second >= 0.5f && m_second <= 1.0f) {
				m_lightness = 0.5f;
			}
			else if (m_second > 1.0f) {
				m_second = 0.0f;
				m_lightness = 1.0f;
			}
		}
		else {
			m_second = 0.0f;
			m_lightness = 1.0f;
		}
		float ratio = m_time / 600.0f;
		// draw
		glBindVertexArray(m_vao[team]);
		m_chrono_shader.use();
		m_chrono_shader.setFloat("ratio", ratio);
		m_chrono_shader.setFloat("lightness", m_lightness);
		m_chrono_shader.setVec3f("color", m_color);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	}
};

struct Player
{
	Avatar m_avatar;
	std::string m_pseudo;
	int m_team;
	Chrono m_chrono;
};

class Bandas
{
	public: // methods
		Bandas(Graphics& graphics);
		~Bandas();
		void createUI();
		inline void create_home_page();
		inline void create_game_page();
		void init_cards(std::string cards);
		void update_home_page(std::bitset<10> user_input, std::string txt_input, float delta);
		void hovering_home_page(Page& page, int id);
		void click_home_page(Page& page, int id);
		void update_avatar(Page& page, int id);
		void draw_home_page(float delta);
		void start_game();
		void quit_game();
		void enemy_gave_up();
		void enemy_disconnected();
		void lost_server_connection();

		void update_game_page(std::array<int, 3> mouse_data, std::bitset<10> user_input, std::string txt_input, float delta);
		void hovering_game_page(Page& page, int id, glm::ivec2 mouse_coords);
		void click_game_page(Page& page, int id, glm::ivec2 mouse_coords);
		void click_on_orange_card(int index);
		void click_on_banana_card(int index);
		void hold_left_click_game_page(Page& page, int id, std::array<int, 3> mouse_data);
		void draw_game_page(float delta);
		void draw_chat(float delta);
		void add_chat_message(std::string msg);
		void update_chat_input(std::bitset<10> user_input, std::string txt_input, float delta);
		void draw_avatar_game_page();
		void draw_cards();
		void remove_card(int id);
		void process_card_effect(int id, bool delay = false);
		void get_card(int card_id, int owner);

	public: // properties
		NetworkClient m_net;
		Graphics& m_graphics;
		UI m_ui;
		UI m_ui_advertiser;
		UI m_ui_end_game;
		Text m_text;
		Writer m_writer;
		Mouse m_mouse;
		Player m_me;
		Player m_enemy;
		Music music;
		std::string m_num_connected_players;
		float m_delta_poll_cp;

	public: // game logic
		Board m_board;
		Logic m_logic;
		Card m_orange_cards[3];
		Card m_banana_cards[3];
		Anvil m_anvil;
		Cow m_cow;
		std::vector<struct Advertiser> m_advertiser;
};

#endif
