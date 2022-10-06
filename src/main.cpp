#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include "window.hpp"
#include "game.hpp"
#include "framebuffer.hpp"
#include "editorUI.hpp"
#include "allocation.hpp"

#define SERVER "127.0.0.1"
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
		if (game->m_bandas.m_net.connect(SERVER, PORT))
		{
			// send pseudo
			game->m_bandas.m_net.send_data("nn:" + game->m_bandas.m_me.m_pseudo);
		}
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
		// send avatar
		game->m_bandas.m_net.send_data("pp:" + game->m_bandas.m_me.m_avatar.get_net_data());
		// then declare we are in queue
		game->m_bandas.m_net.send_data("so");
		game->m_bandas.m_net.search_game(true);
	}
	else if (code == 2) // stop search opponent
	{
		game->m_bandas.m_net.send_data("sso");
		game->m_bandas.m_net.search_game(false);
	}
	else if (code == 3) // give up game
	{
		game->m_bandas.m_net.send_data("gu");
	}
	else if (code == 4) // game chat message
	{
		game->m_bandas.m_net.send_data("gc:" + message.substr(2));
	}
	else if (code == 5) // movement
	{
		game->m_bandas.m_net.send_data("mv:" + message.substr(2));
	}
	else if (code == 6) // winner detected
	{
		game->m_bandas.m_net.send_data("win:" + message.substr(2));
	}
	else if (code == 7) // used a card
	{
		message = message.substr(2);
		int next_token = message.find_first_of('.');
		int card_id = std::atoi(message.substr(0, next_token).c_str());
		message = message.substr(next_token + 1);
		next_token = message.find_first_of('.');
		std::string card_index = message.substr(0, next_token);
		message = message.substr(next_token + 1);
		next_token = message.find_first_of('.');
		std::string effect_destination = message.substr(0, next_token);
		switch (card_id)
		{
		case 2:
			game->m_bandas.m_net.send_data("card:2." + card_index);
			break;
		case 3:
			game->m_bandas.m_net.send_data("card:3." + card_index);
			break;
		case 4:
			game->m_bandas.m_net.send_data("card:4." + card_index + "." + effect_destination);
			break;
		case 5:
			game->m_bandas.m_net.send_data("card:5." + card_index);
			break;
		default:
			break;
		}
	}
}

