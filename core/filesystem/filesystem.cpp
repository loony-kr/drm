//
// Created by iamfiro on 25. 5. 16.
//

#include "filesystem.h"

#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <sys/stat.h>
#include <sys/types.h>
#include <sstream>

void FileSystem::read_file(const std::string& file_path, std::vector<unsigned char>& data) {
	std::ifstream file(file_path, std::ios::binary);
	if (!file) {
		throw std::runtime_error("[Error] Cannot open file: " + file_path);
	}

	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(0, std::ios::beg);

	data.resize(size);
	file.read(reinterpret_cast<char*>(data.data()), size);
}

void FileSystem::write_file(const std::string& file_path, const std::vector<unsigned char>& data) {
	// 디렉토리 경로 추출
	size_t last_slash = file_path.find_last_of("/\\");
	if (last_slash != std::string::npos) {
		std::string dir_path = file_path.substr(0, last_slash);
		create_directory(dir_path);
	}

	std::ofstream file(file_path, std::ios::binary);
	if (!file) {
		throw std::runtime_error("[Error] Cannot create file: " + file_path);
	}

	file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

std::string FileSystem::get_filename(const std::string& file_path) {
	std::string filename = file_path;
	size_t last_slash = filename.find_last_of("/\\");
	if (last_slash != std::string::npos) {
		filename = filename.substr(last_slash + 1);
	}
	return filename;
}

void FileSystem::create_directory(const std::string& dir_path) {
	std::string current_path;
	std::stringstream ss(dir_path);
	std::string dir;
	
	// 경로를 '/' 또는 '\'로 분리하여 각 디렉토리를 순차적으로 생성
	while (std::getline(ss, dir, '/')) {
		if (dir.empty()) continue;
		
		if (!current_path.empty()) {
			current_path += "/";
		}
		current_path += dir;
		
		#ifdef _WIN32
			_mkdir(current_path.c_str());
		#else
			mkdir(current_path.c_str(), 0777);
		#endif
	}
}
