#include "logic.hpp"

std::string Board::to_string()
{
	std::string data;
	for (int line = bounds.top; line <= bounds.bottom; ++line)
	{
		for (int col = bounds.left; col <= bounds.right; ++col)
		{
			if (tile[col][line].fruit.type != 'x')
			{
				if (tile[col][line].fruit.type == 'b') {
					if (col == bounds.right) {
						data += "b";
					}
					else {
						data += "b.";
					}
				}
				else {
					if (col == bounds.right) {
						data += "o";
					}
					else {
						data += "o.";
					}
				}
			}
			else
			{
				if (col == bounds.right) {
					data += "x";
				}
				else {
					data += "x.";
				}
			}
		}
		if (line < bounds.bottom) {
			data += "\n";
		}
	}
	return data;
}

void Board::init(std::string board)
{
	int line = 0;
	int col = 0;

	// board boundaries
	bounds.top = 0;
	bounds.bottom = 7;
	bounds.left = 0;
	bounds.right = 7;

	// init tiles fruit
	for (char c : board)
	{
		if (c == 'b')
		{
			tile[col][line].fruit.type = 'b';
			tile[col][line].fruit.animTimer = 0.0f;
			col += 1;
		}
		else if (c == 'o')
		{
			tile[col][line].fruit.type = 'o';
			tile[col][line].fruit.animTimer = 0.0f;
			col += 1;
		}
		else if (c == '\n')
		{
			line += 1;
			col = 0;
		}
	}

	// init tiles' position
	for (line = 0; line < 8; ++line)
	{
		for (col = 0; col < 8; ++col)
		{
			tile[col][line].pos = glm::vec2(c_draw_start.x + c_draw_shift.x * col, c_draw_start.y + c_draw_shift.y * line);
		}
	}
}

void Board::draw(Logic& logic, float delta)
{
	for (int line = bounds.top; line <= bounds.bottom; ++line)
	{
		for (int col = bounds.left; col <= bounds.right; ++col)
		{
			if (tile[col][line].state != Tile::STATE::DEAD)
			{
				m_sprite.set_pos(tile[col][line].pos);
				m_sprite.draw(tile_tex.id);
			}
		}
	}

	for (int line = bounds.top; line <= bounds.bottom; ++line)
	{
		for (int col = bounds.left; col <= bounds.right; ++col)
		{
			if (tile[col][line].state != Tile::STATE::DEAD)
			{
				if (tile[col][line].fruit.type != 'x')
				{
					// fruit position
					bool stand_still = tile[col][line].fruit.state == Fruit::STATE::STAND_STILL;
					GLuint frame = get_animationFrame(logic, col, line, delta, stand_still);
					m_sprite.set_pos(tile[col][line].pos);
					m_sprite.draw(frame);
				}
			}
		}
	}

	update(logic);
}

GLuint Board::get_animationFrame(Logic& logic, int col, int line, float delta, bool stand_still)
{
	if (tile[col][line].fruit.type == 'b')
	{
		if (logic.move.dir == -1 || stand_still)
		{
			return banana_tex.id;
		}
		else
		{
			tile[col][line].fruit.animTimer += delta;
			return get_banana_texture(logic, col, line, tile[col][line].fruit.animTimer);
		}
	}
	else
	{
		if (logic.move.dir == -1 || stand_still)
		{
			return orange_tex.id;
		}
		else
		{
			tile[col][line].fruit.animTimer += delta;
			return get_orange_texture(logic, col, line, tile[col][line].fruit.animTimer);
		}
	}
}

GLuint Board::get_banana_texture(Logic& logic, int col, int line, float delta)
{
	int frame = 0;
	Animation2D& anim = banana_anims[logic.move.dir];
	float percent = delta / anim.duration;
	if (percent < 0.0f) {
		return banana_tex.id;
	}
	else if (percent <= 1.0f && percent >= 0.0f) {
		frame = static_cast<int>(percent * (anim.frames.size()-1));
		return anim.frames[frame].id;
	}
	else if (percent > 1.0f) {
		frame = anim.frames.size() - 1;
		return anim.frames[frame].id;
	}
}

