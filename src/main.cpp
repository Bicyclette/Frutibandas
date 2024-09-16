#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include "window.hpp"
#include "game.hpp"
#include "framebuffer.hpp"

#define SERVER "vps-ce89df6a.vps.ovh.net"
#define PORT 7777

void connect(std::shared_ptr<Game>& game)
{
	std::string message;
	std::unique_lock<std::mutex> lock(g_msg2server_mtx);
	g_cv_connect_leave.wait(lock, [] () -> bool {return g_try2connect || g_leave_game; });
	g_try2connect = false;
	if (!g_msg2server.empty())
	{
		message = g_msg2server.front();
		g_msg2server.pop();
	}
	else
	{
		return;
	}
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
		// ignore useless data
		message = message.substr(2);
		// send message
		game->m_bandas.m_net.send_data("card:" + message);
	}
	else if (code == 8) // get amount of connected players
	{
		game->m_bandas.m_net.send_data("cp");
	}
	else if (code == 9) // leave game
	{
		game->m_bandas.m_net.send_data("bye");
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
			if (type.compare("gs") == 0)
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
			else if (type.compare("cp") == 0)
			{
				g_connected_players_mtx.lock();
				game->m_bandas.m_num_connected_players = message.substr(3);
				g_connected_players_mtx.unlock();
			}
			else if (type.compare("gu") == 0)
			{
				game->m_bandas.enemy_gave_up();
			}
			else if (type.compare("dc") == 0)
			{
				game->m_bandas.enemy_disconnected();
			}
			else if (type.compare("gc") == 0)
			{
				std::string data = message.substr(3);
				int messageLength = data.size();
				if (messageLength > 0)
				{
					std::string part1 = data;
					std::string part2;
					int boundX = game->m_bandas.m_ui.get_page(1).get_layer(1).get_sprite(5)->get_position().x + game->m_bandas.m_ui.get_page(1).get_layer(1).get_sprite(5)->get_size().x;
					int cut_pos = -1;
					for (int i = 0; i < data.size(); ++i)
					{
						glm::vec3 cursor_shape = game->m_bandas.m_text.get_cursor_shape(data.substr(0, i), 240 + 13, 728 - 698 - 12, 1, i);
						if (cursor_shape.x >= (boundX-10)) {
							cut_pos = i;
							break;
						}
					}

					if (cut_pos != -1)
					{
						part1 = data.substr(0, cut_pos);
						part2 = data.substr(cut_pos);
						game->m_bandas.add_chat_message(part1);
						game->m_bandas.add_chat_message(part2);
					}
					else
					{
						game->m_bandas.add_chat_message(data);
					}
				}
			}
			else if (type.compare("mv") == 0)
			{
				std::string move_dir_str = message.substr(3, 1);
				std::string chrono_timer_str = message.substr(5);
				int move_dir = std::atoi(move_dir_str.data());
				float chrono_timer = std::atof(chrono_timer_str.data());
				if (game->m_bandas.m_me.m_team != game->m_bandas.m_logic.turn) {
					game->m_bandas.m_enemy.m_chrono.m_time = chrono_timer;
				}

				if (game->m_bandas.m_logic.card_effect.disorder && game->m_bandas.m_logic.turn == game->m_bandas.m_logic.card_effect.disorder_destination) {
					g_advertiser_mtx.lock();
					if (!game->m_bandas.m_advertiser.empty()) {
						for (int i = game->m_bandas.m_advertiser.size() - 1; i >= 0; --i) {
							if (game->m_bandas.m_advertiser[i].m_index == 4) {
								game->m_bandas.m_advertiser[i].m_show = true;
								game->m_bandas.m_logic.card_effect.disorder = false;
								game->m_bandas.m_logic.card_effect.disorder_destination = -1;
								game->m_bandas.remove_card(4);
								break;
							}
						}
					}
					g_advertiser_mtx.unlock();
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
				game->m_bandas.m_board.check_activate_trap(game->m_bandas.m_logic);
				game->m_bandas.m_board.set_animTimer(game->m_bandas.m_logic);
				game->m_bandas.m_logic.card_effect.solo_coords = glm::ivec2( -1, -1 );
			}
			else if (type.compare("card") == 0)
			{
				// ignore useless data
				message = message.substr(5);

				// get card identifier
				int next_token = message.find_first_of('.');
				std::string card_id_str = message.substr(0, next_token);
				int card_id = std::atoi(card_id_str.data());
				
				// get advertiser color
				message = message.substr(next_token + 1);
				next_token = message.find_first_of('.');
				std::string green_str = message.substr(0, next_token);
				bool green = std::atoi(green_str.data()) == 1;
				
				// get card index in owner's array
				message = message.substr(next_token + 1);
				next_token = message.find_first_of('.');
				std::string card_index_str = message.substr(0, next_token);
				int card_index = std::atoi(card_index_str.data());
				
				// if card has an effect delayed towards enemy, get to which player it has an effect on
				int effect_destination;
				if (card_id == 4) {
					message = message.substr(next_token + 1);
					next_token = message.find_first_of('.');
					effect_destination = std::atoi(message.substr(0, next_token).data());
				}
				// else if card is reinforcement (spawn up to 3 bandas)
				else if (card_id == 9) {
					char bandas_type = (game->m_bandas.m_logic.turn == 0) ? 'o' : 'b';
					for (int i = 0; i < 3; ++i) {
						message = message.substr(next_token + 1);
						next_token = message.find_first_of('.');
						if (next_token == std::string::npos) {
							break;
						}
						int x = std::atoi(message.substr(0, next_token).data());
						message = message.substr(next_token + 1);
						next_token = message.find_first_of('.');
						int y = std::atoi(message.substr(0, next_token).data());
						game->m_bandas.m_logic.card_effect.reinforcement[i * 2] = x;
						game->m_bandas.m_logic.card_effect.reinforcement[i * 2 + 1] = y;
					}
				}
				// else if targeted card
				else if (card_id == 0 || card_id == 6 || card_id == 7 || card_id == 8 || card_id == 10 || card_id == 11) {
					message = message.substr(next_token + 1);
					next_token = message.find_first_of('.');
					int x = std::atoi(message.substr(0, next_token).data());
					message = message.substr(next_token + 1);
					next_token = message.find_first_of('.');
					int y = std::atoi(message.substr(0, next_token).data());
					if (card_id == 0) {
						game->m_bandas.m_logic.card_effect.conversion_coords = glm::ivec2(x, y);
					}
					else if (card_id == 6) {
						game->m_bandas.m_logic.card_effect.anvil_coords = glm::ivec2(x, y);
					}
					else if (card_id == 7) {
						game->m_bandas.m_logic.card_effect.petrify_coords = glm::ivec2(x, y);
					}
					else if (card_id == 8) {
						game->m_bandas.m_logic.card_effect.trap_coords = glm::ivec2(x, y);
					}
					else if (card_id == 10) {
						game->m_bandas.m_logic.card_effect.solo_coords = glm::ivec2(x, y);
					}
					else if (card_id == 11) {
						game->m_bandas.m_logic.card_effect.cow_coords = glm::ivec2(x, y);
						game->m_bandas.m_cow.col = x;
					}
				}

				g_advertiser_mtx.lock();
				game->m_bandas.m_advertiser.emplace_back();
				game->m_bandas.m_advertiser.back().m_green = green;
				game->m_bandas.m_advertiser.back().m_show = (card_id == 1 || card_id == 4 || card_id == 8) ? false : true;
				game->m_bandas.m_advertiser.back().m_index = card_id;
				g_advertiser_mtx.unlock();

				if (card_id == 4) {
					game->m_bandas.m_logic.card_effect.disorder_destination = effect_destination;
					game->m_bandas.process_card_effect(card_id, true);
				}
				else if (card_id == 1 || card_id == 8) {
					game->m_bandas.process_card_effect(card_id, true);
				}
				else {
					game->m_bandas.process_card_effect(card_id, false);
				}
			}
			else if (type.compare("end") == 0)
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
    // start
	bool alive;
	DRAWING_MODE draw_mode{DRAWING_MODE::SOLID};
    bool debug{false};
    bool debugPhysics{false};
	game->setActiveScene(0);

	// delta
	auto epoch = std::chrono::high_resolution_clock::now();
	auto currentFrame = std::chrono::high_resolution_clock::now();
	auto lastFrame = std::chrono::high_resolution_clock::now();
	float delta{0.0f};

	while(client->isAlive())
	{
		currentFrame = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = currentFrame - lastFrame;
		delta = static_cast<float>(elapsed.count());

		if (game->m_bandas.m_writer.m_cursor.m_focus == 2)
		{
			client->checkEvents(game->m_bandas);
		}
		else
		{
			client->checkEvents(game->m_bandas, true);
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
		std::chrono::duration<double> t = currentFrame - epoch;
		double total_t = t.count();
		game->draw(delta, total_t, c_screen_width, c_screen_height, draw_mode, debug, debugPhysics);
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
