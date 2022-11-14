#include "network_client.hpp"

NetworkClient::NetworkClient() :
	m_client(nullptr),
	m_peer(nullptr),
	m_connected(false),
	m_trying_to_connect(false),
	m_searching_game(false)
{
	if (enet_initialize() != 0)
	{
		throw std::runtime_error("An error occured while initializing ENet !");
	}
	atexit(enet_deinitialize);
	
	// create client
	m_client = enet_host_create(nullptr, 1, 1, 0, 0);
	if (m_client == nullptr)
	{
		enet_deinitialize();
		throw std::runtime_error("Error while trying to create the network client !");
	}
}

NetworkClient::~NetworkClient()
{
	enet_host_destroy(m_client);
	if(is_connected())
	{
		enet_peer_reset(m_peer);
	}
}

bool NetworkClient::is_trying_to_connect()
{
	m_trying_to_connect_mtx.lock();
	bool trying_to_connect = m_trying_to_connect;
	m_trying_to_connect_mtx.unlock();
	return trying_to_connect;
}

bool NetworkClient::connect(std::string server_ip, int port)
{
	m_trying_to_connect_mtx.lock();
	m_trying_to_connect = true;
	m_trying_to_connect_mtx.unlock();

	enet_address_set_host(&m_address, server_ip.c_str());
	m_address.port = port;

	m_peer = enet_host_connect(m_client, &m_address, 1, 0);
	if (m_peer == nullptr)
	{
		enet_host_destroy(m_client);
		enet_deinitialize();
		throw std::runtime_error("Error occured while creating the server peer connection !");
	}

	// check if the server has contacted us back
	if (enet_host_service(m_client, &m_event, 5000) > 0 &&
		m_event.type == ENET_EVENT_TYPE_CONNECT)
	{
		m_trying_to_connect_mtx.lock();
		m_trying_to_connect = false;
		m_trying_to_connect_mtx.unlock();

		mtx.lock();
		m_connected = true;
		mtx.unlock();

		return true;
	}
	else
	{
		enet_peer_reset(m_peer);

		m_trying_to_connect_mtx.lock();
		m_trying_to_connect = false;
		m_trying_to_connect_mtx.unlock();

		mtx.lock();
		m_connected = false;
		mtx.unlock();
		
		return false;
	}
}

bool NetworkClient::disconnect()
{
	enet_peer_disconnect(m_peer, 0);
	while (enet_host_service(m_client, &m_event, 5000) > 0)
	{
		switch (m_event.type)
		{
			case ENET_EVENT_TYPE_RECEIVE:
				enet_packet_destroy(m_event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				mtx.lock();
				m_connected = false;
				mtx.unlock();
				m_trying_to_connect_mtx.lock();
				m_trying_to_connect = false;
				m_trying_to_connect_mtx.unlock();
				m_searching_game_mtx.lock();
				m_searching_game = false;
				m_searching_game_mtx.unlock();
				return true;
		}
	}
	mtx.lock();
	m_connected = false;
	mtx.unlock();
	m_trying_to_connect_mtx.lock();
	m_trying_to_connect = false;
	m_trying_to_connect_mtx.unlock();
	m_searching_game_mtx.lock();
	m_searching_game = false;
	m_searching_game_mtx.unlock();
	return false;
}

void NetworkClient::connection_lost()
{
	mtx.lock();
	m_connected = false;
	mtx.unlock();
	m_trying_to_connect_mtx.lock();
	m_trying_to_connect = false;
	m_trying_to_connect_mtx.unlock();
	m_searching_game_mtx.lock();
	m_searching_game = false;
	m_searching_game_mtx.unlock();
}

bool NetworkClient::has_received_data()
{
	if (enet_host_service(m_client, &m_event, 0) > 0 && m_event.type == ENET_EVENT_TYPE_RECEIVE)
	{
		return true;
	}
	return false;
}

void NetworkClient::print_data()
{
	std::string data = reinterpret_cast<char*>(m_event.packet->data);
	std::cout << data << std::endl;
}

int NetworkClient::send_data(std::string data)
{
	ENetPacket* packet = enet_packet_create(data.c_str(), data.size() + 1, ENET_PACKET_FLAG_RELIABLE);
	return enet_peer_send(m_peer, 0, packet);
}

int NetworkClient::service()
{
	return enet_host_service(m_client, &m_event, 0);
}

bool NetworkClient::is_connected()
{
	mtx.lock();
	bool connected = m_connected;
	mtx.unlock();
	return connected;
}

void NetworkClient::search_game(bool search)
{
	m_searching_game_mtx.lock();
	m_searching_game = search;
	m_searching_game_mtx.unlock();
}

bool NetworkClient::is_searching_game()
{
	m_searching_game_mtx.lock();
	bool searching_game = m_searching_game;
	m_searching_game_mtx.unlock();
	return searching_game;
}
