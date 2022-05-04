#include "window.hpp"

WindowManager::WindowManager(const std::string& title)
{
	alive = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << SDL_GetError() << std::endl;
		std::exit(-1);
	}

	// Get screen size and create main window
	SDL_Rect r;
	if(SDL_GetDisplayBounds(0, &r) != 0)
	{
		std::cerr << SDL_GetError() << std::endl;
		std::exit(-1);
	}

	width = 1050;
	height = 728;

	window = nullptr;

	// OPENGL VERSION
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	// DOUBLE BUFFER
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// MULTISAMPLING
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	window = SDL_CreateWindow(
				title.c_str(),
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				width,
				height,
				SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS
				);

	if(window == nullptr)
	{
		std::cerr << SDL_GetError() << std::endl;
		SDL_Quit();
		std::exit(-1);
	}

	glContext = SDL_GL_CreateContext(window);

	// Glew init
	glewExperimental = true;
	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		std::cerr << glewGetErrorString(err) << std::endl;
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(window);
		SDL_Quit();
		std::exit(-1);
	}

	// Setting OpenGL states
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glClearColor(LIGHT_GREY[0], LIGHT_GREY[1], LIGHT_GREY[2], LIGHT_GREY[3]);
	SDL_GL_SetSwapInterval(1);

	// IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init();
}

WindowManager::~WindowManager()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int WindowManager::getWidth()
{
	return width;
}

int WindowManager::getHeight()
{
	return height;
}

SDL_Window* WindowManager::getWindowPtr()
{
	return window;
}

std::array<int, 3> & WindowManager::getMouseData()
{
	return mouseData;
}

std::bitset<10>& WindowManager::getUserInputs()
{
	return userInputs;
}

std::bitset<52>& WindowManager::getWriteInput()
{
	return writeInput;
}

bool WindowManager::isAlive()
{
	return alive;
}

void WindowManager::checkEvents(bool writing)
{
	event.keyboardState = SDL_GetKeyboardState(nullptr);
	event.mouseButtonBitMask = SDL_GetRelativeMouseState(&mouseData[0], &mouseData[1]);

	while(SDL_PollEvent(&event.e))
	{
		// forward to ImGui
		ImGui_ImplSDL2_ProcessEvent(&event.e);

		if(event.e.type == SDL_QUIT)
		{
			alive = false;
		}

		if(event.e.type == SDL_WINDOWEVENT)
		{
			if(event.e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				userInputs.set(4);
				width = event.e.window.data1;
				height = event.e.window.data2;
				glViewport(0, 0, width, height);
			}
		}

		if (event.e.type == SDL_MOUSEBUTTONUP)
		{
			userInputs.set(9);
		}

		if(event.e.type == SDL_MOUSEWHEEL)
		{
			userInputs.set(3);
			mouseData[2] = event.e.wheel.y;
		}
	}

	if(SDL_BUTTON(event.mouseButtonBitMask) == SDL_BUTTON_LEFT)
	{
		userInputs.set(2);
	}
	
	if(SDL_BUTTON(event.mouseButtonBitMask) == SDL_BUTTON_MIDDLE)
	{
		userInputs.set(1);
	}
	
	if(SDL_BUTTON(event.mouseButtonBitMask) == SDL_BUTTON_RIGHT)
	{
		userInputs.set(0);
	}

	if (writing)
	{
		fillWriteInput();
		return;
	}

	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_UP)])
	{
		userInputs.set(5);
	}

	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_DOWN)])
	{
		userInputs.set(6);
	}

	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_RIGHT)])
	{
		userInputs.set(7);
	}

	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_LEFT)])
	{
		userInputs.set(8);
	}
}

void WindowManager::resetEvents()
{
	userInputs.reset();
	writeInput.reset();
}