void receive_message(std::shared_ptr<Game> & game)
{
	int next_token;
	if (game->m_bandas.m_net.service())
	{
		if (game->m_bandas.m_net.m_event.type == ENET_EVENT_TYPE_RECEIVE)
		{
			std::string message(reinterpret_cast<char*>(game->m_bandas.m_net.m_event.packet->data));
			std::string type(message.substr(0, message.find_first_of(':')));
			if (type == "gs")
			{
				message = message.substr(6);
				// get pseudo
				next_token = message.find_first_of(':');
				std::string pseudo = message.substr(0, next_token);
				message = message.substr(next_token+4);

				// get avatar
				next_token = message.find_first_of(':');
				std::string avatar = message.substr(0, next_token);
				message = message.substr(next_token+6);
				
				// get team
				next_token = message.find_first_of(':');
				int team = std::stoi(message.substr(0, next_token));
				message = message.substr(next_token+3);
				
				// get board
				next_token = message.find_first_of(':');
				std::string plateau = message.substr(0, next_token);
				message = message.substr(next_token + 6);
				
				// get turn
				next_token = message.find_first_of(':');
				int turn = std::stoi(message.substr(0, next_token));
				message = message.substr(next_token + 3);

				//get set of cards
				next_token = message.find_first_of(':');
				std::string cards = message.substr(0, next_token);

				// init board
				game->m_bandas.m_board.init(plateau);

				// set turn
				game->m_bandas.m_logic.turn = turn;

				// set cards
				game->m_bandas.init_cards(cards);

				// set my data
				game->m_bandas.m_me.m_team = team;
				game->m_bandas.m_me.m_chrono.m_color = c_green;

				// set enemy data
				game->m_bandas.m_enemy.m_team = (team == 0) ? 1 : 0;
				game->m_bandas.m_enemy.m_chrono.m_color = c_red;
				game->m_bandas.m_enemy.m_pseudo = pseudo;
				game->m_bandas.m_enemy.m_avatar.create_from_net_data(avatar);
				// switch to game page
				game->m_bandas.start_game();
			}
			else if (type == "gu")
			{
				game->m_bandas.enemy_gave_up();
			}
			else if (type == "dc")
			{
				game->m_bandas.enemy_disconnected();
			}
			else if (type == "gc")
			{
				std::string data = message.substr(3);
				int messageLength = data.size();
				if (messageLength > 0)
				{
					game->m_bandas.add_chat_message(data);
				}
			}
			else if (type == "mv")
			{
				std::string move_dir_str = message.substr(3, 1);
				std::string chrono_timer_str = message.substr(5);
				int move_dir = std::atoi(move_dir_str.data());
				float chrono_timer = std::atof(chrono_timer_str.data());
				if (game->m_bandas.m_me.m_team != game->m_bandas.m_logic.turn) {
					game->m_bandas.m_enemy.m_chrono.m_time = chrono_timer;
				}
				if (game->m_bandas.m_logic.card_effect.disorder && game->m_bandas.m_logic.turn == game->m_bandas.m_logic.card_effect.disorder_destination) {
					game->m_bandas.m_advertiser.m_show = true;
					game->m_bandas.m_logic.card_effect.reset();
				}
				game->m_bandas.m_logic.move.dir = move_dir;
				if (move_dir == 0)
				{
					game->m_bandas.m_logic.move.dir_vec = glm::vec2(1, 0);
				}
				else if (move_dir == 1)
				{
					game->m_bandas.m_logic.move.dir_vec = glm::vec2(-1, 0);
				}
				else if (move_dir == 2)
				{
					game->m_bandas.m_logic.move.dir_vec = glm::vec2(0, 1);
				}
				else if (move_dir == 3)
				{
					game->m_bandas.m_logic.move.dir_vec = glm::vec2(0, -1);
				}
				game->m_bandas.m_board.set_animTimer(game->m_bandas.m_logic);
			}
			else if (type == "card")
			{
				message = message.substr(5);
				int next_token = message.find_first_of('.');
				std::string card_id_str = message.substr(0, next_token);
				message = message.substr(next_token + 1);
				next_token = message.find_first_of('.');
				std::string green_str = message.substr(0, next_token);
				message = message.substr(next_token + 1);
				next_token = message.find_first_of('.');
				std::string card_index_str = message.substr(0, next_token);
				message = message.substr(next_token + 1);
				next_token = message.find_first_of('.');
				int effect_destination = std::atoi(message.substr(0, next_token).data());
				int card_id = std::atoi(card_id_str.data());
				bool green = std::atoi(green_str.data()) == 1;
				int card_index = std::atoi(card_index_str.data());
				game->m_bandas.m_advertiser.m_green = green;
				game->m_bandas.m_advertiser.m_show = (card_id == 1 || card_id == 4 || card_id == 8) ? false : true;
				game->m_bandas.m_advertiser.m_index = card_id;
				if (!green) {
					if (game->m_bandas.m_enemy.m_team == 0) {
						game->m_bandas.m_orange_cards[card_index].m_selected = true;
					}
					else {
						game->m_bandas.m_banana_cards[card_index].m_selected = true;
					}
				}
				if (card_id == 4) {
					game->m_bandas.m_logic.card_effect.disorder_destination = effect_destination;
					game->m_bandas.process_card_effect(true);
				}
				else {
					game->m_bandas.process_card_effect(false);
				}
			}
			else if (type == "end")
			{
				game->m_bandas.m_logic.game_is_finished = true;
			}
		}
		else if (game->m_bandas.m_net.m_event.type == ENET_EVENT_TYPE_DISCONNECT)
		{
			game->m_bandas.lost_server_connection();
			game->m_bandas.m_net.connection_lost();
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
			game->m_bandas.update_game_page(client->m_mouseData, client->m_userInputs, std::string(client->m_textInput), delta);
		}
		game->updateSceneActiveCameraView(game->getActiveScene(), client->m_userInputs, client->m_mouseData, delta);

		// draw scene
		game->draw(delta, currentFrame, c_screen_width, c_screen_height, draw_mode, debug, debugPhysics);
		game->sound_manager();

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
