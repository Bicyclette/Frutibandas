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

	// init fruits' amount
	banana_count = get_banana_count();
	orange_count = get_orange_count();
}

int Board::get_banana_count()
{
	int count{ 0 };
	for (int i = bounds.top; i <= bounds.bottom; ++i)
	{
		for (int j = bounds.left; j <= bounds.right; ++j)
		{
			if (tile[j][i].state == Tile::STATE::ALIVE && tile[j][i].fruit.type == 'b' && !tile[j][i].fruit.is_petrified()) {
				count++;
			}
		}
	}
	return count;
}

int Board::get_orange_count()
{
	int count{ 0 };
	for (int i = bounds.top; i <= bounds.bottom; ++i)
	{
		for (int j = bounds.left; j <= bounds.right; ++j)
		{
			if (tile[j][i].state == Tile::STATE::ALIVE && tile[j][i].fruit.type == 'o' && !tile[j][i].fruit.is_petrified()) {
				count++;
			}
		}
	}
	return count;
}

void Board::draw(Logic& logic, float delta, bool standby)
{
	for (int line = bounds.top; line <= bounds.bottom; ++line)
	{
		for (int col = bounds.left; col <= bounds.right; ++col)
		{
			if (tile[col][line].state != Tile::STATE::DEAD)
			{
				m_sprite.set_pos(tile[col][line].pos);
				GLuint frame = get_tileFrame(logic, col, line, delta);
				m_sprite.draw(frame);
			}
		}
	}

	for (int line = bounds.top; line <= bounds.bottom; ++line)
	{
		for (int col = bounds.left; col <= bounds.right; ++col)
		{
			if (tile[col][line].state != Tile::STATE::DEAD)
			{
				if (tile[col][line].fruit.type != 'x' && !tile[col][line].fruit.is_flying())
				{
					// fruit position
					bool stand_still = tile[col][line].fruit.state == Fruit::STATE::STAND_STILL;
					GLuint frame = get_animationFrame(logic, col, line, delta, stand_still, standby);
					m_sprite.set_pos(tile[col][line].pos);
					m_sprite.draw(frame);
				}
			}
		}
	}
	for (int line = bounds.top; line <= bounds.bottom; ++line)
	{
		for (int col = bounds.left; col <= bounds.right; ++col)
		{
			if (tile[col][line].state != Tile::STATE::DEAD)
			{
				if (tile[col][line].fruit.type != 'x' && tile[col][line].fruit.is_flying())
				{
					// fruit position
					bool stand_still = tile[col][line].fruit.state == Fruit::STATE::STAND_STILL;
					GLuint frame = get_animationFrame(logic, col, line, delta, stand_still, standby);
					m_sprite.set_pos(tile[col][line].pos);
					m_sprite.draw(frame);
				}
			}
		}
	}

	update(logic);
	orange_count = get_orange_count();
	banana_count = get_banana_count();
}

GLuint Board::get_tileFrame(Logic& logic, int col, int line, float delta)
{
	if (tile[col][line].state == Tile::STATE::ALIVE)
	{
		if (tile[col][line].hovered) {
			return tile_select_tex.id;
		}
		else {
			return tile_tex.id;
		}
	}
	else if (tile[col][line].state == Tile::STATE::DYING)
	{
		int frame = 0;
		tile[col][line].animTimer += delta;
		float percent = tile[col][line].animTimer / dying_tile.duration;
		if (percent < 0.0f) {
			return tile_tex.id;
		}
		else if (percent <= 1.0f && percent >= 0.0f) {
			frame = static_cast<int>(percent * (dying_tile.frames.size() - 1));
			return dying_tile.frames[frame].id;
		}
		else if (percent > 1.0f) {
			frame = dying_tile.frames.size() - 1;
			return dying_tile.frames[frame].id;
		}
	}
	else if (tile[col][line].state == Tile::STATE::TRAPPED)
	{
		return tile_select_tex.id;
	}
	else
	{
		return empty_tex.id;
	}
}

