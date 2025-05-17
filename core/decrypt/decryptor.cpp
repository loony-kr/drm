//
// Created by chosu on 25. 5. 15.
//

#include "decryptor.h"

#include <stdexcept>

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
    if (encrypted_data.size() < HEADER_SIZE + IV_SIZE + TAG_SIZE) {
        throw std::runtime_error("Encrypted data is too short");
    }

    size_t offset = HEADER_SIZE;

    // IV 추출 (12바이트)
    iv.clear();
    iv.insert(iv.end(), 
             encrypted_data.begin() + offset, 
             encrypted_data.begin() + offset + IV_SIZE);
    offset += IV_SIZE;

    // TAG 추출 (16바이트)
    tag.clear();
    tag.insert(tag.end(), 
              encrypted_data.begin() + offset, 
              encrypted_data.begin() + offset + TAG_SIZE);
    offset += TAG_SIZE;

    // 나머지 데이터를 암호문으로 추출
    ciphertext.clear();
    ciphertext.insert(ciphertext.end(), 
                     encrypted_data.begin() + offset, 
                     encrypted_data.end());
}