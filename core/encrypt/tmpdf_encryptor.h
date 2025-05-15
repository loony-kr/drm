//
// Created by iamfiro on 25. 5. 15.
//

#ifndef TMPDF_ENCRYPTOR_H
#define TMPDF_ENCRYPTOR_H

#include <string>
#include <vector>

// 파일 읽기용 함수
void read_file(const std::string& path, std::vector<unsigned char>& buffer);

// 파일 쓰기용 함수
void write_file(const std::string& path, const std::vector<unsigned char>& buffer);

// AES-GCM으로 암호화 하는 함수
std::vector<unsigned char> encrypt_aes_gcm(
    const std::vector<unsigned char>& data,
    const std::vector<unsigned char>& key,
    std::vector<unsigned char>& iv,
    std::vector<unsigned char>& tag
);

// PDF를 AES-GCM 기법을 사용해 tmpdf로 암호화하는 함수
void tmpdf_encrypt(const std::string& input_tmpdf_path, const std::string& output_tmpdf_path, const std::string& key_file);

#endif //TMPDF_ENCRYPTOR_H
