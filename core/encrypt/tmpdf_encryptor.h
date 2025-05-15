//
// Created by iamfiro on 25. 5. 15.
//

#ifndef TMPDF_ENCRYPTOR_H
#define TMPDF_ENCRYPTOR_H

#include <string>
#include <vector>

class TMPDFEncryptor {
    public:
        static const size_t AES_KEY_SIZE = 32; // AES-256
        static const size_t IV_SIZE = 12;
        static const size_t TAG_SIZE = 16;

        TMPDFEncryptor() = default;
        ~TMPDFEncryptor() = default;

        // PDF를 AES-GCM 기법을 사용해 tmpdf로 암호화하는 함수
        void encrypt(const std::string& input_tmpdf_path, const std::string& output_tmpdf_path, const std::string& key_file);

    private:
        // 파일 읽기/쓰기용 함수
        void read_file(const std::string& path, std::vector<unsigned char>& buffer);
        void write_file(const std::string& path, const std::vector<unsigned char>& buffer);

        // AES-GCM으로 암호화 하는 함수
        std::vector<unsigned char> encrypt_aes_gcm(
            const std::vector<unsigned char>& data,
            const std::vector<unsigned char>& key,
            std::vector<unsigned char>& iv,
            std::vector<unsigned char>& tag
        );
};


#endif //TMPDF_ENCRYPTOR_H
