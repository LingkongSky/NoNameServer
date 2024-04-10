#include "NonameServer.h"
#include <cstdlib>
#include <cstring>

void ServerUtils::TCPSend(std::shared_ptr<asio2::tcp_session>& session_ptr, std::string content) {
	session_ptr->async_send(content + "$%&");
}


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








void ServerUtils::UnpackZip(std::string zipPath) {
    // 解压players.zip到tmp/download

    //判断os类型
    #ifdef _WIN64
    system((".\\utils\\7z.exe x -y " + zipPath + " -otmp\\player\\ ").c_str());
    #elif 


    #endif

}

