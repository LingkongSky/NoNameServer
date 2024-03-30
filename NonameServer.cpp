#include <memory>
#include <string_view>
#include <asio2/tcp/tcp_session.hpp>
#include <asio2/tcp/tcp_server.hpp>

#include <fstream>
#include <codecvt> 
#include <locale>


std::string_view host = "127.0.0.1";
std::string_view port = "4155";
asio2::tcp_server server;

class svr_listener
{
public:

	void on_recv(std::shared_ptr<asio2::tcp_session>& session_ptr, std::string_view data)
	{


		printf("recv : 长度 %zu  内容 %.*s\n", data.size(), (int)data.size(), data.data());

		// this is just a demo to show :
		// even if we force one packet data to be sent twice,
		// but the client must recvd whole packet once

		session_ptr->async_send("abcdefghijklmnopqrstovuxyz0123456789");

		//session_ptr->async_send(data.substr(0, data.size() / 2));
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		//session_ptr->async_send(data.substr(data.size() / 2));
		//printf(std::string(data.substr(0, data.size() / 2)).c_str() );
	}

	void on_connect(std::shared_ptr<asio2::tcp_session>& session_ptr)
	{
		session_ptr->no_delay(true);

		printf("client enter :远程地址 %s 远程端口 %u 本地地址 %s 本地端口 %u\n",
			session_ptr->remote_address().c_str(), session_ptr->remote_port(),
			session_ptr->local_address().c_str(), session_ptr->local_port());
	}

	void on_disconnect(std::shared_ptr<asio2::tcp_session>& session_ptr)
	{
		printf("client leave : %s %u %s\n",
			session_ptr->remote_address().c_str(), session_ptr->remote_port(),
			asio2::last_error_msg().c_str());
	}

	void on_start(asio2::tcp_server& server)
	{
		printf("start tcp server character : %s %u %d %s\n",
			server.listen_address().c_str(), server.listen_port(),
			asio2::last_error_val(), asio2::last_error_msg().c_str());
	}

	void on_stop(asio2::tcp_server& server)
	{
		printf("stop tcp server character : %s %u %d %s\n",
			server.listen_address().c_str(), server.listen_port(),
			asio2::last_error_val(), asio2::last_error_msg().c_str());
	}

	void on_sned() {
	
	}


};


int main()
{

	svr_listener listener;

	// bind member function
	server
		.bind_recv(&svr_listener::on_recv, listener) // by reference
		.bind_connect(&svr_listener::on_connect, &listener) // by pointer
		.bind_disconnect(&svr_listener::on_disconnect, &listener)
		.bind_start(std::bind(&svr_listener::on_start, &listener, std::ref(server))) //     use std::bind
		.bind_stop(&svr_listener::on_stop, listener, std::ref(server)); // not use std::bind
	// Split data with a single character
	//server.start(host, port, '\n');

	// Split data with string
	server.start(host, port);

	while (std::getchar() != '\n');

	return 0;
}

