//
// Created by iamfiro on 25. 5. 16.
//

#ifndef BASE_DRM_H
#define BASE_DRM_H

#include <string>
#include <vector>
#include <array>

class BaseDRM {
    public:
        BaseDRM() = default;
        virtual ~BaseDRM() = default;
    protected:
        static constexpr size_t AES_KEY_SIZE = 32; // AES-256
        static constexpr size_t IV_SIZE = 12;
        static constexpr size_t TAG_SIZE = 16;
        static constexpr size_t HEADER_SIZE = 7; // 'TMPDF\0' + version byte
        static const std::array<unsigned char, 6> HEADER_DATA;
        static constexpr size_t VERSION = 0x01;
        static constexpr const char* OUTPUT_PATH = "pdf/";

};


#endif //BASE_DRM_H
