//
// Created by iamfiro on 25. 5. 17.
//

#include "random.h"

#include <random>

std::string Random::generate_random_string(size_t length) {
    const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const size_t charset_size = sizeof(charset) - 1;

    std::random_device rd; // 하드웨어 기반 난수
    std::mt19937 gen(rd()); // Mersenne Twister 엔진
    std::uniform_int_distribution<> distrib(0, charset_size - 1);

    std::string result;
    result.reserve(length);

    for (size_t i = 0; i < length; ++i) {
        result += charset[distrib(gen)];
    }

    return result;
}