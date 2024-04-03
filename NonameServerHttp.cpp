#include "NonameServer.h"
#include <time.h>
#include <filesystem>

using namespace httplib;

extern std::vector<std::shared_ptr<asio2::tcp_session>> clients;
extern std::vector<std::string> client_keys;
extern std::shared_ptr<asio2::tcp_session> host_client;

void start_server() {

    // �����ļ���
    if (!std::filesystem::exists("tmp"))
        std::filesystem::create_directory("tmp");

    if (!std::filesystem::exists("world"))
        std::filesystem::create_directory("world");




    http_server.Get("/hi", [](const Request& req, Response& res) {
        res.set_content("Hello World!", "text/plain");
        });





    // �ļ�����

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
            // ���ļ�
            std::ifstream file_reader(filepath, std::ifstream::binary | std::ifstream::in);

            // �޷����ļ���ȡ������
            if (!file_reader.good())
                return false;

            // ��ȡ�ļ���С
            file_reader.seekg(0, file_reader.end);
            size_t file_size = file_reader.tellg();
            file_reader.seekg(0, file_reader.beg);

            // ���ƫ�������ļ���С�����ƫ�������ڵ����ļ���С����ȡ������
            if (offset >= file_size)
                return false;

            // �ϴ�Ŀ��С�ܹ���ø���������ٶȣ�����ʹ�ø����ڴ桢�����ʹ���I/O
            const size_t chunk_size = 32 * 1024;

            // ׼��Ҫ��ȡ�Ŀ�Ĵ�С
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

            // ������ʱ�������������ļ�����뻺����
            std::vector<char> buffer(read_size);
            file_reader.seekg(offset, file_reader.beg);
            file_reader.read(&buffer[0], read_size);
            file_reader.close();

            // ��������������д��sink
            sink.write(&buffer[0], read_size);

            // �����һ����д��sink�����
            if (last_chunk)
                sink.done();

            return true;
            });

        printf("world post successful!\n");

        });



    http_server.Get("/download/player", [](const httplib::Request& req, httplib::Response& res) {
        std::cerr << "Server-log: download\t" << "\t" << req.get_header_value("Content-Type") << std::endl;
        

        });



    // �ļ��ϴ�
    http_server.Post("/upload/world", [](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader& content_reader) {

        std::cerr << "Server-log: upload\t" << req.get_header_value("Content-Type") << std::endl;

        try {

        // ���������ݿ����ã�multipart/form-data �� application/octet-stream
        if (req.is_multipart_form_data()) {// ������ʽ�ļ�

            //���ж����� �ٽ��н���


                std::stringstream ss;
                ss << time(NULL);
                std::string tmp_filename = "tmp/" + ss.str();// ��ʱ�ļ���
                std::string file_name;// �ļ���
                std::string key;

                std::ofstream tmp_file(tmp_filename, std::ios::binary);
                httplib::MultipartFormDataItems files;


                // ���õ� file ��Ϣ������ʽ��ȡ
                content_reader(
                    [&](const httplib::MultipartFormData& file) {
                        files.push_back(file);
                        // std::cerr << "\tupload read " << file.filename << "\t" << file.content << std::endl;
                        file_name = "world/" + file.filename;
                
                        key = file.filename;// ��ȡkey
                        key = key.erase(key.size() - 4);// ɾ����׺
                 
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

                int move_result = rename(tmp_filename.c_str(), file_name.c_str());// ���ļ���tmp����world�ļ���

                if (move_result != 0) return; // �ļ��ƶ�ʧ��
                
                    // ����ɹ� ֪ͨtcp��������ͻ��˷�������
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