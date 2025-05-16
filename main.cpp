//
// Created by chosu on 25. 5. 15.
//

#include <iostream>
#include <core/encrypt/encryptor.h>

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "usage: ./drpdf_encryptor --in input.pdf --key aes.key\n";
        return 1;
    }

    std::string in_file, key_file;
    for (int i = 1; i < argc; i += 2) {
        std::string flag = argv[i];
        std::string value = argv[i + 1];
        if (flag == "--in") in_file = value;
        else if (flag == "--key") key_file = value;
        else {
            std::cerr << "idk options lol: " << flag << std::endl;
            return 1;
        }
    }

    try {
        DRMEncryptor encryptor;
        encryptor.encrypt(in_file, key_file);
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
