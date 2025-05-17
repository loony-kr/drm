//
// Created by chosu on 25. 5. 15.
//

#include "encryptor.h"
#include <core/filesystem/filesystem.h>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/hmac.h>
#include <core/env/env.h>

DRMEncryptor::DRMEncryptor() : db(std::make_unique<Database>()) {}

std::vector<unsigned char> DRMEncryptor::encrypt_aes_gcm(
	const std::vector<unsigned char> &plaintext,
	const std::vector<unsigned char> &key,
	std::vector<unsigned char> &iv,
	std::vector<unsigned char> &tag
) {
	iv.resize(IV_SIZE);
	RAND_bytes(iv.data(), IV_SIZE);

	std::vector<unsigned char> ciphertext(plaintext.size());
	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
	int len = 0, ciphertext_len = 0;

	EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr);
	EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, IV_SIZE, nullptr);
	EVP_EncryptInit_ex(ctx, nullptr, nullptr, key.data(), iv.data());

	EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), plaintext.size());
	ciphertext_len = len;

	tag.resize(TAG_SIZE);
	EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, TAG_SIZE, tag.data());

	EVP_CIPHER_CTX_free(ctx);
	ciphertext.reserve(ciphertext_len);

	return ciphertext;
}

void DRMEncryptor::encrypt(const std::string& file_path) {
	FileSystem fs;


	std::vector<unsigned char> data, iv, tag;

	// 암호화 되지 않은 PDF 읽기
	fs.read_file(file_path, data);

	std::string file_name_hashed = generate_random_string(16);

	// 암호화 키 생성
	std::string master_key = load_env()["MASTER_KEY"];
	std::vector<unsigned char> aes_key = HMAC(master_key, file_name_hashed);
	auto ciphertext = encrypt_aes_gcm(data, aes_key, iv, tag);
	
	// 원본 파일 이름 추출
	std::string original_name = fs.get_filename(file_path);
	
	// 파일 매핑 저장
	db->save_file_mapping(original_name, file_name_hashed);

	std::vector<unsigned char> output;
	output.insert(output.end(), HEADER_DATA.begin(), HEADER_DATA.end());
	output.push_back(VERSION);
	output.insert(output.end(), iv.begin(), iv.end());
	output.insert(output.end(), tag.begin(), tag.end());
	output.insert(output.end(), ciphertext.begin(), ciphertext.end());

	std::string FINAL_OUTPUT_PATH = OUTPUT_PATH + file_name_hashed + ".tmpdf";

	fs.write_file(FINAL_OUTPUT_PATH, output);

	std::cout << "[✓] tmpdf encryption successful " << FINAL_OUTPUT_PATH << std::endl;
}

std::string DRMEncryptor::generate_random_string(size_t length) {
	const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string random_string(length, '0');
	for (size_t i = 0; i < length; ++i) {
		random_string[i] = charset[rand() % (sizeof(charset) - 1)];
	}
	return random_string;
}