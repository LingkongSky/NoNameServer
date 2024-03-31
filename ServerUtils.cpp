#include "NonameServer.h"
#include <string>

long bufSize = 10 * 1024;	//缓冲区大小
char* buffer;	//缓冲区保存文件数据

void ServerUtils::TCPSend(std::shared_ptr<asio2::tcp_session>& session_ptr, std::string content) {

	session_ptr->async_send(content + "$%&");

}