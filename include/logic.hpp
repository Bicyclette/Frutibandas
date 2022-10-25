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
#include "communication.hpp"

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
		FLYING_LEFT,
		FLYING_RIGHT,
		TURN_STONE,
		PETRIFIED
	};

	Fruit() : type('x'), state(STATE::STAND_STILL) {}
	Fruit(char t) : type(t), state(STATE::STAND_STILL) {}
	bool is_flying() {
		return state == STATE::FLYING_LEFT || state == STATE::FLYING_RIGHT;
	}
	bool is_turning_into_stone() {
		return state == STATE::TURN_STONE;
	}
	bool is_petrified() {
		return state == STATE::PETRIFIED;
	}

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
		hovered = false;
	}

	STATE state;
	glm::vec2 pos; // y component is lower left corner
	float animTimer;
	Fruit fruit;
	bool hovered;
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
		bool anvil;
		bool throw_anvil;
		glm::ivec2 anvil_coords;
		bool cow;
		bool cow_charge;
		glm::ivec2 cow_coords;
		bool petrify;
		glm::ivec2 petrify_coords;
		bool trap;
		glm::ivec2 trap_coords;
		bool activate_trap;
		bool confiscation;
		int confiscation_id;
		int confiscation_expiration;
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
			anvil = false;
			throw_anvil = false;
			anvil_coords = glm::ivec2(-1, -1);
			cow = false;
			cow_charge = false;
			cow_coords = glm::ivec2(-1, -1);
			petrify = false;
			petrify_coords = glm::ivec2(-1, -1);
			trap = false;
			trap_coords = glm::ivec2(-1, -1);
			activate_trap = false;
			confiscation = false;
			confiscation_id = -1;
			confiscation_expiration = 0;
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
			anvil = false;
			throw_anvil = false;
			anvil_coords = glm::ivec2(-1, -1);
			cow = false;
			cow_charge = false;
			cow_coords = glm::ivec2(-1, -1);
			petrify = false;
			petrify_coords = glm::ivec2(-1, -1);
			trap = false;
			trap_coords = glm::ivec2(-1, -1);
			activate_trap = false;
			confiscation = false;
			confiscation_id = -1;
			confiscation_expiration = 0;
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

	Logic()
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
		used_a_card = false;
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
	Texture empty_tex = createTexture("assets/game_page/empty.tga", TEXTURE_TYPE::DIFFUSE, true);

	// board animation
	Animation2D dying_tile;
	// banana animations
	std::array<Animation2D, 8> banana_anims; // right, left, up, down, p_right, p_left, p_up, p_down
	Animation2D banana_flying_right;
	Animation2D banana_flying_left;
	Animation2D banana_petrification;
	// orange animations
	std::array<Animation2D, 8> orange_anims; // right, left, up, down, p_right, p_left, p_up, p_down
	Animation2D orange_flying_right;
	Animation2D orange_flying_left;
	Animation2D orange_petrification;
	
	// draw stuff
	Sprite m_sprite;

	Board() :
		m_sprite(0, glm::vec2(0), glm::vec2(256), 1050, 728),
		dying_tile(c_animLength),
		banana_anims{ Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength) },
		banana_flying_right(c_animLength*0.7f),
		banana_flying_left(c_animLength * 0.7f),
		banana_petrification(c_animLength),
		orange_anims{ Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength), Animation2D(c_animLength) },
		orange_flying_right(c_animLength*0.7f),
		orange_flying_left(c_animLength*0.7f),
		orange_petrification(c_animLength)
	{
		dying_tile.load("assets/animation/board", 34);

		banana_anims[0].load("assets/animation/banana/mv_right", 22);
		banana_anims[1].load("assets/animation/banana/mv_left", 22);
		banana_anims[2].load("assets/animation/banana/mv_up", 22);
		banana_anims[3].load("assets/animation/banana/mv_down", 22);
		banana_anims[4].load("assets/animation/banana/pushed_right", 22);
		banana_anims[5].load("assets/animation/banana/pushed_left", 22);
		banana_anims[6].load("assets/animation/banana/pushed_up", 22);
		banana_anims[7].load("assets/animation/banana/pushed_down", 22);

		banana_flying_right.load("assets/animation/banana/flying_right", 15);
		banana_flying_left.load("assets/animation/banana/flying_left", 15);
		banana_petrification.load("assets/animation/banana/petrification", 17);
		
		orange_anims[0].load("assets/animation/orange/mv_right", 22);
		orange_anims[1].load("assets/animation/orange/mv_left", 22);
		orange_anims[2].load("assets/animation/orange/mv_up", 22);
		orange_anims[3].load("assets/animation/orange/mv_down", 22);
		orange_anims[4].load("assets/animation/orange/pushed_right", 22);
		orange_anims[5].load("assets/animation/orange/pushed_left", 22);
		orange_anims[6].load("assets/animation/orange/pushed_up", 22);
		orange_anims[7].load("assets/animation/orange/pushed_down", 22);

		orange_flying_right.load("assets/animation/orange/flying_right", 15);
		orange_flying_left.load("assets/animation/orange/flying_left", 15);
		orange_petrification.load("assets/animation/orange/petrification", 13);
	}

	void init(std::string board);
	std::string to_string();
	int get_banana_count();
	int get_orange_count();
	void draw(Logic& logic, std::vector<Advertiser>& advertiser, float delta, bool standby);
	GLuint get_banana_texture(Logic& logic, int col, int line, float timer);
	GLuint get_orange_texture(Logic& logic, int col, int line, float timer);
	void set_animTimer(Logic& logic);
	GLuint get_animationFrame(Logic& logic, int col, int line, float delta, bool stand_still, bool standby);
	GLuint get_tileFrame(Logic& logic, int col, int line, float delta);
	void set_pusher_index(glm::vec2 dir, char pusher_type, int index[], int& origin);
	bool is_pushed_x(int col, int line, char pusher, int dir, int origin);
	bool is_pushed_y(int col, int line, char pusher, int dir, int origin);
	void update(Logic& logic, std::vector<Advertiser>& advertiser);
	void check_dying_tiles(int& col, int& line); // fill col and line with the index of the col/line which must be deleted
	void set_tileDeleteTimerColumn(int c);
	void set_tileDeleteTimerLine(int l);
	void reset();
	void reset_hovered();
	std::vector<int> get_free_tiles();
	std::string get_reinforcement_position(std::vector<int>& list);
	glm::ivec2 get_tile_coords_from_mouse_position(int x, int y);
	glm::ivec2 get_tile_coords_from_cow_position(int charge_col, glm::vec2 pos);
	void check_activate_trap(Logic& logic);
	bool exist_petrified_fruit_line(int col, int line, bool backward) {
		if (backward) {
			for (int i = col; i >= bounds.left; --i) {
				if (tile[i][line].fruit.type == 'x') {
					return false;
				}
				else if (tile[i][line].fruit.is_petrified()) {
					return true;
				}
			}
		}
		else {
			for (int i = col; i <= bounds.right; ++i) {
				if (tile[i][line].fruit.type == 'x') {
					return false;
				}
				else if (tile[i][line].fruit.is_petrified()) {
					return true;
				}
			}
		}
		return false;
	}
	bool exist_petrified_fruit_column(int col, int line, bool backward) {
		if (backward) {
			for (int i = line; i >= bounds.top; --i) {
				if (tile[col][i].fruit.type == 'x') {
					return false;
				}
				else if (tile[col][i].fruit.is_petrified()) {
					return true;
				}
			}
		}
		else {
			for (int i = line; i <= bounds.bottom; ++i) {
				if (tile[col][i].fruit.type == 'x') {
					return false;
				}
				else if (tile[col][i].fruit.is_petrified()) {
					return true;
				}
			}
		}
		return false;
	}
};

