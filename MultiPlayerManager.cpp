#include "NonameServer.h"

extern std::shared_ptr<asio2::tcp_session> host_client;


	void MultiPlayerManager::NewPlayerJoin(std::shared_ptr<asio2::tcp_session>& session_ptr) {

		// 向主机端请求世界副本
		ServerUtils::TCPSend(host_client, "0|world_post|" + std::to_string(session_ptr->hash_key()));


		// 向客户发送TCP以获取人物信息
		//ServerUtils::TCPSend(session_ptr, "0|player_get");




	}
	
	
	void MultiPlayerManager::MultiPlayerInitial(std::shared_ptr<asio2::tcp_session>& session_ptr) {



	}