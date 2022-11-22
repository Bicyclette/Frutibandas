#ifndef COM_HPP
#define COM_HPP

#include <mutex>
#include <condition_variable>
#include <queue>

inline std::queue<std::string> g_msg2server;
inline std::mutex g_msg2server_mtx;
inline std::mutex g_advertiser_mtx;
inline std::mutex g_connected_players_mtx;
inline std::condition_variable g_cv_connect_leave;
inline bool g_try2connect = false;
inline bool g_leave_game = false;

#endif
