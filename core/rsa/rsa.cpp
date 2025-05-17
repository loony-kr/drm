#include "rsa.h"

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

std::vector<unsigned char> RSAHelper::rsa_encrypt(const std::string& pem_pubkey, const std::vector<unsigned char>& aes_key) {
    BIO* bio = BIO_new_mem_buf(pem_pubkey.data(), pem_pubkey.size());
    RSA* rsa = PEM_read_bio_RSA_PUBKEY(bio, nullptr, nullptr, nullptr);
    if (!rsa) {
        unsigned long err = ERR_get_error();
        char err_buf[256];
        ERR_error_string_n(err, err_buf, sizeof(err_buf));
        throw std::runtime_error(std::string("Invalid RSA public key: ") + err_buf);
    }

    std::vector<unsigned char> encrypted(RSA_size(rsa));
    int len = RSA_public_encrypt(
        aes_key.size(),
        aes_key.data(),
        encrypted.data(),
        rsa,
        RSA_PKCS1_OAEP_PADDING
    );
    RSA_free(rsa);
    BIO_free(bio);

    if (len == -1) throw std::runtime_error("RSA encryption failed");
    encrypted.resize(len);
    return encrypted;
}