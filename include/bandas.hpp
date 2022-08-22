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

constexpr int c_avatar_width{ 512 };
constexpr int c_avatar_height{ 512 };

constexpr glm::ivec2 c_pseudo_input_bottom_left_corner(450, 728 - 465);
constexpr glm::ivec2 c_pseudo_input_size(150, 30);

struct Player
{
	Avatar m_avatar;
	std::string m_pseudo;
	int m_team;
};

class Bandas
{
	public: // methods
		Bandas(Graphics& graphics);
		~Bandas();
		void createUI();
		inline void create_home_page();
		inline void create_game_page();
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
		void hovering_game_page(Page& page, int id);
		void click_game_page(Page& page, int id);
		void hold_left_click_game_page(Page& page, int id, std::array<int, 3> mouse_data);
		void draw_game_page(float delta);
		void draw_chat(float delta);
		void add_chat_message(std::string msg);
		void update_chat_input(std::bitset<10> user_input, std::string txt_input, float delta);
		void draw_avatar_game_page();

	public: // properties
		NetworkClient m_net;
		Graphics& m_graphics;
		UI m_ui;
		Text m_text;
		Writer m_writer;
		Mouse m_mouse;
		Player m_me;
		Player m_enemy;
		Music music;

	public: // game logic
		Board m_board;
		Logic m_logic;
};

#endif