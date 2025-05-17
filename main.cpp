//
// Created by chosu on 25. 5. 15.
//

#include <iostream>
#include <filesystem>
#include <vector>
#include "core/encrypt/encryptor.h"

namespace fs = std::filesystem;

std::vector<std::string> loadPDFsFromDirectory(const std::string& directoryPath) {
    std::vector<std::string> pdfFiles;
    
    try {
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (entry.path().extension() == ".pdf") {
                pdfFiles.push_back(entry.path().string());
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error reading directory: " << e.what() << std::endl;
    }
    
    return pdfFiles;
}

int main(int argc) {
    const std::string inputDir = "input";
    
    // input 디렉토리가 없으면 생성
    if (!fs::exists(inputDir)) {
        fs::create_directory(inputDir);
        std::cout << "Created input directory. Please place PDF files in this directory." << std::endl;
        return 0;
    }

    // PDF 파일 목록 가져오기
    std::vector<std::string> pdfFiles = loadPDFsFromDirectory(inputDir);
    
    if (pdfFiles.empty()) {
        std::cout << "No PDF files found in input directory." << std::endl;
        return 0;
    }

    // 각 PDF 파일을 DRM_Encrypt로 처리
    DRMEncryptor encryptor;
    for (const auto& pdfFile : pdfFiles) {
        try {
            encryptor.encrypt(pdfFile);
            
            // 암호화된 파일의 원본 삭제
            try {
                fs::remove(pdfFile);
                std::cout << "ㄴ Original file deleted: " << pdfFile << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Error deleting original file " << pdfFile << ": " << e.what() << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error encrypting " << pdfFile << ": " << e.what() << std::endl;
        }
    }

    return 0;
}
