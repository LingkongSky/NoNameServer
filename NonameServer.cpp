﻿#include "NonameServer.h"
#include <memory>
#include <string_view>


std::vector<std::shared_ptr<asio2::tcp_session>> clients;
std::vector<std::string> client_keys;
std::shared_ptr<asio2::tcp_session> host_client;
std::string host_client_key;
class MainServer
{
public:

	// 接收请求
	void on_recv(std::shared_ptr<asio2::tcp_session>& session_ptr, std::string_view data)
	{

		// printf("recv : 长度 %zu  内容 %.*s\n", data.size(), (int)data.size(), data.data());
		// 先拆包转成数组

		// 先进行切包
		std::string content = data.data();
		std::string split = "$%&";
		std::vector<std::string> seglist = ServerUtils::StringSplit(content,split);

		std::vector<std::string> processedSeglist;

		// 再处理指令切割
		for (const std::string i : seglist) {
			processedSeglist = {};
			std::string segment;
			std::stringstream stringBuffer;
			stringBuffer << i;

			while (std::getline(stringBuffer, segment, '|')) {
				processedSeglist.push_back(segment);
			}

			//printf("content: %s\n", processedSeglist[1].c_str());
			if(i.size() > 3)
			MultiPlayerManager::CallCommand(i, processedSeglist, session_ptr);
		}

		// 将处理后的结果赋值给seglist


		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	// 连接检测
	void on_connect(std::shared_ptr<asio2::tcp_session>& session_ptr)
	{
		// 关闭TCP发包延迟
		session_ptr->no_delay(true);
		session_ptr->set_keep_alive(true);


		printf("client enter :远程地址 %s 远程端口 %u 本地地址 %s 本地端口 %u\n",
			session_ptr->remote_address().c_str(), session_ptr->remote_port(),
			session_ptr->local_address().c_str(), session_ptr->local_port());


		// 同步基本信息: 端口 uuid
		ServerUtils::TCPSend(session_ptr, "0|data_get|" + std::to_string(http_port) + "|" + std::to_string(session_ptr->hash_key()));

		try {
			// 判断是否为初次接入
			if (!host_client) {
				host_client = session_ptr;
				host_client_key = std::to_string(session_ptr->hash_key()); // 唯一标识符
				printf("主机已加入\n");
			}
			else {
				MultiPlayerManager::NewPlayerJoin(session_ptr);
			}
			
			clients.push_back(session_ptr);
			client_keys.push_back(std::to_string(session_ptr->hash_key()));

		}
		catch (...) {
		}

	}

	// 断开检测
	void on_disconnect(std::shared_ptr<asio2::tcp_session>& session_ptr)
	{

		printf("client leave : %s %u %s\n",
			session_ptr->remote_address().c_str(), session_ptr->remote_port(),
			asio2::last_error_msg().c_str());

		// 当主机断开连接时向所有客户端发送信息后断开
		if(session_ptr ==  host_client){
			host_client = NULL;
			ServerUtils::DirEmpty(tmpPath + "world");
			ServerUtils::DirEmpty(tmpPath + "download");
			ServerUtils::DirEmpty(tmpPath + "player");
			printf("主机已断开连接\n");
		}


	}

	// 服务器启动
	void on_start(asio2::tcp_server& server)
	{
		printf("Start TCP Server character : %s %u %d %s\n",
			server.listen_address().c_str(), server.listen_port(),
			asio2::last_error_val(), asio2::last_error_msg().c_str());
	}

	//服务器终止
	void on_stop(asio2::tcp_server& server)
	{
		printf("stop TCP server character : %s %u %d %s\n",
			server.listen_address().c_str(), server.listen_port(),
			asio2::last_error_val(), asio2::last_error_msg().c_str());

	}

};


int main()
{
	printf("Run NoName Server on version %s\n",version.c_str());

	// 创建文件夹
	std::string dirs[] = { tmpPath, tmpPath + "world",tmpPath + "player",tmpPath + "download","logs"};

	//int dir_count = dirs->length();

	for (int i = 0; i < 5; i++) {
	if (!std::filesystem::exists(dirs[i]))
		std::filesystem::create_directory(dirs[i]);
	}

	ServerUtils::UtilsInitial();

	MainServer listener;

	tcp_server
		.bind_recv(&MainServer::on_recv, listener) // by reference
		.bind_connect(&MainServer::on_connect, &listener) // by pointer
		.bind_disconnect(&MainServer::on_disconnect, &listener)
		.bind_start(std::bind(&MainServer::on_start, &listener, std::ref(tcp_server))) //     use std::bind
		.bind_stop(&MainServer::on_stop, listener, std::ref(tcp_server)); // not use std::bind


	// Split data
	tcp_server.start(tcp_host, tcp_port);

	start_server();

	while (std::getchar() != '\n');

	return 0;
}
