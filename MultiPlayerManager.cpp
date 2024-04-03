#include "NonameServer.h"

extern std::shared_ptr<asio2::tcp_session> host_client;


	void MultiPlayerManager::NewPlayerJoin(std::shared_ptr<asio2::tcp_session>& session_ptr) {

	// ��Ȩ 

	// �����������ͼ
		ServerUtils::TCPSend(host_client, "0|world_post|" + std::to_string(session_ptr->hash_key()) );

	// ��ʼ����״̬֡
	// ���͵�ͼ���ͻ���
	// ����ͬ��״̬֡���ͻ���
	// �޸�״̬ ����״̬֡������ͬ��


	}
	
	
	// ��ʼ������ ��
	void MultiPlayerManager::MultiPlayerInitial(std::shared_ptr<asio2::tcp_session>& session_ptr) {



	}