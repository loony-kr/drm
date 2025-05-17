//
// Created by chosu on 25. 5. 15.
//

#include <chrono>
#include <iomanip>
#include <sstream>

#include "nlohmann/json.hpp"

#include "core/env/env.h"
#include "core/hmac/hmac.h"
#include "core/rsa/rsa.h"
#include "core/base64/base64.h"
#include "core/http/httplib.h"
#include "core/database/database.h"
#include "core/filesystem/filesystem.h"
#include "core/decrypt/decryptor.h"

const std::string api_version = "1";

nlohmann::json mock_auth_server() {
    return nlohmann::json::parse(R"({"ok": true, "data": {"user_id": "1234567890"}})");
};

std::string mock_public_key_server() {
    return "-----BEGIN PUBLIC KEY-----\n"
            "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAsd/N0tig8rMTrybK3Nwo\n"
            "s35EvM3jbtX0bLocabfgfulc37Kn8wbJDLYycO85flxRE/o1/detuPrOoAW1vWZ3\n"
            "oE/CDTA2o6+VjJz9LtDKnl49zVHo+61p39RyCEWzO7CysvSRTYYq0mQUE2Igf5My\n"
            "0PKzf5JgVRecBB8JGnRyPEARQA50+LGUHKbndS0CctrtuI9kCvfx406sCCH/HO/u\n"
            "OUwvIMVbs7nxVq00arKo2ytL4KrFEoG97cKbE524HtBf37NsC1b+BvwNF1kykeCo\n"
            "eJ48rWFtumFr7J4dVXWSA0i6E8azdpKhyIqQIMIDDvazLMN6wpReyqjo+wIYa4tO\n"
            "FQIDAQAB\n"
            "-----END PUBLIC KEY-----";
}

std::string get_current_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << now_ms.count();
    return ss.str();
}

void log_user_action(const std::string& action, const std::string& details) {
    std::cout << "[" << get_current_timestamp() << "] " << action << " - " << details << std::endl;
}

int main() {
    httplib::Server server;
    Database db = Database();
    FileSystem fs = FileSystem();
    DRMDecryptor decryptor = DRMDecryptor();
    RSAHelper rsa;
    HMACHelper hmac;
    Base64 base64;

    // API 인증 서버 주소
    std::string path = "/v" + api_version + "/drm/certificate";

    server.Get(path.c_str(), [&db, &fs, &decryptor, &hmac, &rsa, &base64](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string fileId = req.get_param_value("id");
            log_user_action("API_ACCESS", "GET /v" + api_version + "/drm/certificate - FileID: " + fileId);

            std::string file_exists = db.get_original_name(fileId);

            // 사용자 인증 & 메소드 검증 & body 검증
            if (!mock_auth_server()["ok"].get<bool>() || req.method != "GET" || fileId.empty() || file_exists.empty()) {
                log_user_action("AUTH_FAILED", "FileID: " + fileId + " - Unauthorized access attempt");
                res.status = 401;
                res.set_content("Unauthorized", "text/plain");
                return;
            }

            std::vector<unsigned char> buffer;
            try {
                fs.read_file("pdf/" + fileId + ".tmpdf", buffer);
            } catch (const std::exception& e) {
                log_user_action("FILE_ERROR", "Failed to read file: " + fileId + " - " + e.what());
                res.status = 404;
                res.set_content("File not found", "text/plain");
                return;
            }

            if (!decryptor.validate_header(buffer)) {
                log_user_action("VALIDATION_FAILED", "Invalid header for file: " + fileId);
                res.status = 401;
                res.set_content("Unauthorized", "text/plain");
                return;
            }

            std::vector<unsigned char> iv, tag, ciphertext;
            decryptor.extract_components(buffer, iv, tag, ciphertext);

            // 디버깅을 위한 출력
            std::cout << "Server Tag bytes: ";
            for (size_t i = 0; i < tag.size(); i++) {
                printf("%02x ", tag[i]);
            }
            std::cout << std::endl;

            std::string master_key = load_env()["MASTER_KEY"];
            std::string public_key = mock_public_key_server();
            std::vector<unsigned char> aes_key = hmac.generate_hmac_sha256(master_key, fileId);
            std::vector<unsigned char> encrypted_aes_key = rsa.rsa_encrypt(public_key, aes_key);

            // JSON 응답 생성
            nlohmann::json response = {
                {"filename", file_exists},
                {"iv", base64.encode(iv)},
                {"tag", base64.encode(tag)},
                {"aes_key", base64.encode(encrypted_aes_key)},
            };

            log_user_action("CERTIFICATE_GENERATED", "Successfully generated certificate for file: " + fileId);
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception& e) {
            log_user_action("ERROR", "Unexpected error: " + std::string(e.what()));
            res.status = 500;
            res.set_content(e.what(), "text/plain");
        }
    });

    log_user_action("SERVER_START", "DRM Server started on port 8080");
    std::cout << "Server is running on port 8080" << std::endl;
    server.listen("0.0.0.0", 8080);
    
    return 0;
}