struct Anvil
{
	Texture tex = createTexture("assets/game_page/anvil.tga", TEXTURE_TYPE::DIFFUSE, true);
	Animation2D smoke;
	bool smoke_on;
	// draw stuff
	glm::vec2 pos;
	glm::vec2 fall_speed;
	float timer;
	Sprite m_sprite;
	int frame;

	Anvil() :
		smoke(c_animLength),
		smoke_on(false),
		fall_speed(0.0f, -750.0f),
		timer(0.0f),
		m_sprite(0, glm::vec2(0), glm::vec2(256), 1050, 728)
	{
		smoke.load("assets/animation/anvil", 12);
	}

	void draw(glm::vec2 from, glm::vec2 to, float delta)
	{
		timer += delta;
		pos = from + fall_speed * timer;
		m_sprite.set_pos(pos);
		m_sprite.draw(tex.id);
		if (pos.y <= to.y) {
			timer = 0.0f;
			smoke_on = true;
		}
	}

	void draw_smoke(glm::vec2 target, float delta)
	{
		timer += delta;
		m_sprite.set_pos(target + glm::vec2(0.0f, 100.0f));
		float percent = timer / smoke.duration;
		if (percent <= 1.0f && percent >= 0.0f) {
			frame = static_cast<int>(percent * (smoke.frames.size() - 1));
			m_sprite.draw(smoke.frames[frame].id);
		}
		else {
			timer = 0.0f;
			smoke_on = false;
		}
	}
};

