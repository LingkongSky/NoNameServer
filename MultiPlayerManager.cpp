#include "NonameServer.h"


	void MultiPlayerManager::NewPlayerJoin(std::shared_ptr<asio2::tcp_session>& session_ptr) {

	// ��Ȩ 

	// �����������ͼ
		ServerUtils::TCPSend(session_ptr, "0|world_get|" + std::to_string(session_ptr->hash_key()) );



	// ��ʼ����״̬֡
	// ���͵�ͼ���ͻ���
	// ����ͬ��״̬֡���ͻ���
	// �޸�״̬ ����״̬֡������ͬ��


	}
	
	
	// ��ʼ������ ��
	void MultiPlayerManager::MultiPlayerInitial(std::shared_ptr<asio2::tcp_session>& session_ptr) {



	}