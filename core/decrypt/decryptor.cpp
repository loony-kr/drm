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