struct Cow
{
	struct Smoke
	{
		glm::vec2 position;
		float timer;
		Smoke() : position(0.0f, 0.0f), timer(0.0f) {}
		Smoke(glm::vec2 pos, float t) : position(pos), timer(t){}
	};

	Sprite m_sprite;
	Animation2D run;
	Animation2D smoke_trail;
	int col;
	std::vector<Smoke> smoke;

	// draw stuff
	glm::vec2 pos;
	glm::vec2 run_speed;
	float timer;
	int frame;
	float percent;

	Cow() :
		m_sprite(0, glm::vec2(0), glm::vec2(256), 1050, 728),
		run(c_animLength),
		smoke_trail(c_animLength),
		col(-1),
		pos(0.0f, 0.0f),
		run_speed(0.0f, -350.0f),
		timer(0.0f),
		frame(0),
		percent(0.0f)
	{
		run.load("assets/animation/cow", 17);
		smoke_trail.load("assets/animation/cow/trail", 21);
	}

	void create_smoke_data(Board& board)
	{
		for (int l = board.bounds.top; l <= board.bounds.bottom; ++l)
		{
			smoke.emplace_back(board.tile[col][l].pos, -0.13*(l - board.bounds.top)-0.35f);
		}
	}

	bool draw(Board& board, float pos_x, float delta)
	{
		// draw smoke trail
		size_t smoke_count = smoke.size();
		for (size_t i = 0; i < smoke_count; ++i) {
			m_sprite.set_pos(smoke[i].position);
			smoke[i].timer += delta;
			percent = smoke[i].timer / smoke_trail.duration;
			if (percent <= 1.0f && percent >= 0.0f) {
				frame = static_cast<int>(percent * (smoke_trail.frames.size() - 1));
				m_sprite.draw(smoke_trail.frames.at(frame).id);
			}
		}
		// draw cow
		timer += delta;
		pos = glm::vec2(pos_x, c_draw_start.y + board.bounds.top * c_draw_shift.y + 90.0f) + run_speed * timer;
		percent = std::fmod(timer, run.duration) / run.duration;
		frame = static_cast<int>(percent * (run.frames.size() - 1));
		m_sprite.set_pos(pos);
		m_sprite.draw(run.frames[frame].id);
		glm::ivec2 tile = board.get_tile_coords_from_cow_position(col, pos + glm::vec2(0.0f, -10.0f));
		if (tile != glm::ivec2(-1, -1)) {
			if (board.tile[tile.x][tile.y].fruit.type != 'x' && (tile.y - board.bounds.top) % 2 == 0) {
				board.tile[tile.x][tile.y].fruit.state = Fruit::STATE::FLYING_RIGHT;
			}
			else if(board.tile[tile.x][tile.y].fruit.type != 'x') {
				board.tile[tile.x][tile.y].fruit.state = Fruit::STATE::FLYING_LEFT;
			}
		}
		if (pos.y <= 100.0f) {
			timer = 0.0f;
			smoke.clear();
			return false;
		}
		return true;
	}
};

#endif
