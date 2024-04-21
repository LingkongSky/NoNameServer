#include "NonameServer.h"
#include <map>

extern std::shared_ptr<asio2::tcp_session> host_client;
typedef void (*FunPt)(std::vector<std::string>, std::shared_ptr<asio2::tcp_session>&);

// 声明指令
void player_request(std::vector<std::string> data, std::shared_ptr<asio2::tcp_session>& session_ptr);
void move(std::vector<std::string> data,std::shared_ptr<asio2::tcp_session>& session_ptr);

// 声明指令map
std::map<std::string,FunPt> command_map = {
	{"player_request",player_request},
	{"move",move}
};


// 新玩家加入
void MultiPlayerManager::NewPlayerJoin(std::shared_ptr<asio2::tcp_session>& session_ptr) {

	// 向主机端请求世界副本
	ServerUtils::TCPSend(host_client, "0|world_post|" + std::to_string(session_ptr->hash_key()));

}
	
	
// 多人游戏初始化
void MultiPlayerManager::MultiPlayerInitial(std::shared_ptr<asio2::tcp_session>& session_ptr) {

}


//解析传入命令并执行  0|player_get|player_id
void MultiPlayerManager::CallCommand(std::vector<std::string> seglist, std::shared_ptr<asio2::tcp_session>& session_ptr){

	if (seglist[0] == "0"){
		auto func = command_map[seglist[1]];
		func(seglist, session_ptr);
	}

}

//0|player_get|player_id
void player_request(std::vector<std::string> data, std::shared_ptr<asio2::tcp_session>& session_ptr){
	printf("PlayerRequest: %s\n",data[2].c_str());
	ServerUtils::TCPSend(host_client, "0|player_post|" + data[2]);

}

//0|move|Lingkong|0,0
void move(std::vector<std::string> data, std::shared_ptr<asio2::tcp_session>& session_ptr){

	ServerUtils::TCPBoardCastExcept(std::to_string(session_ptr->hash_key()),"0|player_move|" + data[2] + "|" + data[3] + "|" + data[4]);

}
