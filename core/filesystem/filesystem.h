//
// Created by iamfiro on 25. 5. 16.
//

#ifndef DRM_FILESYSTEM_H
#define DRM_FILESYSTEM_H

#include <string>
#include <vector>

class FileSystem {
    public:
        FileSystem() = default;
        ~FileSystem() = default;

        // 파일 읽기
        void read_file(const std::string& file_path, std::vector<unsigned char>& data);
        
        // 파일 쓰기
        void write_file(const std::string& file_path, const std::vector<unsigned char>& data);
        
        // 파일 이름 추출
        std::string get_filename(const std::string& file_path);

        // 디렉토리 생성
        void create_directory(const std::string& dir_path);
};

#endif //DRM_FILESYSTEM_H