//
// Created by chosu on 25. 5. 15.
//

#include "filesystem.h"

#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <sys/stat.h>
#include <sys/types.h>

void FileSystem::read_file(const std::string& file_path, std::vector<unsigned char>& data) {
	std::ifstream file(file_path, std::ios::binary);
	if (!file) {
		throw std::runtime_error("Cannot open file: " + file_path);
	}

	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(0, std::ios::beg);

	data.resize(size);
	file.read(reinterpret_cast<char*>(data.data()), size);
}

void FileSystem::write_file(const std::string& file_path, const std::vector<unsigned char>& data) {
	std::ofstream file(file_path, std::ios::binary);
	if (!file) {
		throw std::runtime_error("Cannot create file: " + file_path);
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
	#ifdef _WIN32
		_mkdir(dir_path.c_str());
	#else
		mkdir(dir_path.c_str(), 0777);
	#endif
}
