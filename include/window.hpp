#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <iostream>
#include <string>
#include <array>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <bitset>
#include <omp.h>
#include <cmath>
#include <memory>
#include "color.hpp"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

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
		int getWidth();
		int getHeight();
		SDL_Window* getWindowPtr();
		std::array<int, 3> & getMouseData();
		std::bitset<10>& getUserInputs();
		std::bitset<52>& getWriteInput();
		bool isAlive();
		void checkEvents(bool writing = false);
		void resetEvents();

	private:

		void fillWriteInput();

		std::string title;
		int width;
		int height;
		bool alive;

		SDL_Window * window;
		SDL_GLContext glContext;

		struct WindowEvent event;
		std::array<int, 3> mouseData; // 0 = xRel, 1 = yRel, 2 = mouse wheel direction
		std::bitset<10> userInputs;
		std::bitset<52> writeInput;
		// 26 letters
		// 10 digits
		// underscore
		// dash
		// space
		// star
		// backspace
		// left
		// right
		// shift
		// maj
		// point
		// question
		// exclamation
		// comma
		// semicolon
		// div
		// add
};

#endif
