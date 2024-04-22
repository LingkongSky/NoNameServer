#include "NonameServer.h"
#include <cstdlib>
#include <cstring>

extern std::vector<std::shared_ptr<asio2::tcp_session>> clients;
extern std::vector<std::string> client_keys;
extern std::shared_ptr<asio2::tcp_session> host_client;
extern std::string host_client_key;
std::string zip_tool;


// TCP���ͷ�װ �Զ���Ӽ����
void ServerUtils::TCPSend(std::shared_ptr<asio2::tcp_session>& session_ptr, std::string content) {
	session_ptr->async_send(content + "$%&");
}



// TCP�㲥 0:ALL 1:except host
void ServerUtils::TCPBoardCast(std::string content){

	for (int i = 0; i < client_keys.size(); i++) {
		ServerUtils::TCPSend(clients[i], content);
	}

}
 
void ServerUtils::TCPBoardCastExcept(std::string except_client_key, std::string content) {

	for (int i = 0; i < client_keys.size(); i++) {
		if (client_keys[i] != except_client_key) {

			// std::cerr << client_keys[i] << "  aaa  " << host_client_key << std::endl;
			ServerUtils::TCPSend(clients[i], content);
			
		}
	}
}






// ���Ŀ¼ ���������Ͽ����Ӻ������ʱ�ļ�
void ServerUtils::DirEmpty(std::filesystem::path directoryPath){

	if (std::filesystem::is_directory(directoryPath)) {
			for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
				if (entry.is_regular_file()) {
					std::filesystem::remove(entry.path());
					std::cout << "Deleted: " << entry.path().filename() << std::endl;
				}
			}
		}

}

// ���ڸ���ϵͳ�ܹ��뻷����ʼ��ZIP����
void ServerUtils::UtilsInitial(){

    printf("OS: %s CPU: %s\n",os_type.c_str(),cpu_type.c_str());

    if(os_type == "linux"){
		if(cpu_type == "i386"){
			zip_tool = "./utils/7zzs-linux-x86";
		}else if(cpu_type == "amd64"){
			zip_tool = "./utils/7zzs-linux-x64";
		}else if(cpu_type == "arm"){
			zip_tool = "./utils/7zzs-linux-arm32";
		}else if(cpu_type == "aarch64"){
			zip_tool = "./utils/7zzs-linux-aarch64";
		}else{
			printf("not found adaptive 7z on this platform");
		}

	}else{
		if(cpu_type == "amd64"){
			zip_tool = ".\\utils\\7z.exe";
		}else if(cpu_type == "i386"){
			printf("not found adaptive 7z on this platform");
		}	
	}
}


// ��ѹzip
void ServerUtils::UnpackZip(std::string sourcePath,std::string targetPath) {
    const char* path = "";
           // system((zip_tool + " x -y " + sourcePath + " -otmp/player/ 2>&1 >> logs/7zlogs.txt").c_str());

    system((zip_tool + " x -y " + sourcePath + " -o" + targetPath +" 2>&1 >> logs/7zlogs.txt").c_str());
    
           // system((zip_tool + " x -y " + sourcePath + " -otmp\\player\\ * >> logs\\7zlogs.txt").c_str());

}

std::vector<std::string> ServerUtils::StringSplit(std::string& s, std::string& separator)
	{
		int t = 1, pre = 0;
		std::vector<std::string> res;
		while (t != -1)
		{
			t = s.find(separator, pre);
			if (t != -1)
				res.push_back(s.substr(pre, t - pre));
			else
				break;
			pre = t + separator.size();
		}
		if (pre < s.size())
			res.push_back(s.substr(pre));
		return res;

	};