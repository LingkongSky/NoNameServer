// NonameServer.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once
#include <asio2/tcp/tcp_session.hpp>
#include <asio2/tcp/tcp_server.hpp>
#include "cpp-httplib-0.15.3/httplib.h"
#include <iostream>
#include<vector>


// HTTP服务器相关配置
static httplib::Server http_server;
static std::string http_host = "127.0.0.1";
static int http_port = 4156;


// TCP服务器相关配置
static asio2::tcp_server tcp_server;
static std::string_view tcp_host = "127.0.0.1";
static std::string_view tcp_port = "4155";


class MultiPlayerManager
{
public:
	static void NewPlayerJoin(std::shared_ptr<asio2::tcp_session>& session_ptr); // 新用户加入
	static void MultiPlayerInitial(std::shared_ptr<asio2::tcp_session>& session_ptr); // 多人游戏初始化
};

class ServerUtils {

public:
	static void TCPSend(std::shared_ptr<asio2::tcp_session>& session_ptr, std::string content);

private:


};

void start_server();
