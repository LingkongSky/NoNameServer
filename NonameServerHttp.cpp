#include "NonameServer.h"
#include <time.h>
#include <filesystem>

using namespace httplib;

extern std::vector<std::shared_ptr<asio2::tcp_session>> clients;
extern std::vector<std::string> client_keys;
extern std::shared_ptr<asio2::tcp_session> host_client;
extern std::string host_client_key;

void start_server() {

	http_server.Get("/hi", [](const Request& req, Response& res) {
		res.set_content("Hello World!", "text/plain");
		});


	// 向客户端发送世界副本
	http_server.Get("/download/world", [](const httplib::Request& req, httplib::Response& res) {
		std::cerr << "Server-log: download\t" << "\t" << req.get_header_value("Content-Type") << std::endl;

		std::string uuid = "";

		for (std::multimap<std::string, std::string>::const_iterator it = req.params.begin(); it != req.params.end(); ++it)
		{
			std::string params = it->first + " = " + it->second;
			if (it->first == "uuid") {
				uuid = it->second;
				break;
			}
		}

		if (uuid == "") return;


		std::string filename = uuid + ".zip";
		std::string filepath = "tmp/world/" + filename;
		std::ifstream file(filepath, std::ios::binary);


		if (!file) {
			res.status = 404;
			res.set_content("File not found", "text/plain");
			return;
		}
		res.set_header("Cache-Control", "no-cache");
		res.set_header("Content-Disposition", "attachment; filename=world.zip");

		// 文件获取模块
		res.set_chunked_content_provider("application/octet-stream", [filepath](size_t offset, httplib::DataSink& sink) {
			std::ifstream file_reader(filepath, std::ifstream::binary | std::ifstream::in);

			if (!file_reader.good())
				return false;

			file_reader.seekg(0, file_reader.end);
			size_t file_size = file_reader.tellg();
			file_reader.seekg(0, file_reader.beg);

			if (offset >= file_size)
				return false;

			const size_t chunk_size = static_cast<size_t>(32) * 1024;

			size_t read_size = 0;
			bool last_chunk = false;
			if (file_size - offset > chunk_size) {
				read_size = chunk_size;
				last_chunk = false;
			}
			else {
				read_size = file_size - offset;
				last_chunk = true;
			}

			std::vector<char> buffer(read_size);
			file_reader.seekg(offset, file_reader.beg);
			file_reader.read(&buffer[0], read_size);
			file_reader.close();

			sink.write(&buffer[0], read_size);

			if (last_chunk)
				sink.done();

			return true;
			});

		printf("world post successful!\n");

		});



	// 向客户端发送角色数据
	http_server.Get("/download/player", [](const httplib::Request& req, httplib::Response& res) {
		std::cerr << "Server-log: download\t" << "\t" << req.get_header_value("Content-Type") << std::endl;


		std::string game_id = "";

		for (std::multimap<std::string, std::string>::const_iterator it = req.params.begin(); it != req.params.end(); ++it)
		{
			std::string params = it->first + " = " + it->second;
			if (it->first == "game_id") {
				game_id = it->second;
				break;
			}
		}

		if (game_id == "") return;

		std::string path = host_client_key + "_players/otherPlayer/" + game_id + ".player";

		// 进入player文件夹 名字为主机端id
		if (!std::filesystem::exists(path)){
			res.set_content(R"({"message":"failed: no such player"})", "appliation/json");
			return;
		}
		

		std::string filename = game_id + ".player";
		std::ifstream file(path, std::ios::binary);

		/*if (!file) {
			res.status = 404;
			res.set_content("File not found", "text/plain");
			return;
		}*/

		res.set_header("Cache-Control", "no-cache");
		res.set_header("Content-Disposition", "attachment; filename=" + filename);

		// 文件获取模块
		res.set_chunked_content_provider("application/octet-stream", [path](size_t offset, httplib::DataSink& sink) {
			std::ifstream file_reader(path, std::ifstream::binary | std::ifstream::in);

			if (!file_reader.good())
				return false;

			file_reader.seekg(0, file_reader.end);
			size_t file_size = file_reader.tellg();
			file_reader.seekg(0, file_reader.beg);

			if (offset >= file_size)
				return false;

			const size_t chunk_size = static_cast<size_t>(32) * 1024;

			size_t read_size = 0;
			bool last_chunk = false;
			if (file_size - offset > chunk_size) {
				read_size = chunk_size;
				last_chunk = false;
			}
			else {
				read_size = file_size - offset;
				last_chunk = true;
			}

			std::vector<char> buffer(read_size);
			file_reader.seekg(offset, file_reader.beg);
			file_reader.read(&buffer[0], read_size);
			file_reader.close();

			sink.write(&buffer[0], read_size);

			if (last_chunk)
				sink.done();

			return true;
			});

			printf("player post successful!\n");

			res.set_content(R"({"message":"successful: found such player"})", "appliation/json");


		});



	// 从客户端接收人物信息
	http_server.Post("/upload/player", [](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader& content_reader) {
		std::cerr << "Server-log: download\t" << "\t" << req.get_header_value("Content-Type") << std::endl;

		try {

			if (req.is_multipart_form_data()) {

				std::stringstream ss;
				ss << time(NULL);
				std::string tmp_filename = "tmp/download/" + ss.str();
				std::string file_name;
				std::string key;

				std::ofstream tmp_file(tmp_filename, std::ios::binary);
				httplib::MultipartFormDataItems files;

				content_reader(
					[&](const httplib::MultipartFormData& file) {
						files.push_back(file);

						file_name = "tmp/player/" + file.filename;

						key = file.filename;
						key = key.erase(key.size() - 4);
						return true;
					},
					[&](const char* data, size_t data_length) {
						files.back().content.append(data, data_length);
						tmp_file.write(data, data_length);
						return true;
					}
				);
				tmp_file.close();

				int move_result = rename(tmp_filename.c_str(), file_name.c_str());

				if (move_result != 0) return;
				printf("world upload successful!\n");

				// host_client_key + "_players.zip"
				// 解压zip 名字为主机端
				ServerUtils::UnpackZip(file_name,"tmp/player");

			}
			else {
				std::string body;
				content_reader([&](const char* data, size_t data_length) {
					body.append(data, data_length);
					std::cerr << "\tupload read:" << data_length << std::endl;
					return true;
					});
				std::cerr << "\tupload read " << body << std::endl;
			}
			res.set_content(R"({"message":"successful"})", "appliation/json");

		}
		catch (...) {

			res.set_content(R"({"message":"failed"})", "appliation/json");

		}

		});



	// 从主机端接收世界副本
	http_server.Post("/upload/world", [](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader& content_reader) {
		std::cerr << "Server-log: upload\t" << req.get_header_value("Content-Type") << std::endl;

		try {

			if (req.is_multipart_form_data()) {


				std::stringstream ss;
				ss << time(NULL);
				std::string tmp_filename = "tmp/download/" + ss.str();
				std::string file_name;
				std::string key;

				std::ofstream tmp_file(tmp_filename, std::ios::binary);
				httplib::MultipartFormDataItems files;


				content_reader(
					[&](const httplib::MultipartFormData& file) {
						files.push_back(file);

						file_name = "tmp/world/" + file.filename;

						key = file.filename;
						key = key.erase(key.size() - 4);
						return true;
					},
					[&](const char* data, size_t data_length) {
						files.back().content.append(data, data_length);
						tmp_file.write(data, data_length);
						return true;
					}
				);
				tmp_file.close();

				int move_result = rename(tmp_filename.c_str(), file_name.c_str());

				if (move_result != 0) return;
				printf("world upload successful!\n");
				for (int i = 0; i < client_keys.size(); i++) {
					if (client_keys[i] == key) {
						ServerUtils::TCPSend(clients[i], "0|world_get");
						break;
					}
				}



			}
			else {
				std::string body;
				content_reader([&](const char* data, size_t data_length) {
					body.append(data, data_length);
					std::cerr << "\tupload read:" << data_length << std::endl;
					return true;
					});
				std::cerr << "\tupload read " << body << std::endl;
			}
			res.set_content(R"({"message":"successful"})", "appliation/json");


		}
		catch (...) {

			res.set_content(R"({"message":"failed"})", "appliation/json");

		}
		});






	/*
	svr.Get("/stop", [&](const Request& req, Response& res) {
		svr.stop();
		});

	*/
	printf("Start HTTP Server on %s:%d\n", http_host.c_str(), http_port);

	http_server.listen(http_host, http_port);


}