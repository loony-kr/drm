//
// Created by iamfiro on 25. 5. 17.
//

#ifndef BASE64_H
#define BASE64_H

#include <vector>
#include <string>

class Base64 {
    public:
        std::string encode(const std::vector<unsigned char>& data);
};

#endif // BASE64_H