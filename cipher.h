#pragma once
#ifndef CIPHER_H
#define CIPHER_H

#include <string>
#include <vector>
#include <windows.h>
#include <wincrypt.h>

class AesCipher {
private:
    std::string password;
    
    bool processFile(const std::string& inputPath, const std::string& outputPath, bool isEncrypt);

public:
    AesCipher(const std::string& encryptionKey);

    bool encryptFile(const std::string& inputPath, const std::string& outputPath);
    bool decryptFile(const std::string& inputPath, const std::string& outputPath);
};

#endif
