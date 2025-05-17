//
// Created by iamfiro on 25. 5. 17.
//

#ifndef HMAC_H
#define HMAC_H

#include <string>
#include <vector>

class HMACHelper {
    public:
        static std::vector<unsigned char> generate_hmac_sha256(const std::string& key, const std::string& data);
};

#endif //HMAC_H
