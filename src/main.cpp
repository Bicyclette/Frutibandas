#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include "window.hpp"
#include "game.hpp"
#include "framebuffer.hpp"
#include "editorUI.hpp"
#include "allocation.hpp"

#define SERVER "92.88.236.2"
#define PORT 7777

void network_thread(std::shared_ptr<WindowManager>& client, std::shared_ptr<Game>& game)
{	
	while (client->isAlive())
	{
		std::string message;
		
	}
}

void render(std::shared_ptr<Game> game, std::shared_ptr<WindowManager> client)
{
    // IMGUI data
    EDITOR_UI_SETTINGS editor_settings;
    
    // start
	bool alive;
	DRAWING_MODE draw_mode{DRAWING_MODE::SOLID};
    bool debug{false};
    bool debugPhysics{false};
	game->setActiveScene(0);

	// delta
	double currentFrame{0.0f};
	double lastFrame{0.0};
	float delta{0.0f};

	while(client->isAlive())
	{
		currentFrame = omp_get_wtime();
		delta = static_cast<float>(currentFrame - lastFrame);

		if (game->m_bandas.m_writer.m_cursor.m_focus == 2)
		{
			client->checkEvents();
		}
		else
		{
			client->checkEvents(true);
		}
		if (game->m_bandas.m_ui.get_active_page() == 0)
		{
			game->m_bandas.update_home_page(client->m_userInputs, std::string(client->m_textInput), delta);
		}
		else if (game->m_bandas.m_ui.get_active_page() == 1)
		{
			game->m_bandas.update_game_page(client->m_userInputs, std::string(client->m_textInput), delta);
		}
		game->updateSceneActiveCameraView(game->getActiveScene(), client->m_userInputs, client->m_mouseData, delta);

		// draw scene
		game->draw(delta, currentFrame, c_screen_width, c_screen_height, draw_mode, debug, debugPhysics);

		client->resetEvents();
		SDL_GL_SwapWindow(client->getWindowPtr());
		lastFrame = currentFrame;
	}
}

int main(int argc, char* argv[])
{
	std::shared_ptr<WindowManager> client{ std::make_shared<WindowManager>("Frutibandas") };
	std::shared_ptr<Game> game{ std::make_shared<Game>(c_screen_width, c_screen_height) };

	// network thread
	std::thread net_thread(network_thread, std::ref(client), std::ref(game));
	// render game
	render(game, client);
	// exit
	net_thread.join();

	return 0;
}
