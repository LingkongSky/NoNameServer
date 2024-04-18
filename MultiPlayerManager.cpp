#include "NonameServer.h"
#include <map>

extern std::shared_ptr<asio2::tcp_session> host_client;
typedef void (*FunPt)(std::vector<std::string> );

void player_request(std::vector<std::string>  data);
void move(std::vector<std::string>  data);
std::map<std::string,FunPt> command_map = {
	{"player_request",player_request},
	{"move",move}
};

	void MultiPlayerManager::NewPlayerJoin(std::shared_ptr<asio2::tcp_session>& session_ptr) {

		// 向主机端请求世界副本
		ServerUtils::TCPSend(host_client, "0|world_post|" + std::to_string(session_ptr->hash_key()));

		// 向客户发送TCP以获取人物信息
		//ServerUtils::TCPSend(session_ptr, "0|player_get");

	}
	
	
	void MultiPlayerManager::MultiPlayerInitial(std::shared_ptr<asio2::tcp_session>& session_ptr) {

	}


	// 0|player_get|player_id
	void MultiPlayerManager::CallCommand(std::vector<std::string> seglist){

		if (seglist[0] == "0"){
			auto func = command_map[seglist[1]];
			func(seglist);
		}
	}


	void player_request(std::vector<std::string> data){
		printf("PlayerRequest: %s\n",data[2].c_str());
		ServerUtils::TCPSend(host_client, "0|player_post|" + data[2]);

	}

	void move(std::vector<std::string>  data){}
