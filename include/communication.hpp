#ifndef COM_HPP
#define COM_HPP

#include <mutex>
#include <queue>

inline std::queue<std::string> g_msg2server;
inline std::mutex g_msg2server_mtx;
inline std::mutex g_advertiser_mtx;

#endif