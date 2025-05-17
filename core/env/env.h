//
// Created by iamfiro on 25. 5. 17.
//

#ifndef ENV_LOADER_H
#define ENV_LOADER_H

#include <unordered_map>
#include <string>

std::unordered_map<std::string, std::string> load_env(const std::string& filepath = ".env");

#endif // ENV_LOADER_H