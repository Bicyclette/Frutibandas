#ifndef MOUSE_HPP
#define MOUSE_HPP

#include <SDL2/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <string>

class Mouse
{
	public:
		Mouse(std::string img_target);
		~Mouse();
		void use_normal() { m_used = 0; SDL_SetCursor(m_normal); }
		void use_hover() { m_used = 1; SDL_SetCursor(m_hover); }
		void use_target() { m_used = 2; SDL_SetCursor(m_target); }
		uint8_t in_use() { return m_used; }
		glm::ivec2 get_position() {
			int x, y;
			SDL_GetMouseState(&x, &y);
			return glm::ivec2(x, y);
		}

	private:

		void create_target_cursor(std::string img_target);
		SDL_Cursor* m_normal;
		SDL_Cursor* m_hover;
		SDL_Cursor* m_target;
		SDL_Surface* m_target_surface;
		bool m_active;
		uint8_t m_used; // 0 = normal, 1 = hover, 2 = target
};

#endif