GLuint Board::get_animationFrame(Logic& logic, int col, int line, float delta, bool stand_still, bool standby)
{
	if (tile[col][line].fruit.type == 'b')
	{
		if ((logic.move.dir == -1 || stand_still || standby) && !tile[col][line].fruit.is_flying() && !tile[col][line].fruit.is_turning_into_stone() && !tile[col][line].fruit.is_petrified())
		{
			return banana_tex.id;
		}
		else if (tile[col][line].fruit.is_petrified())
		{
			size_t last_index = banana_petrification.frames.size() - 1;
			return banana_petrification.frames[last_index].id;
		}
		else
		{
			tile[col][line].fruit.animTimer += delta;
			return get_banana_texture(logic, col, line, tile[col][line].fruit.animTimer);
		}
	}
	else
	{
		if ((logic.move.dir == -1 || stand_still || standby) && !tile[col][line].fruit.is_flying() && !tile[col][line].fruit.is_turning_into_stone() && !tile[col][line].fruit.is_petrified())
		{
			return orange_tex.id;
		}
		else if (tile[col][line].fruit.is_petrified())
		{
			size_t last_index = orange_petrification.frames.size() - 1;
			return orange_petrification.frames[last_index].id;
		}
		else
		{
			tile[col][line].fruit.animTimer += delta;
			return get_orange_texture(logic, col, line, tile[col][line].fruit.animTimer);
		}
	}
}

GLuint Board::get_banana_texture(Logic& logic, int col, int line, float timer)
{
	int frame = 0;
	Animation2D* anim;
	if (tile[col][line].fruit.state == Fruit::STATE::FLYING_LEFT) {
		anim = &banana_flying_left;
	}
	else if (tile[col][line].fruit.state == Fruit::STATE::FLYING_RIGHT) {
		anim = &banana_flying_right;
	}
	else if (tile[col][line].fruit.is_turning_into_stone()) {
		anim = &banana_petrification;
	}
	else if(logic.turn == 1) {
		anim = &banana_anims[logic.move.dir];
	}
	else {
		anim = &banana_anims[logic.move.dir + 4];
	}
	float percent = timer / anim->duration;
	if (percent < 0.0f) {
		return banana_tex.id;
	}
	else if (percent <= 1.0f && percent >= 0.0f) {
		frame = static_cast<int>(percent * (anim->frames.size()-1));
		return anim->frames[frame].id;
	}
	else {
		if (logic.move.dir == 0 && col + 1 <= bounds.right && tile[col+1][line].state == Tile::STATE::DEAD) // right
		{
			return empty_tex.id;
		}
		else if (logic.move.dir == 1 && col - 1 >= bounds.left && tile[col - 1][line].state == Tile::STATE::DEAD) // left
		{
			return empty_tex.id;
		}
		else if (logic.move.dir == 2 && line - 1 >= bounds.top && tile[col][line - 1].state == Tile::STATE::DEAD) // up
		{
			return empty_tex.id;
		}
		else if (logic.move.dir == 3 && line + 1 <= bounds.bottom && tile[col][line + 1].state == Tile::STATE::DEAD) // down
		{
			return empty_tex.id;
		}
		frame = anim->frames.size() - 1;
		if (tile[col][line].fruit.state == Fruit::STATE::FLYING_LEFT) {
			tile[col][line].fruit.type = 'x';
			tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
		}
		else if (tile[col][line].fruit.state == Fruit::STATE::FLYING_RIGHT) {
			tile[col][line].fruit.type = 'x';
			tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
		}
		else if (tile[col][line].fruit.is_turning_into_stone()) {
			tile[col][line].fruit.state = Fruit::STATE::PETRIFIED;
		}
		return anim->frames[frame].id;
	}
}

