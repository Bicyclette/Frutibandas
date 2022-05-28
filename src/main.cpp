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

void network_thread(bool& run, Writer& writer, MOVE& move, const std::shared_ptr<Game> & g)
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
					case 5:
						client.send_data("mv:" + message.substr(2)); // move
						break;
					case 6:
						client.send_data("rt:" + message.substr(2)); // remaining time
						break;
					case 7:
						client.send_data("c:" + message.substr(2)); // card
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
					else if (type == "mv") { // move
						int dir = std::atoi(&message[3]);
						int who = std::atoi(&message[5]);
						g_turn_mutex.lock();
						int turn = g->m_turn;
						g_turn_mutex.unlock();
						g_fruit_move_mutex.lock();
						if (dir == 1) {
							move = MOVE::UP;
						}
						else if (dir == 2) {
							move = MOVE::DOWN;
						}
						else if (dir == 3) {
							move = MOVE::RIGHT;
						}
						else if (dir == 4) {
							move = MOVE::LEFT;
						}
						g_fruit_move_mutex.unlock();
						if (who == g->getTeam()) {
							g->set_animationTimer(false);
						}
						else {
							g->set_animationTimer(true);
						}
						if (g->m_board.m_invert_next_move && g->m_fruit == turn) {
							g->m_board.m_invert_next_move = false;
							g->m_board.m_invert_next_move_team = -1;
						}
					}
					else if (type == "t") { // turn change
						int turn = std::atoi(message.substr(2).c_str());
						std::cout << message << std::endl;
						if (turn == 0) {
							std::cout << "orange is playing" << std::endl;
						}
						else if (turn == 1) {
							std::cout << "banana is playing" << std::endl;
						}
						g_turn_mutex.lock();
						g->m_turn = turn;
						g_turn_mutex.unlock();
					}
					else if (type == "win") { // a player won the match
						int winner = std::atoi(message.substr(4).c_str());
						g_winner_mutex.lock();
						g->m_winner = winner;
						g_winner_mutex.unlock();
						g_turn_mutex.lock();
						g->m_turn = -1;
						g_turn_mutex.unlock();
						g->getScenes()[0].getSoundSource(0).stop_sound();
						g->getScenes()[0].getSoundSource(0).set_volume(0.25f);
						if (g->m_fruit == winner) {
							g->m_popup.set_background_img_gl(g->m_popup_tex[0].id);
							g->getScenes()[0].getSoundSource(0).set_looping(false);
							g->getScenes()[0].playSound(0, 1);
						}
						else {
							g->m_popup.set_background_img_gl(g->m_popup_tex[1].id);
							g->getScenes()[0].getSoundSource(0).set_looping(false);
							g->getScenes()[0].playSound(0, 2);
						}
					}
					else if (type == "rt") { // remaining time of enemy
						std::string remaining = message.substr(3);
						g_rte_mutex.lock();
						g->m_remaining_time_enemy = std::stof(remaining);
						g_rte_mutex.unlock();
					}
					else if (type == "c") { // enemy player used a card
						std::cout << message << std::endl;
						std::istringstream card_stream(message);
						std::vector<int> info;
						std::string element;
						while (getline(card_stream, element, ':'))
						{
							info.push_back(std::atoi(element.c_str()));
						}
						int card_id = info[1];
						int line = info[2];
						int col = info[3];
						if (card_id == 3) // renfort
						{
							int num_renfort = info[4];
							glm::ivec2 p1(info[5], info[6]);
							glm::ivec2 p2(info[7], info[8]);
							glm::ivec2 p3(info[9], info[10]);
							int fruit_renfort = info[11];
							g->apply_card_renfort(fruit_renfort, num_renfort, { p1, p2, p3 });
						}
						else
						{
							g->use_enemy_card(card_id, line, col);
						}
					}
				}
				else if (client.m_event.type == ENET_EVENT_TYPE_DISCONNECT)
				{
					g_connected_mutex.lock();
					g_connected = false;
					g_connected_mutex.unlock();
				}
			}
		}

		// disconnect
		client.disconnect();
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
	std::unique_ptr<WindowManager> client{ std::make_unique<WindowManager>("Frutibandas") };
	std::shared_ptr<Game> game{ std::make_shared<Game>(client->getWidth(), client->getHeight()) };
	// network thread
	std::thread net_thread(network_thread, std::ref(client->isAlive()), std::ref(game->get_writer()), std::ref(game->get_move()), std::ref(game));
	// render game
	render(std::move(client), game);
	// exit
	net_thread.join();

	return 0;
}
