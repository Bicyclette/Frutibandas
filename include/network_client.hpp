#ifndef NETWORK_CLIENT_HPP
#define NETWORK_CLIENT_HPP

#include <iostream>
#include <string>
#include <exception>
#include <enet/enet.h>
#include <mutex>

class NetworkClient
{
	public:
		NetworkClient();
		~NetworkClient();
		bool connect(std::string server_ip, int port);
		bool is_trying_to_connect();
		bool disconnect();
		bool has_received_data();
		void print_data();
		void send_data(std::string data);
		int service();
		bool is_connected();
		bool is_searching_game();
		void search_game(bool search);

	public:
		ENetHost* m_client;
		ENetAddress m_address; // IP & port of the server the client will connect to
		ENetPeer* m_peer; // the server the client is connected to
		ENetEvent m_event; // event received from the server
		bool m_connected;
		std::mutex mtx;
		bool m_trying_to_connect;
		std::mutex m_trying_to_connect_mtx;
		bool m_searching_game;
		std::mutex m_searching_game_mtx;
};

#endif