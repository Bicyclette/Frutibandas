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

constexpr int c_screen_width{ 1050 };
constexpr int c_screen_height{ 728 };

constexpr int c_avatar_width{ 512 };
constexpr int c_avatar_height{ 512 };

constexpr glm::ivec2 c_pseudo_input_bottom_left_corner(450, 728 - 465);
constexpr glm::ivec2 c_pseudo_input_size(150, 30);

struct Player
{
	Avatar m_avatar;
	std::string m_pseudo;
};

class Bandas
{
	public: // methods
		Bandas(Graphics& graphics);
		~Bandas();
		void createUI();
		void update_home_page(std::bitset<10> user_input, std::string txt_input, float delta);
		void hovering_home_page(Page& page, int id);
		void click_home_page(Page& page, int id);
		void update_avatar(Page& page, int id);
		void draw_home_page(float delta);
		void update_game_page(std::bitset<10> user_input, std::string txt_input, float delta);
		void draw_game_page(float delta);

	public: // properties
		Graphics& m_graphics;
		UI m_ui;
		Text m_text;
		Writer m_writer;
		Mouse m_mouse;
		Player m_me;
		Player m_enemy;
};

#endif