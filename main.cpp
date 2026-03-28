#include <iostream>
#include <string>
#include "LicenseManager.h"

int main() {
    std::cout << "==================================================" << std::endl;
    std::cout << "    LICENSE SYSTEM - LABORATORY WORK    " << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << std::endl;

    LicenseManager manager;

    int choice;
    std::string userName;

    do {
        std::cout << "\n--- SELECT ACTION ---" << std::endl;
        std::cout << "1. Create perpetual license" << std::endl;
        std::cout << "2. Create temporary license (30 days)" << std::endl;
        std::cout << "3. Verify license" << std::endl;
        std::cout << "4. Show all licenses" << std::endl;
        std::cout << "5. Save license to file" << std::endl;
        std::cout << "6. Check remaining days (Temporary)" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Your choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1: {
            std::cout << "Enter licensee name: ";
            std::getline(std::cin, userName);

            LicenseKey license = manager.createPerpetualLicense(userName);
            std::cout << "\n!!!! PERPETUAL LICENSE CREATED:" << std::endl;
            license.printInfo();
            break;
        }

        case 2: {
            std::cout << "Enter licensee name: ";
            std::getline(std::cin, userName);

            LicenseKey license = manager.createTemporaryLicense(userName, 30);
            std::cout << "\nv TEMPORARY LICENSE CREATED (30 days):" << std::endl;
            license.printInfo();
            break;
        }

        case 3: {
            std::string key;
            std::cout << "Enter license key: ";
            std::getline(std::cin, key);

            if (manager.verifyLicense(key)) {
                std::cout << "v LICENSE IS VALID:" << std::endl;
            }
            else {
                std::cout << "x LICENSE IS INVALID OR NOT FOUND:" << std::endl;
            }
            break;
        }

        case 4: {
            manager.listAllLicenses();
            break;
        }

      case 5: {
            std::string key, filename;
            std::cout << "Enter license key to save: ";
            std::getline(std::cin, key);
            std::cout << "Enter filename (e.g., license.txt): ";
            std::getline(std::cin, filename);

            if (manager.saveToFile(key, filename)) {
                std::cout << "v License saved to " << filename << std::endl;
            }
            else {
                std::cout << "x Error: Key not found or file error" << std::endl;
            }
            break;
        }
        
        case 6: {
            std::string key;
            std::cout << "Enter license key to check: ";
            std::getline(std::cin, key);
            manager.checkRemainingDays(key);
            break;
        }

        case 0: {
            std::cout << "Program finished." << std::endl;
            break;
        }

        default: {
            std::cout << "Invalid choice:" << std::endl;
            break;
        }
        }

    } while (choice != 0);

    return 0;
}

