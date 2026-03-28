#include "LicenseManager.h"
#include "HardwareID.h"
#include <iostream>
#include <fstream>
#include <sstream>

LicenseManager::LicenseManager() {
    HardwareID hwid;
    currentMachineID = hwid.getMachineID();

    std::cout << "Current Device ID " << currentMachineID << std::endl;
    std::cout << "CPU ID  " << hwid.getCpuId() << std::endl;
    std::cout << "MAC address " << hwid.getMacAddress() << std::endl;
    std::cout << std::endl;
}

LicenseKey LicenseManager::createPerpetualLicense(const std::string& userName) {
    LicenseKey license(currentMachineID, userName);
    licenses.push_back(license);
    return license;
}

LicenseKey LicenseManager::createTemporaryLicense(const std::string& userName, int days) {
    LicenseKey license(currentMachineID, userName, days);
    licenses.push_back(license);
    return license;
}

bool LicenseManager::verifyLicense(const std::string& licenseKey) {
    // Գտնել լիցենզիան
    for (auto& license : licenses) {
        if (license.getKey() == licenseKey) {
            return license.isValid(currentMachineID);
        }
    }

    // Եթե չի գտնվել, ստուգել ֆայլային պահոցից (Simple way)
    return verifyFromFile(licenseKey);
}

bool LicenseManager::saveToFile(const std::string& licenseKey, const std::string& filename) {
    for (const auto& license : licenses) {
        if (license.getKey() == licenseKey) {
            std::ofstream file(filename);
            if (!file.is_open()) return false;

            file << "KEY=" << license.getKey() << std::endl;
            file << "MACHINE=" << license.getMachineID() << std::endl;
            file << "TYPE=" << (license.isTemporaryLicense() ? "TEMPORARY" : "PERPETUAL") << std::endl;
            file << "EXPIRY=" << license.getExpiryDate() << std::endl;

            file.close();
            return true;
        }
    }
    return false;
}

bool LicenseManager::verifyFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line, savedKey, savedMachine, savedType;
    time_t savedExpiry = 0;

    while (std::getline(file, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string name = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            if (name == "KEY") savedKey = value;
            else if (name == "MACHINE") savedMachine = value;
            else if (name == "TYPE") savedType = value;
            else if (name == "EXPIRY") savedExpiry = std::stoll(value);
        }
    }
    file.close();

    if (savedMachine != currentMachineID) {
        std::cout << "ERROR: Invalid Machine ID" << std::endl;
        return false;
    }

    if (savedType == "TEMPORARY") {
        if (time(nullptr) > savedExpiry) {
            std::cout << "ERROR: License EXPIRED" << std::endl;
            return false;
        }
    }

    return true;
}

void LicenseManager::listAllLicenses() {
    if (licenses.empty()) {
        std::cout << "Լիցենզիաներ չեն գտնվել:" << std::endl;
        return;
    }

    std::cout << "\nԲՈԼՈՐ ԼԻՑԵՆԶԻԱՆԵՐԸ:" << std::endl;
    for (auto& license : licenses) {
        license.printInfo();
    }
}


void LicenseManager::checkRemainingDays(const std::string& licenseKey) {
    for (const auto& license : licenses) {
        if (license.getKey() == licenseKey) {
            if (!license.isTemporaryLicense()) {
                std::cout << "i This is a PERPETUAL license. It never expires." << std::endl;
                return;
            }
            
            time_t now = time(nullptr);
            time_t expiry = license.getExpiryDate();
            
            if (now > expiry) {
                std::cout << "x License EXPIRED on: " << ctime(&expiry);
            } else {
                // difftime возвращает разницу в секундах. Делим на кол-во секунд в дне.
                int daysLeft = difftime(expiry, now) / (60 * 60 * 24);
                std::cout << "v License is ACTIVE." << std::endl;
                std::cout << "  Expiration Date: " << ctime(&expiry);
                std::cout << "  Days remaining: " << daysLeft << " days." << std::endl;
            }
            return;
        }
    }
    std::cout << "x Error: Key not found in current session memory." << std::endl;
}
