//
// Created by iamfiro on 25. 5. 16.
//

#ifndef DRM_DATABASE_H
#define DRM_DATABASE_H

#include <string>
#include <sqlite3.h>
#include <memory>

class Database {
public:
    Database();
    ~Database();

    // 파일 매핑 저장
    void save_file_mapping(const std::string& original_name, const std::string& hashed_name);
    
    // 해시된 이름으로 원본 파일 이름 조회
    std::string get_original_name(const std::string& hashed_name);
    
    // 원본 파일 이름으로 해시된 이름 조회
    std::string get_hashed_name(const std::string& original_name);

private:
    void init_database();
    void create_tables();

    sqlite3* db;
    static constexpr const char* DB_PATH = "./database/files.db";
};

#endif //DRM_DATABASE_H
