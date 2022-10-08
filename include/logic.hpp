#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <cmath>
#include <random>
#include "shader_light.hpp"
#include "helpers.hpp"
#include "user_interface.hpp"

constexpr glm::vec2 c_tile_size(256);
constexpr glm::vec2 c_draw_start(243, 477);
constexpr glm::vec2 c_draw_shift(44, -45);
constexpr float c_animLength(0.4f);

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
		animTimer = 42.0f;
	}

	STATE state;
	glm::vec2 pos; // y component is lower left corner
	float animTimer;
	Fruit fruit;
};

struct Advertiser
{
	bool m_show;
	bool m_green;
	int m_index; // texture index
	float m_stand_still_timer;
	float m_moving_timer;
	float m_timer;
	bool erase;

	Advertiser() :
		m_show(false),
		m_green(true),
		m_index(-1),
		m_stand_still_timer(0.75f),
		m_moving_timer(0.35f),
		m_timer(0.0f),
		erase(false)
	{}

	glm::vec2 get_pos(float delta)
	{
		m_timer += delta;
		if (m_timer <= m_moving_timer) {
			float pos_x = 1100.0f * (m_timer / m_moving_timer);
			return glm::vec2(pos_x - 1150.0f, 339);
		}
		else if (m_timer > m_moving_timer && m_timer < (m_moving_timer + m_stand_still_timer)) {
			return glm::vec2(-50, 339);
		}
		else if (m_timer >= (m_moving_timer + m_stand_still_timer) && m_timer <= (m_moving_timer * 2.0f + m_stand_still_timer)) {
			float pos_x = 1100.0f * ((m_timer - (m_moving_timer + m_stand_still_timer)) / m_moving_timer);
			return glm::vec2(-50 + pos_x, 339);
		}
		else {
			//m_index = -1;
			m_show = false;
			m_timer = 0.0f;
			erase = true;
			return glm::vec2();
		}
	}
};

struct Logic
{
	struct MOVE
	{
		int dir; // -1 = still, 0 = right, 1 = left, 2 = up, 3 = down
		glm::vec2 dir_vec;
	};

	struct CardEffect
	{
		bool charge;
		bool second_wave;
		bool disorder;
		int disorder_destination;
		std::array<int, 6> reinforcement;
		bool select_enemy_banda;
		glm::ivec2 conversion_coords;
		bool select_ally_banda;
		glm::ivec2 solo_coords;
		CardEffect()
		{
			charge = false;
			second_wave = false;
			disorder = false;
			disorder_destination = -1;
			reinforcement = std::array<int, 6>{-1,-1,-1,-1,-1,-1};
			select_enemy_banda = false;
			conversion_coords = glm::ivec2(-1,-1);
			select_ally_banda = false;
			solo_coords = glm::ivec2(-1, -1);
		}
		void reset()
		{
			charge = false;
			second_wave = false;
			disorder = false;
			disorder_destination = -1;
			reinforcement = std::array<int, 6>{-1, -1, -1, -1, -1, -1};
			select_enemy_banda = false;
			conversion_coords = glm::ivec2(-1, -1);
			select_ally_banda = false;
			solo_coords = glm::ivec2(-1, -1);
		}
	};

	int turn; // 0 = orange, 1 = banane
	bool used_a_card;
	MOVE move;
	bool kill_tiles;
	int delete_line_id;
	int delete_column_id;
	bool change_turn;
	bool game_is_finished;
	CardEffect card_effect;

	Logic::Logic()
	{
		used_a_card = false;
		move.dir = -1;
		move.dir_vec = glm::vec2(0, 0);
		kill_tiles = false;
		delete_line_id = -1;
		delete_column_id = -1;
		change_turn = false;
		game_is_finished = false;
	}

	void reset()
	{
		move.dir = -1;
		move.dir_vec = glm::vec2(0, 0);
		kill_tiles = false;
		delete_line_id = -1;
		delete_column_id = -1;
		change_turn = false;
		game_is_finished = false;
		card_effect.reset();
	}
};

class RandomGenerator
{
public:
	RandomGenerator(int from, int to) :
		m_random_engine(m_random_device()),
		m_distribution(from, to)
	{}

