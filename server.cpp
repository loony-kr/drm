#include "core/http/httplib.h"
#include "core/database/database.h"
#include "core/filesystem/filesystem.h"
#include "core/decrypt/decryptor.h"
#include "nlohmann/json.hpp"

const std::string api_version = "1";

nlohmann::json mock_auth_server() {
    return nlohmann::json::parse(R"({"ok": true, "data": {"user_id": "1234567890"}})");
};

int main() {
    httplib::Server server;
    Database db = Database();
    FileSystem fs = FileSystem();
    DRMDecryptor decryptor = DRMDecryptor();
    
    // API 인증 서버 주소
    std::string path = "/v" + api_version + "/drm/certificate";

    server.Get(path.c_str(), [&db, &fs, &decryptor](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string fileId = req.get_param_value("id");
            std::string file_exists = db.get_original_name(fileId);

            // 사용자 인증 & 메소드 검증 & body 검증
            if (!mock_auth_server()["ok"].get<bool>() || req.method != "GET" || fileId.empty() || file_exists.empty()) {
                res.status = 401;
                res.set_content("Unauthorized", "text/plain");
                return;
            }

            std::vector<unsigned char> buffer;
            try {
                fs.read_file("pdf/" + fileId + ".tmpdf", buffer);
            } catch (const std::exception& e) {
                res.status = 404;
                res.set_content("File not found", "text/plain");
                return;
            }


            if (!decryptor.validate_header(buffer)) {
                res.status = 401;
                res.set_content("Unauthorized", "text/plain");
                return;
            }

            std::vector<unsigned char> iv, tag, ciphertext;
            decryptor.extract_components(buffer, iv, tag, ciphertext);

            // JSON 응답 생성
            nlohmann::json response = {
                {"filename", file_exists},
                {"iv", nlohmann::json::binary(iv)},
                {"tag", nlohmann::json::binary(tag)},
                {"aes_key", nlohmann::json::binary(ciphertext)},
                {"expired_at", "2024-12-31T23:59:59Z"} // 만료 시간은 실제 구현에 맞게 수정 필요
            };

            res.set_content(response.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content(e.what(), "text/plain");
        }
    });

    std::cout << "Server is running on port 8080" << std::endl;
    server.listen("0.0.0.0", 8080);
    
    return 0;
}