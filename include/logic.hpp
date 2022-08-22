#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include "shader_light.hpp"
#include "helpers.hpp"
#include "user_interface.hpp"
#include <cmath>

constexpr glm::vec2 c_tile_size(256);
constexpr glm::vec2 c_draw_start(243, 477);
constexpr glm::vec2 c_draw_shift(44, -45);
constexpr float c_animLength(0.375f);

struct Animation2D
{
	std::vector<Texture> frames;
	float duration;

	Animation2D(float d) : duration(d)
	{}

	void load(std::string path, int count)
	{
		for (int i = 0; i < count; ++i)
		{
			std::string img_path = path + "/" + std::to_string(i + 1) + ".tga";
			Texture tex = createTexture(img_path, TEXTURE_TYPE::DIFFUSE, true);
			frames.push_back(tex);
		}
	}
};

struct Fruit
{
	enum STATE
	{
		STAND_STILL,
		MOVING,
		PETRIFIED
	};

	Fruit() : type('x'), state(STATE::STAND_STILL) {}
	Fruit(char t) : type(t), state(STATE::STAND_STILL) {}

	char type;
	STATE state;
	float animTimer;
};

struct Tile
{
	enum STATE
	{
		ALIVE,
		DEAD,
		DYING,
		TRAPPED
	};

	Tile() : state(STATE::ALIVE)
	{
		pos = glm::vec2(0, 0);
	}

	STATE state;
	glm::vec2 pos; // y component is lower left corner
	Fruit fruit;
};

struct Logic
{
	struct MOVE
	{
		int dir; // -1 = still, 0 = right, 1 = left, 2 = up, 3 = down
		glm::vec2 dir_vec;
		float max_animTimer;
	};

	int turn; // 0 = orange, 1 = banane
	MOVE move;

	Logic::Logic()
	{
		move.dir = -1;
		move.dir_vec = glm::vec2(0, 0);
	}
};

struct Board
{
	struct AABB
	{
		int left;
		int right;
		int bottom;
		int top;
	};

	Tile tile[8][8];
	AABB bounds;

	// static elements
	Texture banana_tex = createTexture("assets/game_page/banana.tga", TEXTURE_TYPE::DIFFUSE, true);
	Texture orange_tex = createTexture("assets/game_page/orange.tga", TEXTURE_TYPE::DIFFUSE, true);
	Texture tile_tex = createTexture("assets/game_page/board.tga", TEXTURE_TYPE::DIFFUSE, true);

	// board animation
	
	// banana animations
	std::array<Animation2D, 4> banana_anims; // right, left, up, down
	// orange animations
	std::array<Animation2D, 4> orange_anims; // right, left, up, down
	// draw stuff
	Sprite m_sprite;


	Board() :
		m_sprite(0, glm::vec2(0), glm::vec2(256), 1050, 728),
		banana_anims{ Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength) },
		orange_anims{ Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength) }
	{
		banana_anims[0].load("assets/animation/banana/mv_right", 22);
		banana_anims[1].load("assets/animation/banana/mv_left", 22);
		banana_anims[3].load("assets/animation/banana/mv_down", 22);
		banana_anims[2].load("assets/animation/banana/mv_up", 22);
		orange_anims[0].load("assets/animation/orange/mv_right", 22);
		orange_anims[1].load("assets/animation/orange/mv_left", 22);
		orange_anims[3].load("assets/animation/orange/mv_down", 22);
		orange_anims[2].load("assets/animation/orange/mv_up", 22);
	}

	void init(std::string board);
	std::string to_string();
	void draw(Logic& logic, float delta);
	GLuint get_banana_texture(Logic& logic, int col, int line, float delta);
	GLuint get_orange_texture(Logic& logic, int col, int line, float delta);
	void set_animTimer(Logic& logic);
	GLuint get_animationFrame(Logic& logic, int col, int line, float delta, bool stand_still);
	void set_pusher_index(glm::vec2 dir, char pusher_type, int index[], int& origin);
	bool is_pushed_x(int col, int line, char pusher, int dir, int origin);
	bool is_pushed_y(int col, int line, char pusher, int dir, int origin);
	void update(Logic& logic);
};

#endif