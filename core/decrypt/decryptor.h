//
// Created by chosu on 25. 5. 15.
//

#ifndef DRM_DECRYPTOR_H
#define DRM_DECRYPTOR_H

#include <string>
#include <vector>
#include <openssl/evp.h>
#include <core/base/baseDRM.h>

class DRMDecryptor: public BaseDRM {
    public:
        DRMDecryptor() = default;
        ~DRMDecryptor() = default;

        // 파일 복호화
        std::vector<unsigned char> decrypt(
            const std::vector<unsigned char>& ciphertext,
            const std::vector<unsigned char>& key,
            const std::vector<unsigned char>& iv,
            const std::vector<unsigned char>& tag
        );

        // PDF의 헤더 데이터를 검증
        bool validate_header(const std::vector<unsigned char>& data);
        void extract_components(
            const std::vector<unsigned char>& encrypted_data,
            std::vector<unsigned char>& iv,
            std::vector<unsigned char>& tag,
            std::vector<unsigned char>& extracted_ciphertext
        );
        
    private:
        std::vector<unsigned char> decrypt_aes_gcm(
            const std::vector<unsigned char>& ciphertext,
            const std::vector<unsigned char>& key,
            const std::vector<unsigned char>& iv,
            const std::vector<unsigned char>& tag
        );
};

#endif //DRM_DECRYPTOR_H