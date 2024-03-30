#include "NonameServer.h"




void ServerUtils::TCPSend(std::shared_ptr<asio2::tcp_session>& session_ptr, std::string content) {



	session_ptr->async_send(content);



}

