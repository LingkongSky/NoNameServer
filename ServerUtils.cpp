#include "NonameServer.h"
#include <cstdlib>
#include <cstring>

std::string zip_tool;

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


void ServerUtils::UnpackZip(std::string sourcePath,std::string targetPath) {
    const char* path = "";
           // system((zip_tool + " x -y " + sourcePath + " -otmp/player/ 2>&1 >> logs/7zlogs.txt").c_str());

    system((zip_tool + " x -y " + sourcePath + " -o" + targetPath +" 2>&1 >> logs/7zlogs.txt").c_str());
    
           // system((zip_tool + " x -y " + sourcePath + " -otmp\\player\\ * >> logs\\7zlogs.txt").c_str());

}

