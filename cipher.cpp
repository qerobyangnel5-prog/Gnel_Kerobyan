#include "cipher.h"
#include <fstream>
#include <iostream>

#pragma comment(lib, "advapi32.lib")

AesCipher::AesCipher(const std::string& encryptionKey) : password(encryptionKey) {}

bool AesCipher::processFile(const std::string& inputPath, const std::string& outputPath, bool isEncrypt) {
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    HCRYPTKEY hKey = 0;
  
    if (!CryptAcquireContext(&hProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        std::cerr << "Error: CryptAcquireContext failed." << std::endl;
        return false;
    }

    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
        std::cerr << "Error: CryptCreateHash failed." << std::endl;
        CryptReleaseContext(hProv, 0);
        return false;
    }

    if (!CryptHashData(hHash, (BYTE*)password.c_str(), password.length(), 0)) {
        std::cerr << "Error: CryptHashData failed." << std::endl;
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }

    if (!CryptDeriveKey(hProv, CALG_AES_256, hHash, 0, &hKey)) {
        std::cerr << "Error: CryptDeriveKey failed." << std::endl;
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }

    std::ifstream inFile(inputPath, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error: Cannot open input file: " << inputPath << std::endl;
        CryptDestroyKey(hKey); CryptDestroyHash(hHash); CryptReleaseContext(hProv, 0);
        return false;
    }

    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error: Cannot create output file: " << outputPath << std::endl;
        inFile.close();
        CryptDestroyKey(hKey); CryptDestroyHash(hHash); CryptReleaseContext(hProv, 0);
        return false;
    }

    const DWORD CHUNK_SIZE = 4096;

    std::vector<BYTE> buffer(CHUNK_SIZE + 16); 
    DWORD bytesRead = 0;
    bool isFinal = false;

  
    inFile.seekg(0, std::ios::end);
    std::streampos fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    std::streampos currentPos = 0;

    while (currentPos < fileSize) {
        inFile.read((char*)buffer.data(), CHUNK_SIZE);
        bytesRead = static_cast<DWORD>(inFile.gcount());
        currentPos += bytesRead;
        
    
        isFinal = (currentPos >= fileSize);

        if (isEncrypt) {
            if (!CryptEncrypt(hKey, 0, isFinal, 0, buffer.data(), &bytesRead, buffer.size())) {
                std::cerr << "Error: Encryption failed." << std::endl;
                break;
            }
        } else {
            if (!CryptDecrypt(hKey, 0, isFinal, 0, buffer.data(), &bytesRead)) {
                std::cerr << "Error: Decryption failed (Wrong key or corrupted file)." << std::endl;
                break;
            }
        }
        
        outFile.write((char*)buffer.data(), bytesRead);
    }

    inFile.close();
    outFile.close();
    CryptDestroyKey(hKey);
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    std::cout << (isEncrypt ? "File encrypted: " : "File decrypted: ") << outputPath << std::endl;
    return true;
}

bool AesCipher::encryptFile(const std::string& inputPath, const std::string& outputPath) {
    return processFile(inputPath, outputPath, true);
}

bool AesCipher::decryptFile(const std::string& inputPath, const std::string& outputPath) {
    return processFile(inputPath, outputPath, false);
}