GLuint Board::get_orange_texture(Logic& logic, int col, int line, float timer)
{
	int frame = 0;
	Animation2D* anim;
	if (tile[col][line].fruit.state == Fruit::STATE::FLYING_LEFT) {
		anim = &orange_flying_left;
	}
	else if (tile[col][line].fruit.state == Fruit::STATE::FLYING_RIGHT) {
		anim = &orange_flying_right;
	}
	else if (tile[col][line].fruit.is_turning_into_stone()) {
		anim = &orange_petrification;
	}
	else if (logic.turn == 0) {
		anim = &orange_anims[logic.move.dir];
	}
	else {
		anim = &orange_anims[logic.move.dir + 4];
	}
	float percent = timer / anim->duration;
	if (percent < 0.0f) {
		return orange_tex.id;
	}
	else if (percent <= 1.0f && percent >= 0.0f) {
		frame = static_cast<int>(percent * (anim->frames.size() - 1));
		return anim->frames[frame].id;
	}
	else {
		if (logic.move.dir == 0 && col + 1 <= bounds.right && tile[col + 1][line].state == Tile::STATE::DEAD) // right
		{
			return empty_tex.id;
		}
		else if (logic.move.dir == 1 && col - 1 >= bounds.left && tile[col - 1][line].state == Tile::STATE::DEAD) // left
		{
			return empty_tex.id;
		}
		else if (logic.move.dir == 2 && line - 1 >= bounds.top && tile[col][line - 1].state == Tile::STATE::DEAD) // up
		{
			return empty_tex.id;
		}
		else if (logic.move.dir == 3 && line + 1 <= bounds.bottom && tile[col][line + 1].state == Tile::STATE::DEAD) // down
		{
			return empty_tex.id;
		}
		frame = anim->frames.size() - 1;
		if (tile[col][line].fruit.state == Fruit::STATE::FLYING_LEFT) {
			tile[col][line].fruit.type = 'x';
			tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
		}
		else if (tile[col][line].fruit.state == Fruit::STATE::FLYING_RIGHT) {
			tile[col][line].fruit.type = 'x';
			tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
		}
		else if (tile[col][line].fruit.is_turning_into_stone()) {
			tile[col][line].fruit.state = Fruit::STATE::PETRIFIED;
		}
		return anim->frames[frame].id;
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
	set_pusher_index(move_direction, fruit_pusher, first_pusher, origin);

	int dir_x = static_cast<int>(move_direction.x);
	int dir_y = static_cast<int>(move_direction.y);
	bool backward = (dir_x + dir_y) < 0;

	if (dir_x != 0)
	{
		if (logic.card_effect.solo_coords.x == -1 && logic.card_effect.solo_coords.y == -1)
		{
			for (int i = bounds.top; i <= bounds.bottom; ++i)
			{
				for (int j = 0; j <= 7; ++j)
				{
					col = (backward) ? -j + 7 : j;
					line = i;
					if (tile[col][line].state != Tile::STATE::DEAD && tile[col][line].fruit.type != 'x')
					{
						if (is_pushed_x(col, line, fruit_pusher, dir_x, origin))
						{
							tile[col][line].fruit.animTimer = -c_animLength * abs(col - origin) + ((c_animLength * 0.625f) * abs(col - origin));
							tile[col][line].fruit.state = Fruit::STATE::MOVING;
						}
						else
						{
							if (!tile[col][line].fruit.is_petrified()) {
								tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
							}
							tile[col][line].fruit.animTimer = 42.0f;
						}
					}
				}
			}
		}
		else
		{
			origin = logic.card_effect.solo_coords.x;
			int line = logic.card_effect.solo_coords.y;
			int inc = (backward) ? -1 : 1;
			for (int j = origin; (backward) ? j >= 0 : j <= 7; j += inc)
			{
				col = j;
				if (tile[col][line].state != Tile::STATE::DEAD && tile[col][line].fruit.type != 'x')
				{
					if (is_pushed_x(col, line, fruit_pusher, dir_x, origin))
					{
						tile[col][line].fruit.animTimer = -c_animLength * abs(col - origin) + ((c_animLength * 0.625f) * abs(col - origin));
						tile[col][line].fruit.state = Fruit::STATE::MOVING;
					}
					else
					{
						if (!tile[col][line].fruit.is_petrified()) {
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
						tile[col][line].fruit.animTimer = 42.0f;
					}
				}
				else { break; }
			}
		}
	}
	else
	{
		if (logic.card_effect.solo_coords.x == -1 && logic.card_effect.solo_coords.y == -1)
		{
			for (int i = bounds.left; i <= bounds.right; ++i)
			{
				for (int j = 0; j <= 7; ++j)
				{
					line = (backward) ? j : -j + 7;
					col = i;
					if (tile[col][line].state != Tile::STATE::DEAD && tile[col][line].fruit.type != 'x')
					{
						if (is_pushed_y(col, line, fruit_pusher, dir_y, origin))
						{
							tile[col][line].fruit.animTimer = -c_animLength * abs(line - origin) + ((c_animLength * 0.625f) * abs(line - origin));
							tile[col][line].fruit.state = Fruit::STATE::MOVING;
						}
						else
						{
							if (!tile[col][line].fruit.is_petrified()) {
								tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
							}
							tile[col][line].fruit.animTimer = 42.0f;
						}
					}
				}
			}
		}
		else
		{
			origin = logic.card_effect.solo_coords.y;
			int col = logic.card_effect.solo_coords.x;
			int inc = (backward) ? 1 : -1;
			for (int j = origin; (backward) ? j <= 7 : j >= 0; j += inc)
			{
				line = j;
				if (tile[col][line].state != Tile::STATE::DEAD && tile[col][line].fruit.type != 'x')
				{
					if (is_pushed_y(col, line, fruit_pusher, dir_y, origin))
					{
						tile[col][line].fruit.animTimer = -c_animLength * abs(line - origin) + ((c_animLength * 0.625f) * abs(line - origin));
						tile[col][line].fruit.state = Fruit::STATE::MOVING;
					}
					else
					{
						if (!tile[col][line].fruit.is_petrified()) {
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
						tile[col][line].fruit.animTimer = 42.0f;
					}
				}
				else { break; }
			}
		}
	}

	/*
	// print timer
	for (int i = bounds.top; i <= bounds.bottom; ++i)
	{
		for (int j = bounds.left; j <= bounds.right; ++j)
		{
			if (tile[j][i].state != Tile::STATE::DEAD && tile[j][i].fruit.type != 'x')
			{
				std::cout << tile[j][i].fruit.animTimer << " | ";
			}
			else if (tile[j][i].state != Tile::STATE::DEAD)
			{
				std::cout << 'V' << " | ";
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
			line = i;
			for (int j = 0; j <= 7; ++j)
			{
				col = (backward) ? -j + 7 : j;
				if (tile[col][line].state != Tile::STATE::DEAD && tile[col][line].fruit.type != 'x' && !tile[col][line].fruit.is_petrified())
				{
					if (tile[col][line].fruit.type == pusher_type) {
						// check if along all consecutive fruits, none of them is petrified
						if (!exist_petrified_fruit_line(col, line, backward)) {
							index[i] = col;
							break;
						}
					}
				}
			}
		}
		origin = (backward) ? get_max_array(index, 8) : get_min_array(index, 8);
	}
	else
	{
		for (int i = bounds.left; i <= bounds.right; ++i)
		{
			col = i;
			for (int j = 0; j <= 7; ++j)
			{
				line = (backward) ? j : -j + 7;
				if (tile[col][line].state != Tile::STATE::DEAD && tile[col][line].fruit.type != 'x' && !tile[col][line].fruit.is_petrified())
				{
					if (tile[col][line].fruit.type == pusher_type) {
						// check if along all consecutive fruits, none of them is petrified
						if (!exist_petrified_fruit_column(col, line, backward)) {
							index[i] = line;
							break;
						}
					}
				}
			}
		}
		origin = (backward) ? get_min_array(index, 8) : get_max_array(index, 8);
	}
}

bool Board::is_pushed_x(int col, int line, char pusher, int dir, int origin)
{
	bool backward = (dir < 0) ? true : false;
	do
	{
		if (tile[col][line].fruit.type == pusher && !tile[col][line].fruit.is_petrified()) {
			if (!exist_petrified_fruit_line(col, line, backward)) {
				return true;
			}
		}
		col -= dir;
	} while (col >= bounds.left && col <= bounds.right && line >= bounds.top && line <= bounds.bottom && tile[col][line].fruit.type != 'x');
	return false;
}

bool Board::is_pushed_y(int col, int line, char pusher, int dir, int origin)
{
	bool backward = (dir > 0) ? true : false;
	do
	{
		if (tile[col][line].fruit.type == pusher && !tile[col][line].fruit.is_petrified()) {
			if (!exist_petrified_fruit_column(col, line, backward)) {
				return true;
			}
		}
		line += dir;
	} while (col >= bounds.left && col <= bounds.right && line >= bounds.top && line <= bounds.bottom && tile[col][line].fruit.type != 'x');
	return false;
}

void Board::update(Logic& logic)
{
	bool end_move = true;
	bool kill_tiles = false;
	for (int line = bounds.top; line <= bounds.bottom; ++line)
	{
		for (int col = bounds.left; col <= bounds.right; ++col)
		{
			if (tile[col][line].fruit.type != 'x' && tile[col][line].fruit.state == Fruit::STATE::MOVING)
			{
				if (tile[col][line].fruit.type == 'b')
				{
					Animation2D& anim = (logic.turn == 0) ? banana_anims[logic.move.dir] : banana_anims[logic.move.dir + 4];
					if (tile[col][line].fruit.animTimer <= anim.duration)
					{
						end_move = false;
						break;
					}
				}
				else
				{
					Animation2D& anim = (logic.turn == 0) ? orange_anims[logic.move.dir] : orange_anims[logic.move.dir + 4];
					if (tile[col][line].fruit.animTimer <= anim.duration)
					{
						end_move = false;
						break;
					}
				}
			}
		}
	}

	if (end_move && logic.move.dir != -1)
	{
		if (logic.move.dir == 0) // right
		{
			for (int line = bounds.top; line <= bounds.bottom; ++line)
			{
				for (int col = bounds.right; col >= bounds.left; --col)
				{
					// if tile is dead, pass
					if (tile[col][line].state == Tile::STATE::DEAD)
					{
						continue;
					}
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
						// else if left tile exists & left tile fruit is petrified
						else if (col - 1 >= bounds.left && tile[col - 1][line].fruit.is_petrified()) {
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
		else if (logic.move.dir == 1) // left
		{
			for (int line = bounds.top; line <= bounds.bottom; ++line)
			{
				for (int col = bounds.left; col <= bounds.right; ++col)
				{
					// if tile is dead, pass
					if (tile[col][line].state == Tile::STATE::DEAD)
					{
						continue;
					}
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
						// else if right tile exists & right tile fruit is petrified
						else if (col + 1 <= bounds.right && tile[col + 1][line].fruit.is_petrified()) {
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
		else if (logic.move.dir == 2) // up
		{
			for (int col = bounds.left; col <= bounds.right; ++col)
			{
				for (int line = bounds.top; line <= bounds.bottom; ++line)
				{
					// if tile is dead, pass
					if (tile[col][line].state == Tile::STATE::DEAD)
					{
						continue;
					}
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
						// else if down tile exists & down tile fruit is petrified
						else if (line + 1 <= bounds.bottom && tile[col][line + 1].fruit.is_petrified()) {
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
		else if (logic.move.dir == 3) // down
		{
			for (int col = bounds.left; col <= bounds.right; ++col)
			{
				for (int line = bounds.bottom; line >= bounds.top; --line)
				{
					// if tile is dead, pass
					if (tile[col][line].state == Tile::STATE::DEAD)
					{
						continue;
					}
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
							tile[col][line].fruit.animTimer = 0.0f;
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
						// else if up tile exists & up tile does not contain a fruit
						else if (line - 1 >= bounds.top && tile[col][line - 1].fruit.type == 'x') {
							tile[col][line].fruit.type = 'x';
							tile[col][line].fruit.animTimer = 0.0f;
							tile[col][line].fruit.state = Fruit::STATE::STAND_STILL;
						}
						// else if up tile exists & up tile fruit is petrified
						else if (line - 1 >= bounds.top && tile[col][line - 1].fruit.is_petrified()) {
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
		if (logic.card_effect.charge)
		{
			set_animTimer(logic);
			logic.card_effect.charge = false;
		}
		else
		{
			logic.move.dir = -1;
			logic.move.dir_vec = glm::vec2(0);
			if (logic.card_effect.second_wave)
			{
				logic.card_effect.second_wave = false;
			}
			else
			{
				logic.change_turn = true;
			}
		}
	}

	if (!logic.kill_tiles)
	{
		// check if a row/column of tiles can be deleted
		int c = -1;
		int l = -1;
		check_dying_tiles(c, l);
		if (c != -1)
		{
			logic.kill_tiles = true;
			set_tileDeleteTimerColumn(c);
			logic.delete_column_id = c;
		}
		else if (l != -1)
		{
			logic.kill_tiles = true;
			set_tileDeleteTimerLine(l);
			logic.delete_line_id = l;
		}
	}

	if (logic.kill_tiles)
	{
		if (logic.delete_column_id != -1)
		{
			bool all_tiles_dead = true;
			for (int i = bounds.top; i <= bounds.bottom; ++i)
			{
				if (tile[logic.delete_column_id][i].animTimer > c_animLength) {
					tile[logic.delete_column_id][i].state = Tile::STATE::DEAD;
				}
				else {
					all_tiles_dead = false;
				}
			}
			if (all_tiles_dead)
			{
				logic.kill_tiles = false;
				if (logic.delete_column_id == bounds.left) {
					bounds.left++;
				}
				else if(logic.delete_column_id == bounds.right) {
					bounds.right--;
				}
				logic.delete_column_id = -1;
				logic.delete_line_id = -1;
			}
		}
		else if (logic.delete_line_id != -1)
		{
			bool all_tiles_dead = true;
			for (int i = bounds.left; i <= bounds.right; ++i)
			{
				if (tile[i][logic.delete_line_id].animTimer > c_animLength) {
					tile[i][logic.delete_line_id].state = Tile::STATE::DEAD;
				}
				else {
					all_tiles_dead = false;
				}
			}
			if (all_tiles_dead)
			{
				logic.kill_tiles = false;
				if (logic.delete_line_id == bounds.top) {
					bounds.top++;
				}
				else if (logic.delete_line_id == bounds.bottom) {
					bounds.bottom--;
				}
				logic.delete_column_id = -1;
				logic.delete_line_id = -1;
			}
		}
	}
	else if (logic.change_turn)
	{
		logic.turn = (logic.turn == 0) ? 1 : 0;
		logic.change_turn = false;
		logic.used_a_card = false;
	}
}

void Board::check_dying_tiles(int& col, int& line)
{
	int l = 0;
	int c = 0;

	int rows[2] = { bounds.top, bounds.bottom };
	int columns[2] = { bounds.left, bounds.right };
	
	// check rows
	for (int i = 0; i < 2; ++i)
	{
		line = rows[i];
		for (c = bounds.left; c <= bounds.right; ++c)
		{
			if (tile[c][line].fruit.type != 'x' && !tile[c][line].fruit.is_petrified() && tile[c][line].state == Tile::STATE::ALIVE) {
				line = -1;
				break;
			}
		}
		if (line != -1) {
			return;
		}
	}
	// check columns
	for (int i = 0; i < 2; ++i)
	{
		col = columns[i];
		for (l = bounds.top; l <= bounds.bottom; ++l)
		{
			if (tile[col][l].fruit.type != 'x' && !tile[col][l].fruit.is_petrified() && tile[col][l].state == Tile::STATE::ALIVE) {
				col = -1;
				break;
			}
		}
		if (col != -1) {
			return;
		}
	}
}

void Board::set_tileDeleteTimerColumn(int c)
{
	for (int line = bounds.top; line <= bounds.bottom; ++line)
	{
		if (tile[c][line].state != Tile::STATE::DEAD) {
			tile[c][line].animTimer = (bounds.top - line) * c_animLength + abs(bounds.top - line) * (c_animLength * 0.625f);
			tile[c][line].state = Tile::STATE::DYING;
		}
	}
}

void Board::set_tileDeleteTimerLine(int l)
{
	for (int col = bounds.left; col <= bounds.right; ++col)
	{
		if (tile[col][l].state != Tile::STATE::DEAD) {
			tile[col][l].animTimer = (bounds.left - col) * c_animLength + abs(bounds.left - col) * (c_animLength * 0.625f);
			tile[col][l].state = Tile::STATE::DYING;
		}
	}
}

void Board::reset()
{
	for (int line = 0; line <= 7; ++line)
	{
		for (int col = 0; col <= 7; ++col)
		{
			tile[col][line].animTimer = 42.0f;
			tile[col][line].pos = glm::vec2(0);
			tile[col][line].state = Tile::STATE::ALIVE;
		}
	}
}

void Board::reset_hovered()
{
	for (int line = 0; line <= 7; ++line)
	{
		for (int col = 0; col <= 7; ++col)
		{
			tile[col][line].hovered = false;
		}
	}
}

std::vector<int> Board::get_free_tiles()
{
	std::vector<int> list;
	for (int line = 0; line <= 7; ++line)
	{
		for (int col = 0; col <= 7; ++col)
		{
			if (tile[col][line].state == Tile::STATE::ALIVE && tile[col][line].fruit.type == 'x') {
				list.push_back(col);
				list.push_back(line);
			}
		}
	}
	return list;
}

std::string Board::get_reinforcement_position(std::vector<int> & list)
{
	const int list_size = list.size() / 2;
	RandomGenerator rg(0, (list_size/2) - 1);
	
	std::vector<int> position;
	std::vector<int> index;
	int id;
	int num_bandas = std::min(3, list_size);
	for (int i = 0; i < num_bandas; ++i) {
		id = rg.gen();
		while (std::find(index.begin(), index.end(), id) != index.end()) {
			id = rg.gen();
		}
		index.push_back(id);
		position.push_back(list[id*2]);
		position.push_back(list[id*2+1]);
	}
	std::string res;
	for (int i = 0; i < num_bandas; ++i) {
		res += std::to_string(position[i * 2]) + ".";
		if (i == (num_bandas - 1)) {
			res += std::to_string(position[i * 2 + 1]);
		}
		else {
			res += std::to_string(position[i * 2 + 1]) + ".";
		}
	}
	return res;
}

glm::ivec2 Board::get_tile_coords_from_mouse_position(int x, int y)
{
	for (int line = 0; line < 8; ++line)
	{
		for (int col = 0; col < 8; ++col)
		{
			if (tile[col][line].state == Tile::STATE::ALIVE)
			{
				if (tile[col][line].pos.x + 106 < x && tile[col][line].pos.x + 150 > x) {
					if (tile[col][line].pos.y + 105 < y && tile[col][line].pos.y + 150 > y) {
						return glm::ivec2(col, line);
					}
				}
			}
		}
	}
	return glm::ivec2(-1, -1);
}

glm::ivec2 Board::get_tile_coords_from_cow_position(int charge_col, glm::vec2 pos)
{
	for (int line = 0; line < 8; ++line)
	{
		if (pos.y >= tile[charge_col][line].pos.y && pos.y <= tile[charge_col][line].pos.y + 45) {
			return glm::ivec2(charge_col, line);
		}
	}
	return glm::ivec2(-1, -1);
}
