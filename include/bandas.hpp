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

constexpr int c_screen_width{ 1050 };
constexpr int c_screen_height{ 728 };

constexpr int c_avatar_width{ 512 };
constexpr int c_avatar_height{ 512 };

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
		void swap_gender_features(Avatar::GENDER from, Avatar::GENDER to);
		void update_home_page(std::bitset<10> & user_input, char* txt_input, float delta);
		void hovering_home_page(Page& page, int id);
		void click_home_page(Page& page, int id);
		void update_avatar(Page& page, int id);
		void draw_home_page();
		void update_game_page(std::bitset<10>& user_input, char* txt_input, float delta);
		void draw_game_page();

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