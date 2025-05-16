//
// Created by chosu on 25. 5. 15.
//

#include "decryptor.h"

#include <stdexcept>

std::vector<unsigned char> DRMDecryptor::decrypt_aes_gcm(
    const std::vector<unsigned char>& ciphertext,
    const std::vector<unsigned char>& key,
    const std::vector<unsigned char>& iv,
    const std::vector<unsigned char>& tag
) {
    std::vector<unsigned char> plaintext(ciphertext.size());
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len = 0, plaintext_len = 0;

    EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr);
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, IV_SIZE, nullptr);
    EVP_DecryptInit_ex(ctx, nullptr, nullptr, key.data(), iv.data());
    
    EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size());
    plaintext_len = len;

    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, TAG_SIZE, 
                       const_cast<unsigned char*>(tag.data()));

    int final_len = 0;
    if(EVP_DecryptFinal_ex(ctx, plaintext.data() + plaintext_len, &final_len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to decrypt data");
    }

    plaintext_len += final_len;
    EVP_CIPHER_CTX_free(ctx);
    plaintext.resize(plaintext_len);

    return plaintext;
}

bool DRMDecryptor::validate_header(const std::vector<unsigned char>& data) {
    if(data.size() < DRMDecryptor::HEADER_SIZE) {
        return false;
    }

    const unsigned char expected_header[] = {'T', 'M', 'P', 'D', 'F', '\0'};

    for(size_t i = 0; i < 6; i++) {
        if(data[i] != expected_header[i]) {
            return false;
        }
    }
    
    if(data[6] != 0x01) {
        return false;
    }

    return true;
}

void DRMDecryptor::extract_components(
    const std::vector<unsigned char>& encrypted_data,
    std::vector<unsigned char>& iv,
    std::vector<unsigned char>& tag,
    std::vector<unsigned char>& ciphertext
) {
    size_t offset = DRMDecryptor::HEADER_SIZE;

    iv.assign(encrypted_data.begin() + offset, 
             encrypted_data.begin() + offset + DRMDecryptor::IV_SIZE);
    offset += IV_SIZE;

    tag.assign(encrypted_data.begin() + offset, 
              encrypted_data.begin() + offset + DRMDecryptor::TAG_SIZE);
    offset += TAG_SIZE;

    ciphertext.assign(encrypted_data.begin() + offset, 
                     encrypted_data.end());
}

std::vector<unsigned char> DRMDecryptor::decrypt(
    const std::vector<unsigned char>& encrypted_data,
    const std::vector<unsigned char>& key,
    const std::vector<unsigned char>& iv,
    const std::vector<unsigned char>& tag
) {
    if(!validate_header(encrypted_data)) {
        throw std::runtime_error("Invalid file format");
    }

    std::vector<unsigned char> extracted_iv, extracted_tag, ciphertext;
    extract_components(encrypted_data, extracted_iv, extracted_tag, ciphertext);

    return decrypt_aes_gcm(ciphertext, key, extracted_iv, extracted_tag);
}