GLuint Board::get_orange_texture(Logic& logic, int col, int line, float delta)
{
	int frame = 0;
	Animation2D& anim = orange_anims[logic.move.dir];
	float percent = delta / anim.duration;
	if (percent < 0.0f) {
		return orange_tex.id;
	}
	else if (percent <= 1.0f && percent >= 0.0f) {
		frame = static_cast<int>(percent * (anim.frames.size() - 1));
		return anim.frames[frame].id;
	}
	else if (percent > 1.0f) {
		frame = anim.frames.size() - 1;
		return anim.frames[frame].id;
	}
}

void Board::set_animTimer(Logic& logic)
{
	char fruit_pusher = (logic.turn == 0) ? 'o' : 'b';
	glm::vec2 move_direction = logic.move.dir_vec;
	int line;
	int col;
	int first_pusher[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
	int origin = 42;
	float max_delay = 0.0f;
	set_pusher_index(move_direction, fruit_pusher, first_pusher, origin);
	std::cout << "turn = " << (logic.turn == 0 ? " orange\n" : " banane\n");
	std::cout << "origin = " << origin << std::endl;

	int dir_x = static_cast<int>(move_direction.x);
	int dir_y = static_cast<int>(move_direction.y);
	bool backward = (dir_x + dir_y) < 0;

	if (dir_x != 0)
	{
		for (int i = bounds.top; i <= bounds.bottom; ++i)
		{
			for (int j = bounds.left; j <= bounds.right; ++j)
			{
				col = (backward) ? -j + 7 : j;
				line = i;
				if (tile[col][line].state != Tile::STATE::DEAD && tile[col][line].fruit.type != 'x')
				{
					if (is_pushed_x(col, line, fruit_pusher, dir_x, origin))
					{
						tile[col][line].fruit.animTimer = -c_animLength * abs(col - origin) + ((c_animLength * 0.625f) * abs(col - origin));
						tile[col][line].fruit.state = Fruit::STATE::MOVING;
						max_delay = (tile[col][line].fruit.animTimer < max_delay) ? tile[col][line].fruit.animTimer : max_delay;
					}
					else
					{
						tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						tile[col][line].fruit.animTimer = 42.0f;
					}
				}
			}
		}
	}
	else
	{
		for (int i = bounds.left; i <= bounds.right; ++i)
		{
			for (int j = bounds.top; j <= bounds.bottom; ++j)
			{
				line = (backward) ? j : -j + 7;
				col = i;
				if (tile[col][line].state != Tile::STATE::DEAD && tile[col][line].fruit.type != 'x')
				{
					if (is_pushed_y(col, line, fruit_pusher, dir_y, origin))
					{
						tile[col][line].fruit.animTimer = -c_animLength * abs(line - origin) + ((c_animLength * 0.625f) * abs(line - origin));
						tile[col][line].fruit.state = Fruit::STATE::MOVING;
						max_delay = (tile[col][line].fruit.animTimer < max_delay) ? tile[col][line].fruit.animTimer : max_delay;
					}
					else
					{
						tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
					}
				}
			}
		}
	}

	// print timer
	/*
	for (int i = bounds.top; i <= bounds.bottom; ++i)
	{
		for (int j = bounds.left; j <= bounds.right; ++j)
		{
			if (tile[j][i].state != Tile::STATE::DEAD && tile[j][i].fruit.type != 'x')
			{
				std::cout << tile[j][i].fruit.animTimer << " | ";
			}
		}
		std::cout << "\n";
	}
	*/
}

void Board::set_pusher_index(glm::vec2 dir, char pusher_type, int index[], int& origin)
{
	int col = 0;
	int line = 0;
	int dir_x = static_cast<int>(dir.x);
	int dir_y = static_cast<int>(dir.y);
	bool backward = (dir_x + dir_y) < 0;

	if (dir_x != 0)
	{
		for (int i = bounds.top; i <= bounds.bottom; ++i)
		{
			for (int j = bounds.left; j <= bounds.right; ++j)
			{
				col = (backward) ? -j + 7 : j;
				line = i;
				// set origin
				origin = (origin == 42) ? col : (backward) ? ((col >= origin) ? col : origin) : ((col <= origin) ? col : origin);
				if (tile[col][line].state != Tile::STATE::DEAD && tile[col][line].fruit.type != 'x')
				{
					if (tile[col][line].fruit.type == pusher_type) {
						index[i] = col;
						break;
					}
				}
			}
		}
	}
	else
	{
		for (int i = bounds.left; i <= bounds.right; ++i)
		{
			for (int j = bounds.top; j <= bounds.bottom; ++j)
			{
				line = (backward) ? j : -j + 7;
				col = i;
				// set origin
				origin = (origin == 42) ? line : (backward) ? ((line <= origin) ? line : origin) : ((line >= origin) ? line : origin);
				if (tile[col][line].state != Tile::STATE::DEAD && tile[col][line].fruit.type != 'x')
				{
					if (tile[col][line].fruit.type == pusher_type) {
						index[i] = line;
						break;
					}
				}
			}
		}
	}
}

bool Board::is_pushed_x(int col, int line, char pusher, int dir, int origin)
{
	do
	{
		if (tile[col][line].fruit.type == pusher) {
			return true;
		}
		col -= dir;
	} while (col >= bounds.left && col <= bounds.right && line >= bounds.top && line <= bounds.bottom && tile[col][line].fruit.type != 'x');
	return false;
}

bool Board::is_pushed_y(int col, int line, char pusher, int dir, int origin)
{
	do
	{
		if (tile[col][line].fruit.type == pusher) {
			return true;
		}
		line += dir;
	} while (col >= bounds.left && col <= bounds.right && line >= bounds.top && line <= bounds.bottom && tile[col][line].fruit.type != 'x');
	return false;
}

void Board::update(Logic& logic)
{
	bool end_movement{ true };
	for (int line = bounds.top; line <= bounds.bottom; ++line)
	{
		for (int col = bounds.left; col <= bounds.right; ++col)
		{
			if (tile[col][line].fruit.type != 'x')
			{
				if (tile[col][line].fruit.type == 'b')
				{
					if (tile[col][line].fruit.animTimer <= banana_anims[logic.move.dir].duration)
					{
						end_movement = false;
						break;
					}
				}
				else
				{
					if (tile[col][line].fruit.animTimer <= orange_anims[logic.move.dir].duration)
					{
						end_movement = false;
						break;
					}
				}
			}
		}
	}

	if (end_movement && logic.move.dir != -1)
	{
		if (logic.move.dir == 0)
		{
			for (int line = bounds.top; line <= bounds.bottom; ++line)
			{
				for (int col = bounds.right; col >= bounds.left; --col)
				{
					// if tile contains a non moving fruit, pass
					if (tile[col][line].fruit.type != 'x' && tile[col][line].fruit.state == Fruit::STATE::STAND_STILL)
					{
						tile[col][line].fruit.animTimer = 0.0f;
						continue;
					}
					// else if tile contains a moving fruit
					else if (tile[col][line].fruit.type != 'x' && tile[col][line].fruit.state == Fruit::STATE::MOVING)
					{
						// if left tile exists & left tile contains a fruit & that fruit is moving
						if (col - 1 >= bounds.left && tile[col-1][line].fruit.type != 'x' && tile[col-1][line].fruit.state == Fruit::STATE::MOVING) {
							tile[col][line].fruit.type = tile[col - 1][line].fruit.type;
							tile[col][line].fruit.animTimer = 0.0f;
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
						// else if left tile exists & left tile contains a fruit & that fruit is not moving
						else if (col - 1 >= bounds.left && tile[col - 1][line].fruit.type != 'x' && tile[col - 1][line].fruit.state == Fruit::STATE::STAND_STILL) {
							tile[col][line].fruit.type = 'x';
							tile[col][line].fruit.animTimer = 0.0f;
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
						// else if left tile exists & left tile does not contain a fruit
						else if (col - 1 >= bounds.left && tile[col - 1][line].fruit.type == 'x') {
							tile[col][line].fruit.type = 'x';
							tile[col][line].fruit.animTimer = 0.0f;
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
						// else if left tile does not exists
						else if (col - 1 < bounds.left) {
							tile[col][line].fruit.type = 'x';
							tile[col][line].fruit.animTimer = 0.0f;
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
					}
					// else if tile does not contain any fruit & left tile contains a moving fruit
					else if (tile[col][line].fruit.type == 'x' && col - 1 >= bounds.left && tile[col - 1][line].fruit.type != 'x' && tile[col - 1][line].fruit.state == Fruit::STATE::MOVING) {
						tile[col][line].fruit.type = tile[col - 1][line].fruit.type;
						tile[col][line].fruit.animTimer = 0.0f;
						tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
					}
				}
			}
		}
		else if (logic.move.dir == 1)
		{
			for (int line = bounds.top; line <= bounds.bottom; ++line)
			{
				for (int col = bounds.left; col <= bounds.right; ++col)
				{
					// if tile contains a non moving fruit, pass
					if (tile[col][line].fruit.type != 'x' && tile[col][line].fruit.state == Fruit::STATE::STAND_STILL)
					{
						tile[col][line].fruit.animTimer = 0.0f;
						continue;
					}
					// else if tile contains a moving fruit
					else if (tile[col][line].fruit.type != 'x' && tile[col][line].fruit.state == Fruit::STATE::MOVING)
					{
						// if right tile exists & right tile contains a fruit & that fruit is moving
						if (col + 1 <= bounds.right && tile[col + 1][line].fruit.type != 'x' && tile[col + 1][line].fruit.state == Fruit::STATE::MOVING) {
							tile[col][line].fruit.type = tile[col + 1][line].fruit.type;
							tile[col][line].fruit.animTimer = 0.0f;
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
						// else if right tile exists & right tile contains a fruit & that fruit is not moving
						else if (col + 1 <= bounds.right && tile[col + 1][line].fruit.type != 'x' && tile[col + 1][line].fruit.state == Fruit::STATE::STAND_STILL) {
							tile[col][line].fruit.type = 'x';
							tile[col][line].fruit.animTimer = 0.0f;
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
						// else if right tile exists & right tile does not contain a fruit
						else if (col + 1 <= bounds.right && tile[col + 1][line].fruit.type == 'x') {
							tile[col][line].fruit.type = 'x';
							tile[col][line].fruit.animTimer = 0.0f;
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
						// else if right tile does not exists
						else if (col + 1 > bounds.right) {
							tile[col][line].fruit.type = 'x';
							tile[col][line].fruit.animTimer = 0.0f;
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
					}
					// else if tile does not contain any fruit & right tile contains a moving fruit
					else if (tile[col][line].fruit.type == 'x' && col + 1 <= bounds.right && tile[col + 1][line].fruit.type != 'x' && tile[col + 1][line].fruit.state == Fruit::STATE::MOVING) {
						tile[col][line].fruit.type = tile[col + 1][line].fruit.type;
						tile[col][line].fruit.animTimer = 0.0f;
						tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
					}
				}
			}
		}
		else if (logic.move.dir == 2)
		{
			for (int col = bounds.left; col <= bounds.right; ++col)
			{
				for (int line = bounds.top; line <= bounds.bottom; ++line)
				{
					// if tile contains a non moving fruit, pass
					if (tile[col][line].fruit.type != 'x' && tile[col][line].fruit.state == Fruit::STATE::STAND_STILL)
					{
						tile[col][line].fruit.animTimer = 0.0f;
						continue;
					}
					// else if tile contains a moving fruit
					else if (tile[col][line].fruit.type != 'x' && tile[col][line].fruit.state == Fruit::STATE::MOVING)
					{
						// if down tile exists & down tile contains a fruit & that fruit is moving
						if (line + 1 <= bounds.bottom && tile[col][line + 1].fruit.type != 'x' && tile[col][line + 1].fruit.state == Fruit::STATE::MOVING) {
							tile[col][line].fruit.type = tile[col][line + 1].fruit.type;
							tile[col][line].fruit.animTimer = 0.0f;
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
						// else if down tile exists & down tile contains a fruit & that fruit is not moving
						else if (line + 1 <= bounds.bottom && tile[col][line + 1].fruit.type != 'x' && tile[col][line + 1].fruit.state == Fruit::STATE::STAND_STILL) {
							tile[col][line].fruit.type = 'x';
							tile[col][line].fruit.animTimer = 0.0f;
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
						// else if down tile exists & down tile does not contain a fruit
						else if (line + 1 <= bounds.bottom && tile[col][line + 1].fruit.type == 'x') {
							tile[col][line].fruit.type = 'x';
							tile[col][line].fruit.animTimer = 0.0f;
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
						// else if down tile does not exists
						else if (line + 1 > bounds.bottom) {
							tile[col][line].fruit.type = 'x';
							tile[col][line].fruit.animTimer = 0.0f;
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
					}
					// else if tile does not contain any fruit & down tile contains a moving fruit
					else if (tile[col][line].fruit.type == 'x' && line + 1 <= bounds.bottom && tile[col][line + 1].fruit.type != 'x' && tile[col][line + 1].fruit.state == Fruit::STATE::MOVING) {
						tile[col][line].fruit.type = tile[col][line + 1].fruit.type;
						tile[col][line].fruit.animTimer = 0.0f;
						tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
					}
				}
			}
		}
		else if (logic.move.dir == 3)
		{
			for (int col = bounds.left; col <= bounds.right; ++col)
			{
				for (int line = bounds.bottom; line >= bounds.top; --line)
				{
					// if tile contains a non moving fruit, pass
					if (tile[col][line].fruit.type != 'x' && tile[col][line].fruit.state == Fruit::STATE::STAND_STILL)
					{
						tile[col][line].fruit.animTimer = 0.0f;
						continue;
					}
					// else if tile contains a moving fruit
					else if (tile[col][line].fruit.type != 'x' && tile[col][line].fruit.state == Fruit::STATE::MOVING)
					{
						// if up tile exists & up tile contains a fruit & that fruit is moving
						if (line - 1 >= bounds.top && tile[col][line - 1].fruit.type != 'x' && tile[col][line - 1].fruit.state == Fruit::STATE::MOVING) {
							tile[col][line].fruit.type = tile[col][line - 1].fruit.type;
							tile[col][line].fruit.animTimer = 0.0f;
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
						// else if up tile exists & up tile contains a fruit & that fruit is not moving
						else if (line - 1 >= bounds.top && tile[col][line - 1].fruit.type != 'x' && tile[col][line - 1].fruit.state == Fruit::STATE::STAND_STILL) {
							tile[col][line].fruit.type = 'x';
							tile[col][line].fruit.type = 'x';
							tile[col][line].fruit.animTimer = 0.0f;
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
						// else if up tile exists & up tile does not contain a fruit
						else if (line - 1 >= bounds.top && tile[col][line - 1].fruit.type == 'x') {
							tile[col][line].fruit.type = 'x';
							tile[col][line].fruit.animTimer = 0.0f;
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
						// else if up tile does not exists
						else if (line - 1 < bounds.top) {
							tile[col][line].fruit.type = 'x';
							tile[col][line].fruit.animTimer = 0.0f;
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
					}
					// else if tile does not contain any fruit & up tile contains a moving fruit
					else if (tile[col][line].fruit.type == 'x' && line - 1 >= bounds.top && tile[col][line - 1].fruit.type != 'x' && tile[col][line - 1].fruit.state == Fruit::STATE::MOVING) {
						tile[col][line].fruit.type = tile[col][line - 1].fruit.type;
						tile[col][line].fruit.animTimer = 0.0f;
						tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
					}
				}
			}
		}

		logic.move.dir = -1;
		logic.move.dir_vec = glm::vec2(0);
		logic.move.max_animTimer = 0.0f;
	}
}
