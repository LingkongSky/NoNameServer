#include "NonameServer.h"
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

		printf("recv : 长度 %zu  内容 %.*s\n", data.size(), (int)data.size(), data.data());

		//session_ptr->has_hash() 唯一标识符

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

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
				host_client_key = session_ptr->hash_key();
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
			ServerUtils::DirEmpty("tmp/world");
			ServerUtils::DirEmpty("tmp/download");
			ServerUtils::DirEmpty("tmp/player");
			printf("主机已断开连接\n");
		}


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


	// 创建缓存文件夹

	std::string dirs[] = { "tmp", "tmp/world","tmp/player","tmp/download","logs"};

	//int dir_count = dirs->length();

	for (int i = 0; i < 5; i++) {
	if (!std::filesystem::exists(dirs[i]))
		std::filesystem::create_directory(dirs[i]);
	}


	MainServer listener;

	tcp_server
		.bind_recv(&MainServer::on_recv, listener) // by reference
		.bind_connect(&MainServer::on_connect, &listener) // by pointer
		.bind_disconnect(&MainServer::on_disconnect, &listener)
		.bind_start(std::bind(&MainServer::on_start, &listener, std::ref(tcp_server))) //     use std::bind
		.bind_stop(&MainServer::on_stop, listener, std::ref(tcp_server)); // not use std::bind
	// Split data with a single character
	//server.start(host, port, '\n'); // 自动切割数据包

	// Split data with string
	tcp_server.start(tcp_host, tcp_port);



	ServerUtils::UnpackZip("D:/work/c/NonameServer/build/utils/1712561176_players.zip");
	//ServerUtils::UnpackZip("/lingkong/work/NoNameServer/build/libzip-1.10.1.zip");

	start_server();

	while (std::getchar() != '\n');

	return 0;
}
