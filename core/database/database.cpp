#include "database.h"
#include <core/filesystem/filesystem.h>
#include <stdexcept>

Database::Database() {
    init_database();
}

Database::~Database() {
    if (db) {
        sqlite3_close(db);
    }
}

void Database::init_database() {
    // 데이터베이스 디렉토리 생성
    FileSystem fs;
    fs.create_directory("./database");
    
    int rc = sqlite3_open(DB_PATH, &db);
    if (rc) {
        throw std::runtime_error("Cannot open database: " + std::string(sqlite3_errmsg(db)));
    }

    create_tables();
}

void Database::create_tables() {
    const char* sql = "CREATE TABLE IF NOT EXISTS file_mappings ("
                     "original_name TEXT PRIMARY KEY,"
                     "hashed_name TEXT NOT NULL);";
    
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::string error = errMsg;
        sqlite3_free(errMsg);
        throw std::runtime_error("SQL error: " + error);
    }
}

void Database::save_file_mapping(const std::string& original_name, const std::string& hashed_name) {
    const char* sql = "INSERT INTO file_mappings (original_name, hashed_name) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
    }

    sqlite3_bind_text(stmt, 1, original_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hashed_name.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        throw std::runtime_error("Failed to save file mapping: " + std::string(sqlite3_errmsg(db)));
    }
}

std::string Database::get_original_name(const std::string& hashed_name) {
    const char* sql = "SELECT original_name FROM file_mappings WHERE hashed_name = ?;";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
    }

    sqlite3_bind_text(stmt, 1, hashed_name.c_str(), -1, SQLITE_STATIC);

    std::string result;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }

    sqlite3_finalize(stmt);
    return result;
}

std::string Database::get_hashed_name(const std::string& original_name) {
    const char* sql = "SELECT hashed_name FROM file_mappings WHERE original_name = ?;";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
    }

    sqlite3_bind_text(stmt, 1, original_name.c_str(), -1, SQLITE_STATIC);

    std::string result;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }

    sqlite3_finalize(stmt);
    return result;
} 