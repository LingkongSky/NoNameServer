// NonameServer.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once
#include <asio2/tcp/tcp_session.hpp>
#include <asio2/tcp/tcp_server.hpp>
#include <iostream>




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