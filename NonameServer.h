// NonameServer.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once
#include <asio2/tcp/tcp_session.hpp>
#include <asio2/tcp/tcp_server.hpp>
#include "cpp-httplib-0.15.3/httplib.h"
#include <iostream>
#include <vector>
#include <sstream>

// HTTP服务器相关配置
static httplib::Server http_server;
static std::string http_host = "127.0.0.1";
static int http_port = 4156;


// TCP服务器相关配置
static asio2::tcp_server tcp_server;
static std::string_view tcp_host = "127.0.0.1";
static std::string_view tcp_port = "4155";

static std::string version = "0.4.0";

// 获取操作系统
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

	#ifdef _WIN64 // win64
	static std::string os_type = "win64";
	#else
	static std::string os_type = "win32";
	#endif

#elif __linux__ // linux
	static std::string os_type = "linux";
#endif

// 获取cpu型号
#ifdef __i386__
	static std::string cpu_type = "i386";
	#elif defined __x86_64__
	static std::string cpu_type = "amd64";
	#elif defined __arm__
	static std::string cpu_type = "arm";
	#elif defined __aarch64__
	static std::string cpu_type = "aarch64";
#endif




class MultiPlayerManager
{
public:
	static void NewPlayerJoin(std::shared_ptr<asio2::tcp_session>& session_ptr); // 新用户加入
	static void MultiPlayerInitial(std::shared_ptr<asio2::tcp_session>& session_ptr); // 多人游戏初始化
	static void CallCommand(std::vector<std::string> seglist); // 解析传入内容
};

class ServerUtils {

public:
	static void TCPSend(std::shared_ptr<asio2::tcp_session>& session_ptr, std::string content);
	static void DirEmpty(std::filesystem::path directoryPath);
	static void UnpackZip(std::string sourcePath,std::string targetPath);
	static void UtilsInitial();

};

void start_server();
