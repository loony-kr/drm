//
// Created by chosu on 25. 5. 15.
//

#include "tmpdf_encryptor.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <openssl/evp.h>
#include <openssl/rand.h>

const size_t AES_KEY_SIZE = 32; // AES-256
const size_t IV_SIZE = 12;
const size_t TAG_SIZE = 16;

void read_file(const std::string &path, std::vector<unsigned char> &buffer) {
	std::ifstream file(path, std::ios::binary);
	if (!file) {
		throw std::runtime_error("[Error] Failed to open file: " + path);
	}

	buffer.assign((std::istreambuf_iterator<char>(file)),
	              std::istreambuf_iterator<char>());
}

void write_file(const std::string &path, const std::vector<unsigned char> &buffer) {
	std::ofstream file(path, std::ios::binary);
	if (!file) {
		throw std::runtime_error("[Error] Failed to write file: " + path);
	}
	file.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());
}

std::vector<unsigned char> encrypt_aes_gcm(
	const std::vector<unsigned char> &data,
	const std::vector<unsigned char> &key,
	std::vector<unsigned char> &iv,
	std::vector<unsigned char> &tag
) {
	iv.resize(IV_SIZE);
	RAND_bytes(iv.data(), IV_SIZE);

	std::vector<unsigned char> ciphertext(data.size());
	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
	int len = 0, ciphertext_len = 0;

	EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr);
	EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, IV_SIZE, nullptr);
	EVP_EncryptInit_ex(ctx, nullptr, nullptr, key.data(), iv.data());

	EVP_EncryptUpdate(ctx, ciphertext.data(), &len, data.data(), data.size());
	ciphertext_len = len;

	tag.resize(TAG_SIZE);
	EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, TAG_SIZE, tag.data());

	EVP_CIPHER_CTX_free(ctx);
	ciphertext.reserve(ciphertext_len);

	return ciphertext;
}

void tmpdf_encrypt(
	const std::string& input_tmpdf_path,
	const std::string& output_tmpdf_path,
	const std::string& key_file
) {
	std::vector<unsigned char> key, data, iv, tag;

	read_file(key_file, key);
	if (key.size() != AES_KEY_SIZE) throw std::runtime_error("[Error] Invalid key size, AES key must be 32 length");

	read_file(input_tmpdf_path, data);
	auto ciphertext = encrypt_aes_gcm(data, key, iv, tag);

	std::vector<unsigned char> output;
	output.insert(output.end(), {'T', 'M', 'P', 'D', 'F', '\0'});
	output.push_back(0x01); // 알고리즘 암호화 버전
	output.insert(output.end(), iv.begin(), iv.end());
	output.insert(output.end(), tag.begin(), tag.end());
	output.insert(output.end(), ciphertext.begin(), ciphertext.end());

	write_file(output_tmpdf_path, output);

	std::cout << "[✓] tmpdf encryption successful" << output_tmpdf_path << std::endl;
}