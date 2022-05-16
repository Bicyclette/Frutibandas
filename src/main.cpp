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

void network_thread(bool& run, Writer& writer)
{
	NetworkClient client;
	
	while (run)
	{
		std::string message;
		g_msg2server_mutex.lock();
		if (!g_msg2server_queue.empty()) {
			message = g_msg2server_queue.front();
			g_msg2server_queue.pop();
		}
		else {
			g_msg2server_mutex.unlock();
			continue;
		}
		g_msg2server_mutex.unlock();

		// processing
		int code = std::atoi(message.substr(0, message.find_first_of(':')).c_str());

		if (code == 0) {
			if (client.connect(SERVER, PORT)) {
				g_msg2client_mutex.lock();
				g_msg2client_queue.emplace("0:1");
				g_msg2client_mutex.unlock();
				g_connected_mutex.lock();
				g_connected = true;
				g_connected_mutex.unlock();
				// send nickname and profile picture
				std::string nn("nn" + message.substr(message.find_first_of(':') + 1, message.find_last_of(':') - 2));
				std::string pp("pp" + message.substr(message.find_last_of(':') + 1));
				client.send_data(nn);
				client.send_data(pp);
			}
			else {
				g_msg2client_mutex.lock();
				g_msg2client_queue.emplace("0:0");
				g_msg2client_mutex.unlock();
			}
		}

		// connected to server
		while (g_connected)
		{
			std::string message;
			g_msg2server_mutex.lock();
			if (!g_msg2server_queue.empty()) {
				message = g_msg2server_queue.front();
				g_msg2server_queue.pop();
				g_msg2server_mutex.unlock();
				
				// processing
				int code = std::atoi(message.substr(0, message.find_first_of(':')).c_str());
				switch (code)
				{
					case 1:
						client.send_data("so"); // search opponent
						break;
					case 2:
						client.send_data("sso"); // stop stearch opponent
						break;
					case 3:
						client.send_data("gu"); // give up
						break;
					case 4:
						if (!message.substr(2).empty()) {
							client.send_data("gc:" + message.substr(2)); // game chat
						}
						break;
					default:
						break;
				};
			}
			else{
				g_msg2server_mutex.unlock();
			}

			if (client.service())
			{
				if (client.m_event.type == ENET_EVENT_TYPE_RECEIVE)
				{
					std::string message(reinterpret_cast<char*>(client.m_event.packet->data));
					std::string type(message.substr(0, message.find_first_of(':')));
					if (type == "g0") { // game init
						g_game_init_mutex.lock();
						g_game_found = true;
						g_game_init = message.substr(message.find_first_of(':')+1);
						g_game_init_mutex.unlock();
					}
					else if (type == "gc") { // game chat
						std::string data = message.substr(message.find_first_of(':') + 1);
						int messageLength{std::atoi(data.substr(0, data.find_first_of(':')).c_str())};
						if(messageLength > 0){
							std::string chatMsg = data.substr(data.find_first_of(':') + 1);
							if (writer.m_chatLog.size() == 7)
							{
								for (int i{ 0 }; i < 6; ++i) {
									writer.m_chatLog[i] = writer.m_chatLog[i + 1];
								}
								writer.m_chatLog[6] = chatMsg;
							}
							else {
								writer.m_chatLog.push_back(chatMsg);
							}
						}
					}
				}
			}
		}

		// disconnect
		client.disconnect();
	}
}

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
	// network thread
	std::thread net_thread(network_thread, std::ref(client->isAlive()), std::ref(game->get_writer()));
	// render game
	render(std::move(client), std::move(game));

	return 0;
}
