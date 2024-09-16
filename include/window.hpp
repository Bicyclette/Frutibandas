#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <bitset>
//#include <omp.h>
#include <cmath>
#include <memory>
#include "color.hpp"
#include "gl_debug.hpp"
#include "communication.hpp"
#include "bandas.hpp"

class Bandas;

struct WindowEvent
{
	SDL_Event e;
	const Uint8* keyboardState; // snapshot of the keyboard state
	Uint32 mouseButtonBitMask;
};

// about the bitset which registers user inputs and events
// 0 => right mouse button
// 1 => middle mouse button
// 2 => left mouse button
// 3 => wheel scroll
// 4 => screen resize
// 5 => arrow up
// 6 => arrow down
// 7 => arrow right
// 8 => arrow left

class WindowManager
{
	public:

		/**
 		* \brief Creates a SDL window, with an OPENGL context, returns a pointer
 		* to the newly allocated window, or nullptr on failure.
 		*/
		WindowManager(const std::string& title);
		~WindowManager();
		SDL_Window* getWindowPtr();
		bool isAlive();
		void checkEvents(Bandas& bandas, bool writing = false);
		void resetEvents();

	private:

		void fillWriteInput();

		std::string title;
		SDL_Window * window;
		SDL_GLContext glContext;
		struct WindowEvent event;

	public:

		bool m_alive;
		std::array<int, 3> m_mouseData; // 0 = xRel, 1 = yRel, 2 = mouse wheel direction
		std::bitset<10> m_userInputs;
		char m_textInput[32];
};

#endif
