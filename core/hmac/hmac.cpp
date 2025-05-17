//
// Created by iamfiro on 25. 5. 17.
//

#include "hmac.h"

#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/err.h>

std::vector<unsigned char> HMACHelper::generate_hmac_sha256(const std::string& master_key, const std::string& salt) {
    unsigned char* hmac_result = HMAC(EVP_sha256(), 
        master_key.c_str(), master_key.size(),
        reinterpret_cast<const unsigned char*>(salt.c_str()), salt.size(),
        nullptr, nullptr);

    if (!hmac_result) {
        throw std::runtime_error("HMAC_SHA256 failed: " + std::string(ERR_error_string(ERR_get_error(), nullptr)));
    }

    return std::vector<unsigned char>(hmac_result, hmac_result + EVP_MD_size(EVP_sha256()));
}