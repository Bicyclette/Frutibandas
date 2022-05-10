#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include "window.hpp"
#include "game.hpp"
#include "framebuffer.hpp"
#include "editorUI.hpp"
#include "allocation.hpp"

void render(std::unique_ptr<WindowManager> client, std::unique_ptr<Game> game)
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
		if (game->getCursorFocus() == 2) {
			client->checkEvents();
		}
        else {
            client->checkEvents(true);
        }
		game->updateUI(client->getUserInputs(), client->get_text_input(), client->getWidth(), client->getHeight(), delta);
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
	std::unique_ptr<WindowManager> client{std::make_unique<WindowManager>("Frutibandas")};
	std::unique_ptr<Game> game{std::make_unique<Game>(client->getWidth(), client->getHeight())};
	render(std::move(client), std::move(game));

	return 0;
}
