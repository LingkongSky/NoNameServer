#include "NonameServer.h"
#include <map>

extern std::shared_ptr<asio2::tcp_session> host_client;
extern std::string host_client_key;

typedef void (*FunPt)(std::string,std::vector<std::string>, std::shared_ptr<asio2::tcp_session>&);

// 声明指令
void player_request(std::string data,std::vector<std::string> data_array, std::shared_ptr<asio2::tcp_session>& session_ptr);
void move(std::string data, std::vector<std::string> data_array, std::shared_ptr<asio2::tcp_session>& session_ptr);
void block_set(std::string data, std::vector<std::string> data_array, std::shared_ptr<asio2::tcp_session>& session_ptr);
void player_position_sync(std::string data,std::vector<std::string> data_array, std::shared_ptr<asio2::tcp_session>& session_ptr);

// 声明指令map
std::map<std::string,FunPt> command_map = {
	{"player_request",player_request},
	{"move",move},
	{"player_position_sync",player_position_sync},
	{"block_set",block_set},
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
void MultiPlayerManager::CallCommand(std::string data,std::vector<std::string> seglist, std::shared_ptr<asio2::tcp_session>& session_ptr){

	if (seglist[0] == "0"){
		auto func = command_map[seglist[1]];
		func(data,seglist, session_ptr);
	}

}

//0|player_get|player_id
void player_request(std::string data,std::vector<std::string> data_array, std::shared_ptr<asio2::tcp_session>& session_ptr){
	ServerUtils::TCPSend(host_client, "0|player_post|" + data_array[2]);

}

//0|move|Lingkong|0,0
void move(std::string data,std::vector<std::string> data_array, std::shared_ptr<asio2::tcp_session>& session_ptr){

	ServerUtils::TCPBoardCastExcept(std::to_string(session_ptr->hash_key()),"0|player_move|" + data_array[2] + "|" + data_array[3] + "|" + data_array[4]);
}

//0|player_position_sync|lingkong|1|2
void player_position_sync(std::string data,std::vector<std::string> data_array, std::shared_ptr<asio2::tcp_session>& session_ptr){
	ServerUtils::TCPBoardCastExcept(host_client_key,data);
}

//0|block_set|block_id|x|y
void block_set(std::string data, std::vector<std::string> data_array, std::shared_ptr<asio2::tcp_session>& session_ptr) {
	ServerUtils::TCPBoardCastExcept(std::to_string(session_ptr->hash_key()),data);
}
