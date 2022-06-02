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

void network_thread(bool& run, Bandas & bandas)
{
	NetworkClient client;
	
	while (run)
	{
		std::string message;
		
	}
}

void render(std::unique_ptr<WindowManager> client, std::shared_ptr<Game> game)
{
    // IMGUI data
    EDITOR_UI_SETTINGS editor_settings;
    
    // start
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
		if (game->m_bandas.m_writer.m_cursor.m_focus == 2) { client->checkEvents(); }
        else { client->checkEvents(true); }
		if (game->m_bandas.m_ui.get_active_page() == 0) { game->m_bandas.update_home_page(client->getUserInputs(), client->get_text_input(), delta); }
		else if (game->m_bandas.m_ui.get_active_page() == 1) { game->m_bandas.update_game_page(client->getUserInputs(), client->get_text_input(), delta); }
		game->updateSceneActiveCameraView(game->getActiveScene(), client->getUserInputs(), client->getMouseData(), delta);

		// draw scene
		game->draw(delta, currentFrame, client->getWidth(), client->getHeight(), draw_mode, debug, debugPhysics);

		client->resetEvents();
		SDL_GL_SwapWindow(client->getWindowPtr());
		lastFrame = currentFrame;
	}
}

int main(int argc, char* argv[])
{
	std::unique_ptr<WindowManager> client{ std::make_unique<WindowManager>("Frutibandas") };
	std::shared_ptr<Game> game{ std::make_shared<Game>(client->getWidth(), client->getHeight()) };
	
	// network thread
	std::thread net_thread(network_thread, std::ref(client->isAlive()), std::ref(game->m_bandas));
	// render game
	render(std::move(client), game);
	// exit
	net_thread.join();

	return 0;
}
