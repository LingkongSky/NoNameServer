#include "NonameServer.h"

extern std::shared_ptr<asio2::tcp_session> host_client;


	void MultiPlayerManager::NewPlayerJoin(std::shared_ptr<asio2::tcp_session>& session_ptr) {

		// ���������������縱��
		ServerUtils::TCPSend(host_client, "0|world_post|" + std::to_string(session_ptr->hash_key()));


		// ��ͻ�����TCP�Ի�ȡ������Ϣ
		//ServerUtils::TCPSend(session_ptr, "0|player_get");




	}
	
	
	void MultiPlayerManager::MultiPlayerInitial(std::shared_ptr<asio2::tcp_session>& session_ptr) {



	}