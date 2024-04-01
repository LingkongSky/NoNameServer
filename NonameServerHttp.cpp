#include "NonameServer.h"

void start_server() {

    using namespace httplib;

    Server svr;

    std::string host = "127.0.0.1";
    int port = 4156;


    svr.Get("/hi", [](const Request& req, Response& res) {
        res.set_content("Hello World!", "text/plain");
        });


    // 文件下载
    svr.Get("/download/:id", [](const httplib::Request& req, httplib::Response& res) {
        std::cerr << "Server-log: download\t" << req.path_params.at("id") << "\t" << req.get_header_value("Content-Type") << std::endl;
        res.set_header("Cache-Control", "no-cache");
        res.set_header("Content-Disposition", "attachment; filename=hello.txt");
        res.set_chunked_content_provider(
            "multipart/form-data", [](size_t offset, httplib::DataSink& sink) {
                const char arr[] = "hello world";
                auto ret = sink.write(arr + offset, sizeof(arr));
                sink.done();
                std::cerr << "\tdownload write:" << sizeof(arr) << std::endl;
                return !!ret;
            });

        });






    
    // 文件上传
    svr.Post("/upload", [](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader& content_reader) {
        std::cerr << "Server-log: upload\t" << req.get_header_value("Content-Type") << std::endl;
        // 二进制数据可以用：multipart/form-data 和 application/octet-stream
        if (req.is_multipart_form_data()) {// 存在流式文件

            MultipartFormData uuid = req.get_file_value("uuid");
            MultipartFormData content_file = req.get_file_value("file");
            std::string file_name = content_file.filename;

            printf("%s", file_name.c_str());
            /*
                    cout << "image file length: " << image_file.content.length() << endl
             << "image file name: " << image_file.filename << endl
             << "text file length: " << text_file.content.length() << endl
             << "text file name: " << text_file.filename << endl;
            */

            httplib::MultipartFormDataItems files;
            // 先拿到 file 信息，再流式读取
            content_reader(
                [&](const httplib::MultipartFormData& file) {
                    files.push_back(file);
                    std::cerr << "\tupload read " << file.filename << "\t" << file.content << std::endl;
                    printf("bccb");


                    return true;
                },
                [&](const char* data, size_t data_length) {
                    files.back().content.append(data, data_length);

                    std::ofstream file1("aaa", std::ios::binary);
                    file1.write(data, data_length);
                    file1.close();
                    std::cerr << "\tupload read:" << data_length << std::endl;
                    printf("baab");

                    return true;
                }
            );









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
        res.set_content(R"({"message":"upload result"})", "appliation/json");
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
    printf("Start HTTP Server on %s:%d", host.c_str(), port);

    svr.listen(host, port);


}