	int gen()
	{
		return m_distribution(m_random_engine);
	}

public:
	std::random_device m_random_device;
	std::mt19937 m_random_engine;
	std::uniform_int_distribution<int> m_distribution;
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
	int banana_count;
	int orange_count;

	// static elements
	Texture banana_tex = createTexture("assets/game_page/banana.tga", TEXTURE_TYPE::DIFFUSE, true);
	Texture orange_tex = createTexture("assets/game_page/orange.tga", TEXTURE_TYPE::DIFFUSE, true);
	Texture tile_tex = createTexture("assets/game_page/board.tga", TEXTURE_TYPE::DIFFUSE, true);
	Texture tile_select_tex = createTexture("assets/animation/board/select/1.tga", TEXTURE_TYPE::DIFFUSE, true);

	// board animation
	Animation2D dying_tile;
	// banana animations
	std::array<Animation2D, 8> banana_anims; // right, left, up, down, p_right, p_left, p_up, p_down
	//Animation2D banana_death;
	// orange animations
	std::array<Animation2D, 8> orange_anims; // right, left, up, down, p_right, p_left, p_up, p_down
	std::array<Animation2D, 4> orange_death;
	
	// draw stuff
	Sprite m_sprite;

	Board() :
		m_sprite(0, glm::vec2(0), glm::vec2(256), 1050, 728),
		dying_tile(c_animLength),
		orange_death{ Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength) },
		banana_anims{ Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength) },
		orange_anims{ Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength) }
	{
		dying_tile.load("assets/animation/board", 34);

		orange_death[0].load("assets/animation/orange/death/right", 16);
		orange_death[1].load("assets/animation/orange/death/left", 16);
		orange_death[2].load("assets/animation/orange/death/up", 16);
		orange_death[3].load("assets/animation/orange/death/down", 16);

		banana_anims[0].load("assets/animation/banana/mv_right", 22);
		banana_anims[1].load("assets/animation/banana/mv_left", 22);
		banana_anims[2].load("assets/animation/banana/mv_up", 22);
		banana_anims[3].load("assets/animation/banana/mv_down", 22);
		banana_anims[4].load("assets/animation/banana/pushed_right", 22);
		banana_anims[5].load("assets/animation/banana/pushed_left", 22);
		banana_anims[6].load("assets/animation/banana/pushed_up", 22);
		banana_anims[7].load("assets/animation/banana/pushed_down", 22);
		
		orange_anims[0].load("assets/animation/orange/mv_right", 22);
		orange_anims[1].load("assets/animation/orange/mv_left", 22);
		orange_anims[2].load("assets/animation/orange/mv_up", 22);
		orange_anims[3].load("assets/animation/orange/mv_down", 22);
		orange_anims[4].load("assets/animation/orange/pushed_right", 22);
		orange_anims[5].load("assets/animation/orange/pushed_left", 22);
		orange_anims[6].load("assets/animation/orange/pushed_up", 22);
		orange_anims[7].load("assets/animation/orange/pushed_down", 22);
	}

	void init(std::string board);
	std::string to_string();
	int get_banana_count();
	int get_orange_count();
	void draw(Logic& logic, float delta, bool standby);
	GLuint get_banana_texture(Logic& logic, int col, int line, float timer);
	GLuint get_orange_texture(Logic& logic, int col, int line, float timer);
	void set_animTimer(Logic& logic);
	GLuint get_animationFrame(Logic& logic, int col, int line, float delta, bool stand_still, bool standby);
	GLuint get_tileFrame(Logic& logic, int col, int line, float delta);
	void set_pusher_index(glm::vec2 dir, char pusher_type, int index[], int& origin);
	bool is_pushed_x(int col, int line, char pusher, int dir, int origin);
	bool is_pushed_y(int col, int line, char pusher, int dir, int origin);
	void update(Logic& logic);
	void check_dying_tiles(int& col, int& line); // fill col and line with the index of the col/line which must be deleted
	void set_tileDeleteTimerColumn(int c);
	void set_tileDeleteTimerLine(int l);
	void reset();
	std::vector<int> get_free_tiles();
	std::string get_reinforcement_position(std::vector<int>& list);
	glm::ivec2 get_tile_coords_from_mouse_position(int x, int y);
};

#endif