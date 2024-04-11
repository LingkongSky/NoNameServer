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
    const char* path = "";
    //判断os类型 先判断平台 再判断架构
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

        #ifdef _WIN64 // win64
        system((".\\utils\\7z.exe x -y " + zipPath + " -otmp\\player\\ * >> logs\\7zlogs.txt").c_str() );
        #endif

    #elif __linux__ // linux

        #ifdef __i386__
        system(("./utils/7zzs-linux-x86 x -y " + zipPath + " -otmp/player/ 2>&1 >> logs/7zlogs.txt").c_str());
        #elif defined __x86_64__
        system(("./utils/7zzs-linux-x64 x -y " + zipPath + " -otmp/player/ 2>&1 >> logs/7zlogs.txt").c_str());
        #elif defined __arm__
        system(("./utils/7zzs-linux-arm32 x -y " + zipPath + " -otmp/player/ 2>&1 >> logs/7zlogs.txt").c_str());
        #elif defined __aarch64__
        system(("./utils/7zzs-linux-aarch64 x -y " + zipPath + " -otmp/player/ 2>&1 >> logs/7zlogs.txt").c_str());
        #endif

    #endif


}

