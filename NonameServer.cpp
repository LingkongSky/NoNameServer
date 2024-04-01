#include "NonameServer.h"
#include <memory>
#include <string_view>

std::string_view host = "127.0.0.1";
std::string_view port = "4155";
asio2::tcp_server server;
std::shared_ptr<asio2::tcp_session> host_client;
std::list<std::shared_ptr<asio2::tcp_session>> clients;




class MainServer
{
public:

	// 接收请求
	void on_recv(std::shared_ptr<asio2::tcp_session>& session_ptr, std::string_view data)
	{

		printf("recv : 长度 %zu  内容 %.*s\n", data.size(), (int)data.size(), data.data());

		//session_ptr->has_hash() 唯一标识符

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	}

	// 连接检测
	void on_connect(std::shared_ptr<asio2::tcp_session>& session_ptr)
	{
		// 关闭TCP法宝延迟
		session_ptr->no_delay(true);

		printf("client enter :远程地址 %s 远程端口 %u 本地地址 %s 本地端口 %u\n",
			session_ptr->remote_address().c_str(), session_ptr->remote_port(),
			session_ptr->local_address().c_str(), session_ptr->local_port());

		
		// 判断是否为初次接入
		if (!host_client) {
			host_client = session_ptr;
		}

		MultiPlayerManager::NewPlayerJoin(session_ptr);

		ServerUtils::TCPSend(session_ptr, "Server Connected Successful");
		clients.push_back(session_ptr);
	}

	// 断开检测
	void on_disconnect(std::shared_ptr<asio2::tcp_session>& session_ptr)
	{
		printf("client leave : %s %u %s\n",
			session_ptr->remote_address().c_str(), session_ptr->remote_port(),
			asio2::last_error_msg().c_str());
	}

	// 服务器启动
	void on_start(asio2::tcp_server& server)
	{
		printf("Start Tcp Server character : %s %u %d %s\n",
			server.listen_address().c_str(), server.listen_port(),
			asio2::last_error_val(), asio2::last_error_msg().c_str());
	}

	//服务器终止
	void on_stop(asio2::tcp_server& server)
	{
		printf("stop tcp server character : %s %u %d %s\n",
			server.listen_address().c_str(), server.listen_port(),
			asio2::last_error_val(), asio2::last_error_msg().c_str());

	}

};


int main()
{
	
	MainServer listener;

	server
		.bind_recv(&MainServer::on_recv, listener) // by reference
		.bind_connect(&MainServer::on_connect, &listener) // by pointer
		.bind_disconnect(&MainServer::on_disconnect, &listener)
		.bind_start(std::bind(&MainServer::on_start, &listener, std::ref(server))) //     use std::bind
		.bind_stop(&MainServer::on_stop, listener, std::ref(server)); // not use std::bind
	// Split data with a single character
	//server.start(host, port, '\n'); // 自动切割数据包

	// Split data with string
	server.start(host, port);

	start_server();

	while (std::getchar() != '\n');
	
	return 0;
}
