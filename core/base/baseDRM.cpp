//
// Created by iamfiro on 25. 5. 16.
//

#include "baseDRM.h"

constexpr size_t BaseDRM::AES_KEY_SIZE;
constexpr size_t BaseDRM::IV_SIZE;
constexpr size_t BaseDRM::TAG_SIZE;
constexpr size_t BaseDRM::HEADER_SIZE;
constexpr size_t BaseDRM::VERSION;
static constexpr const char* OUTPUT_PATH = "pdf/";

const std::array<unsigned char, 6> BaseDRM::HEADER_DATA = {'T', 'M', 'P', 'D', 'F', '\0'};