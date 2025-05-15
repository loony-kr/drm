//
// Created by chosu on 25. 5. 15.
//

#include <string>
#include <vector>
#include <openssl/evp.h>

class TMPDFDecryptor {
    public:
        static const size_t AES_KEY_SIZE = 32; // AES-256
        static const size_t IV_SIZE = 12;
        static const size_t TAG_SIZE = 16;
        static const size_t HEADER_SIZE = 7; // 'TMPDF\0' + version byte

        TMPDFDecryptor() = default;
        ~TMPDFDecryptor() = default;

        std::vector<unsigned char> decrypt(
            const std::vector<unsigned char>& encrypted_data,
            const std::vector<unsigned char>& key,
            const std::vector<unsigned char>& iv,
            const std::vector<unsigned char>& tag
        );

        bool validate_header(const std::vector<unsigned char>& data);
        void extract_components(
            const std::vector<unsigned char>& encrypted_data,
            std::vector<unsigned char>& iv,
            std::vector<unsigned char>& tag,
            std::vector<unsigned char>& ciphertext
        );
        
    private:
        std::vector<unsigned char> decrypt_aes_gcm(
            const std::vector<unsigned char>& ciphertext,
            const std::vector<unsigned char>& key,
            const std::vector<unsigned char>& iv,
            const std::vector<unsigned char>& tag
        );
};