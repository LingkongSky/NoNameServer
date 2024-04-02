#include "NonameServer.h"

extern std::shared_ptr<asio2::tcp_session> host_client;


	void MultiPlayerManager::NewPlayerJoin(std::shared_ptr<asio2::tcp_session>& session_ptr) {

	// 鉴权 

	// 向主机请求地图
		ServerUtils::TCPSend(host_client, "0|world_post");

	// 开始缓存状态帧
	// 发送地图到客户端
	// 发送同步状态帧到客户端
	// 修改状态 进行状态帧的正常同步


	}
	
	
	// 初始化世界 无
	void MultiPlayerManager::MultiPlayerInitial(std::shared_ptr<asio2::tcp_session>& session_ptr) {



	}