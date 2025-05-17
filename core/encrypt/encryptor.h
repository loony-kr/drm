//
// Created by iamfiro on 25. 5. 15.
//

#ifndef DRM_ENCRYPTOR_H
#define DRM_ENCRYPTOR_H

#include <string>
#include <vector>
#include <core/base/baseDRM.h>
#include <core/database/database.h>

class DRMEncryptor: public BaseDRM {
    public:
        DRMEncryptor();
        ~DRMEncryptor() = default;

        // PDF를 AES-GCM 기법을 사용해 tmpdf로 암호화하는 함수
        void encrypt(const std::string& file_path);

    private:
        // AES-GCM으로 암호화 하는 함수
        std::vector<unsigned char> encrypt_aes_gcm(
            const std::vector<unsigned char>& plaintext,
            const std::vector<unsigned char>& key,
            std::vector<unsigned char>& iv,
            std::vector<unsigned char>& tag
        );

        std::string generate_random_string(size_t length);
        
        std::unique_ptr<Database> db;
};

#endif //DRM_ENCRYPTOR_H
