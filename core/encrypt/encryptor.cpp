//
// Created by chosu on 25. 5. 15.
//

#include "encryptor.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/hmac.h>

#include <core/env/env.h>
#include <core/hmac/hmac.h>
#include <core/random/random.h>
#include <core/filesystem/filesystem.h>

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

	// Finalize encryption
	int final_len = 0;
	EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &final_len);
	ciphertext_len += final_len;

	// Get the tag after encryption is complete
	tag.resize(TAG_SIZE);
	EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, TAG_SIZE, tag.data());

	EVP_CIPHER_CTX_free(ctx);
	ciphertext.resize(ciphertext_len);

	return ciphertext;
}

void DRMEncryptor::encrypt(const std::string& file_path) {
	FileSystem fs;
	HMACHelper hmac;
	Random rand;

	std::vector<unsigned char> original_pdf_data, iv, tag;

	// 암호화 되지 않은 PDF 읽기
	fs.read_file(file_path, original_pdf_data);
	
	// 원본 파일 이름 추출 및 해시된 이름 생성
	std::string original_name = fs.get_filename(file_path);
	std::string file_name_hashed = rand.generate_random_string(16);

	// 암호화 키 생성
	std::string master_key = load_env()["MASTER_KEY"];
	std::vector<unsigned char> secret_key = hmac.generate_hmac_sha256(master_key, file_name_hashed);
	auto ciphertext = encrypt_aes_gcm(original_pdf_data, secret_key, iv, tag);
	
	// 복호화 시 원본 파일 이름을 찾을 수 있게 sqlite3 데이터베이스에 저장
	db->save_file_mapping(original_name, file_name_hashed);

	// tmpdf 파일 생성 (암호화 된 텍스트에 헤더 추가)
	std::vector<unsigned char> output;
	output.insert(output.end(), HEADER_DATA.begin(), HEADER_DATA.end());
	output.push_back(VERSION);
	output.insert(output.end(), iv.begin(), iv.end());
	output.insert(output.end(), tag.begin(), tag.end());
	output.insert(output.end(), ciphertext.begin(), ciphertext.end());

	const std::string FINAL_OUTPUT_PATH = OUTPUT_PATH + file_name_hashed + ".tmpdf";

	fs.write_file(FINAL_OUTPUT_PATH, output);

	std::cout << "[✓] tmpdf encryption successful " << FINAL_OUTPUT_PATH << std::endl;
}