void WindowManager::fillWriteInput()
{
	// alphabet
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_a)])
	{
		writeInput.set(0);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_b)])
	{
		writeInput.set(1);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_c)])
	{
		writeInput.set(2);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_d)])
	{
		writeInput.set(3);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_e)])
	{
		writeInput.set(4);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_f)])
	{
		writeInput.set(5);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_g)])
	{
		writeInput.set(6);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_h)])
	{
		writeInput.set(7);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_i)])
	{
		writeInput.set(8);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_j)])
	{
		writeInput.set(9);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_k)])
	{
		writeInput.set(10);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_l)])
	{
		writeInput.set(11);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_m)])
	{
		writeInput.set(12);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_n)])
	{
		writeInput.set(13);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_o)])
	{
		writeInput.set(14);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_p)])
	{
		writeInput.set(15);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_q)])
	{
		writeInput.set(16);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_r)])
	{
		writeInput.set(17);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_s)])
	{
		writeInput.set(18);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_t)])
	{
		writeInput.set(19);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_u)])
	{
		writeInput.set(20);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_v)])
	{
		writeInput.set(21);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_w)])
	{
		writeInput.set(22);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_x)])
	{
		writeInput.set(23);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_y)])
	{
		writeInput.set(24);
	}
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_z)])
	{
		writeInput.set(25);
	}
	// digit
	if ((event.keyboardState[SDL_GetScancodeFromKey(SDLK_LSHIFT)] && event.keyboardState[SDL_GetScancodeFromKey(SDLK_0)]) ||
		event.keyboardState[SDL_GetScancodeFromKey(SDLK_KP_0)])
	{
		writeInput.set(26);
	}
	if ((event.keyboardState[SDL_GetScancodeFromKey(SDLK_LSHIFT)] && event.keyboardState[SDL_GetScancodeFromKey(SDLK_1)]) ||
		event.keyboardState[SDL_GetScancodeFromKey(SDLK_KP_1)])
	{
		writeInput.set(27);
	}
	if ((event.keyboardState[SDL_GetScancodeFromKey(SDLK_LSHIFT)] && event.keyboardState[SDL_GetScancodeFromKey(SDLK_2)]) ||
		event.keyboardState[SDL_GetScancodeFromKey(SDLK_KP_2)])
	{
		writeInput.set(28);
	}
	if ((event.keyboardState[SDL_GetScancodeFromKey(SDLK_LSHIFT)] && event.keyboardState[SDL_GetScancodeFromKey(SDLK_3)]) ||
		event.keyboardState[SDL_GetScancodeFromKey(SDLK_KP_3)])
	{
		writeInput.set(29);
	}
	if ((event.keyboardState[SDL_GetScancodeFromKey(SDLK_LSHIFT)] && event.keyboardState[SDL_GetScancodeFromKey(SDLK_4)]) ||
		event.keyboardState[SDL_GetScancodeFromKey(SDLK_KP_4)])
	{
		writeInput.set(30);
	}
	if ((event.keyboardState[SDL_GetScancodeFromKey(SDLK_LSHIFT)] && event.keyboardState[SDL_GetScancodeFromKey(SDLK_5)]) ||
		event.keyboardState[SDL_GetScancodeFromKey(SDLK_KP_5)])
	{
		writeInput.set(31);
	}
	if ((event.keyboardState[SDL_GetScancodeFromKey(SDLK_LSHIFT)] && event.keyboardState[SDL_GetScancodeFromKey(SDLK_6)]) ||
		event.keyboardState[SDL_GetScancodeFromKey(SDLK_KP_6)])
	{
		writeInput.set(32);
	}
	if ((event.keyboardState[SDL_GetScancodeFromKey(SDLK_LSHIFT)] && event.keyboardState[SDL_GetScancodeFromKey(SDLK_7)]) ||
		event.keyboardState[SDL_GetScancodeFromKey(SDLK_KP_7)])
	{
		writeInput.set(33);
	}
	if ((event.keyboardState[SDL_GetScancodeFromKey(SDLK_LSHIFT)] && event.keyboardState[SDL_GetScancodeFromKey(SDLK_8)]) ||
		event.keyboardState[SDL_GetScancodeFromKey(SDLK_KP_8)])
	{
		writeInput.set(34);
	}
	if ((event.keyboardState[SDL_GetScancodeFromKey(SDLK_LSHIFT)] && event.keyboardState[SDL_GetScancodeFromKey(SDLK_9)]) ||
		event.keyboardState[SDL_GetScancodeFromKey(SDLK_KP_9)])
	{
		writeInput.set(35);
	}
	// underscore
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_UNDERSCORE)])
	{
		writeInput.set(36);
	}
	// dash
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_KP_MINUS)] || event.keyboardState[SDL_GetScancodeFromKey(SDLK_MINUS)])
	{
		writeInput.set(37);
	}
	// space
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_SPACE)])
	{
		writeInput.set(38);
	}
	// star
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_KP_MULTIPLY)] || event.keyboardState[SDL_GetScancodeFromKey(SDLK_ASTERISK)])
	{
		writeInput.set(39);
	}
	// backspace
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_BACKSPACE)])
	{
		writeInput.set(40);
	}
	// left
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_LEFT)])
	{
		writeInput.set(41);
	}
	// right
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_RIGHT)])
	{
		writeInput.set(42);
	}
	// shift
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_LSHIFT)])
	{
		writeInput.set(43);
	}
	// maj
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_CAPSLOCK)])
	{
		writeInput.set(44);
	}
	// point
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_LSHIFT)] && event.keyboardState[SDL_GetScancodeFromKey(SDLK_PERIOD)])
	{
		writeInput.set(44);
	}
	// question
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_LSHIFT)] && event.keyboardState[SDL_GetScancodeFromKey(SDLK_QUESTION)])
	{
		writeInput.set(44);
	}
	// exclamation
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_KP_EXCLAM)])
	{
		writeInput.set(44);
	}
	// comma
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_COMMA)])
	{
		writeInput.set(44);
	}
	// semicolon
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_SEMICOLON)])
	{
		writeInput.set(44);
	}
	// div
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_KP_DIVIDE)])
	{
		writeInput.set(44);
	}
	// add
	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_KP_PLUS)])
	{
		writeInput.set(44);
	}
}