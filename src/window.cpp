#include "window.hpp"

WindowManager::WindowManager(const std::string& title)
{
	m_alive = true;

	// enable composition for X11
	SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");

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

	int width = 1050;
	int height = 728;

	window = nullptr;

	// OPENGL VERSION
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	// CREATE OPENGL DEBUG CONTEXT IF DEBUG BUILD_TYPE
#if _DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

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
				SDL_WINDOW_OPENGL
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

#if _DEBUG
	if (glDebugMessageCallback) {
		std::cout << "Register OpenGL debug callback " << std::endl;
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(openglCallbackFunction, nullptr);
		GLuint unusedIds = 0;
		glDebugMessageControl(GL_DONT_CARE,
			GL_DONT_CARE,
			GL_DONT_CARE,
			0,
			&unusedIds,
			true);
	}
	else
	{
		std::cout << "glDebugMessageCallback not available" << std::endl;
	}
#endif
}

WindowManager::~WindowManager()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

SDL_Window* WindowManager::getWindowPtr()
{
	return window;
}

bool WindowManager::isAlive()
{
	return m_alive;
}

void WindowManager::checkEvents(Bandas& bandas, bool writing)
{
	event.keyboardState = SDL_GetKeyboardState(nullptr);
	event.mouseButtonBitMask = SDL_GetRelativeMouseState(&m_mouseData[0], &m_mouseData[1]);

	while(SDL_PollEvent(&event.e))
	{
		if(event.e.type == SDL_QUIT)
		{
			m_alive = false;

			// save pseudo and frutibouille
			std::ofstream user_data;
			user_data.open("user.txt", std::ios::trunc);
			if (user_data.is_open())
			{
				user_data << bandas.m_me.m_pseudo << "\n";
				user_data << bandas.m_me.m_avatar.get_net_data();
				user_data.close();
			}
			else
			{
				std::cerr << "ERROR: Failed saving pseudo and frutibouille data." << std::endl;
			}
			// send leave message to server
			g_msg2server_mtx.lock();
			g_leave_game = true;
			g_msg2server.emplace("9");
			g_msg2server_mtx.unlock();
			g_cv_connect_leave.notify_one();
		}

		if(event.e.type == SDL_WINDOWEVENT)
		{
			if(event.e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				m_userInputs.set(4);
				int width = event.e.window.data1;
				int height = event.e.window.data2;
				glViewport(0, 0, width, height);
			}
		}

		if (event.e.type == SDL_MOUSEBUTTONUP)
		{
			m_userInputs.set(9);
		}

		if(event.e.type == SDL_MOUSEWHEEL)
		{
			m_userInputs.set(3);
			m_mouseData[2] = event.e.wheel.y;
		}
		if (writing)
		{
			if (event.e.type == SDL_TEXTINPUT)
			{
				memcpy(m_textInput, event.e.text.text, 32);
			}
			else
			{
				for (int i{ 0 }; i < 32; ++i) {
					m_textInput[i] = '\0';
				}
			}
		}
	}

	if(SDL_BUTTON(event.mouseButtonBitMask) == SDL_BUTTON_LEFT)
	{
		m_userInputs.set(2);
	}
	
	if(SDL_BUTTON(event.mouseButtonBitMask) == SDL_BUTTON_MIDDLE)
	{
		m_userInputs.set(1);
	}
	
	if(SDL_BUTTON(event.mouseButtonBitMask) == SDL_BUTTON_RIGHT)
	{
		m_userInputs.set(0);
	}

	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_RETURN)])
	{
		m_userInputs.set(5);
	}

	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_BACKSPACE)])
	{
		m_userInputs.set(6);
	}

	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_RIGHT)])
	{
		m_userInputs.set(7);
	}

	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_LEFT)])
	{
		m_userInputs.set(8);
	}
}

void WindowManager::resetEvents()
{
	m_userInputs.reset();
}
