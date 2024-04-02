#include "NonameServer.h"
#include <time.h>
#include <filesystem>

using namespace httplib;

extern std::vector<std::shared_ptr<asio2::tcp_session>> clients;
extern std::vector<std::string> client_keys;
extern std::shared_ptr<asio2::tcp_session> host_client;

void start_server() {

    // 创建文件夹
    if (!std::filesystem::exists("tmp"))
        std::filesystem::create_directory("tmp");

    if (!std::filesystem::exists("world"))
        std::filesystem::create_directory("world");




    http_server.Get("/hi", [](const Request& req, Response& res) {
        res.set_content("Hello World!", "text/plain");
        });





    // 文件下载

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
        std::string filepath = "world/" + filename;
        
       //std::string filepath = "C:/Users/lingk/AppData/Roaming/Godot/app_userdata/Noname/tmp/server/world.zip";

       // const char* filepath = &dir + filename.c_str();
        std::ifstream file(filepath, std::ios::binary);
 

        if (!file) {
            res.status = 404;
            res.set_content("File not found", "text/plain");
            return;
        }
        res.set_header("Cache-Control", "no-cache");
        res.set_header("Content-Disposition", "attachment; filename=world.zip");
            

        res.set_chunked_content_provider("application/octet-stream", [filepath](size_t offset, httplib::DataSink& sink) {
            // 打开文件
            std::ifstream file_reader(filepath, std::ifstream::binary | std::ifstream::in);

            // 无法打开文件，取消处理
            if (!file_reader.good())
                return false;

            // 获取文件大小
            file_reader.seekg(0, file_reader.end);
            size_t file_size = file_reader.tellg();
            file_reader.seekg(0, file_reader.beg);

            // 检查偏移量和文件大小，如果偏移量大于等于文件大小，则取消处理
            if (offset >= file_size)
                return false;

            // 较大的块大小能够获得更快的下载速度，但会使用更多内存、带宽和磁盘I/O
            const size_t chunk_size = 32 * 1024;

            // 准备要读取的块的大小
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

            // 分配临时缓冲区，并将文件块读入缓冲区
            std::vector<char> buffer(read_size);
            file_reader.seekg(offset, file_reader.beg);
            file_reader.read(&buffer[0], read_size);
            file_reader.close();

            // 将缓冲区的内容写入sink
            sink.write(&buffer[0], read_size);

            // 在最后一个块写入sink后完成
            if (last_chunk)
                sink.done();

            return true;
            });

        printf("world post successful!\n");

        });



    // 文件上传
    http_server.Post("/upload/world", [](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader& content_reader) {

        std::cerr << "Server-log: upload\t" << req.get_header_value("Content-Type") << std::endl;

        try {

        // 二进制数据可以用：multipart/form-data 和 application/octet-stream
        if (req.is_multipart_form_data()) {// 存在流式文件

            //先判断请求 再进行接收


                std::stringstream ss;
                ss << time(NULL);
                std::string tmp_filename = "tmp/" + ss.str();// 临时文件名
                std::string file_name;// 文件名
                std::string key;

                std::ofstream tmp_file(tmp_filename, std::ios::binary);
                httplib::MultipartFormDataItems files;


                // 先拿到 file 信息，再流式读取
                content_reader(
                    [&](const httplib::MultipartFormData& file) {
                        files.push_back(file);
                        // std::cerr << "\tupload read " << file.filename << "\t" << file.content << std::endl;
                        file_name = "world/" + file.filename;
                
                        key = file.filename;// 获取key
                        key = key.erase(key.size() - 4);// 删除后缀
                 
                        return true;
                    },
                    [&](const char* data, size_t data_length) {
                        files.back().content.append(data, data_length);
                        tmp_file.write(data, data_length);
                        //std::cerr << "\tupload read:" << data_length << std::endl;
                        return true;
                    }
                );
                tmp_file.close();

                int move_result = rename(tmp_filename.c_str(), file_name.c_str());// 将文件从tmp移至world文件夹

                if (move_result != 0) return; // 文件移动失败
                
                    // 传输成功 通知tcp服务器向客户端发送请求
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
    // Match the request path against a regular expression
    // and extract its captures
    svr.Get(R"(/numbers/(\d+))", [&](const Request& req, Response& res) {
        auto numbers = req.matches[1];
        res.set_content(numbers, "text/plain");
        });

    // Capture the second segment of the request path as "id" path param
    svr.Get("/users/:id", [&](const Request& req, Response& res) {
        auto user_id = req.path_params.at("id");
        res.set_content(user_id, "text/plain");
        });

    // Extract values from HTTP headers and URL query params
    svr.Get("/body-header-param", [](const Request& req, Response& res) {
        if (req.has_header("Content-Length")) {
            auto val = req.get_header_value("Content-Length");
        }
        if (req.has_param("key")) {
            auto val = req.get_param_value("key");
        }
        res.set_content(req.body, "text/plain");
        });

    svr.Get("/stop", [&](const Request& req, Response& res) {
        svr.stop();
        });

    */
    printf("Start HTTP Server on %s:%d\n", http_host.c_str(), http_port);

    http_server.listen(http_host, http_port);


}