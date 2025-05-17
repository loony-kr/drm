//
// Created by iamfiro on 25. 5. 17.
//

#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>

std::unordered_map<std::string, std::string> load_env(const std::string& filepath = ".env") {
    std::unordered_map<std::string, std::string> env;
    std::ifstream file(filepath);
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            env[key] = value;
        }
    }

    return env;
}