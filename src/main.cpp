#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include "window.hpp"
#include "game.hpp"
#include "framebuffer.hpp"
#include "editorUI.hpp"
#include "allocation.hpp"

#define SERVER "192.168.3.28"
#define PORT 7777

void connect(std::shared_ptr<Game>& game)
{
	std::string message;
	g_msg2server_mtx.lock();
	if (!g_msg2server.empty())
	{
		message = g_msg2server.front();
		g_msg2server.pop();
	}
	else
	{
		g_msg2server_mtx.unlock();
		return;
	}
	g_msg2server_mtx.unlock();
	int code = std::atoi(message.substr(0, message.find_first_of(':')).c_str());

	if(code == 0)
	{
		game->m_bandas.m_net.connect(SERVER, PORT);
	}
}

void send_message(std::shared_ptr<Game> & game)
{
	std::string message;
	g_msg2server_mtx.lock();
	if (!g_msg2server.empty())
	{
		message = g_msg2server.front();
		g_msg2server.pop();
	}
	else
	{
		g_msg2server_mtx.unlock();
		return;
	}
	g_msg2server_mtx.unlock();
	int code = std::atoi(message.substr(0, message.find_first_of(':')).c_str());
	if (code == 1) // play (search opponent)
	{
		game->m_bandas.m_net.send_data("so");
		game->m_bandas.m_net.search_game(true);
	}
	else if (code == 2) // stop search opponent
	{
		game->m_bandas.m_net.send_data("sso");
		game->m_bandas.m_net.search_game(false);
	}
}

void receive_message(std::shared_ptr<Game> & game)
{
	if (game->m_bandas.m_net.service())
	{
		if (game->m_bandas.m_net.m_event.type == ENET_EVENT_TYPE_RECEIVE)
		{
			std::string message(reinterpret_cast<char*>(game->m_bandas.m_net.m_event.packet->data));
			std::string type(message.substr(0, message.find_first_of(':')));
			
		}
		else if (game->m_bandas.m_net.m_event.type == ENET_EVENT_TYPE_DISCONNECT)
		{
			game->m_bandas.m_net.disconnect();
		}
	}
}

void network_thread(std::shared_ptr<WindowManager>& client, std::shared_ptr<Game>& game)
{
	while (client->isAlive())
	{
		if (game->m_bandas.m_net.is_connected())
		{
			send_message(game);
			receive_message(game);
		}
		else
		{
			connect(game);
		}
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
