//
// Created by chosu on 25. 5. 17.
//

#include <vector>
#include <string>

#ifndef RSA_H
#define RSA_H

class RSAHelper {
    public:
        std::vector<unsigned char> rsa_encrypt(const std::string& pem_pubkey, const std::vector<unsigned char>& aes_key);
};

#endif