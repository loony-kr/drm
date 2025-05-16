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

void DRMEncryptor::encrypt(
	const std::string& file_path,
	const std::string& key_file
) {
	FileSystem fs;

	std::vector<unsigned char> key, data, iv, tag;

	fs.read_file(key_file, key);
	if (key.size() != AES_KEY_SIZE) throw std::runtime_error("Invalid key size, AES key must be 32 length");

	fs.read_file(file_path, data);
	auto ciphertext = encrypt_aes_gcm(data, key, iv, tag);

	std::string file_name_hashed = generate_random_string(16);
	
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