#include "NonameServer.h"


	void MultiPlayerManager::NewPlayerJoin(std::shared_ptr<asio2::tcp_session>& session_ptr) {

	// 鉴权 

	// 向主机请求地图
		ServerUtils::TCPSend(session_ptr, "0|world_get|" + std::to_string(session_ptr->hash_key()) );



	// 开始缓存状态帧
	// 发送地图到客户端
	// 发送同步状态帧到客户端
	// 修改状态 进行状态帧的正常同步


	}
	
	
	// 初始化世界 无
	void MultiPlayerManager::MultiPlayerInitial(std::shared_ptr<asio2::tcp_session>& session_ptr) {



	}