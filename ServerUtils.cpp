#include "NonameServer.h"
#include <string>

long bufSize = 10 * 1024;	//��������С
char* buffer;	//�����������ļ�����

void ServerUtils::TCPSend(std::shared_ptr<asio2::tcp_session>& session_ptr, std::string content) {

	session_ptr->async_send(content + "$%&");

}