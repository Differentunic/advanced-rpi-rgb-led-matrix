#include "ConfigManager.h"
#include <fstream>
#include <iostream>

ConfigManager::ConfigManager() {
    // Constructor
}

ConfigManager::~ConfigManager() {
    // Destructor
}

void ConfigManager::loadOrCreateConfig() {
    std::ifstream configFile("config.txt");
    if (!configFile.good()) {
        // If config file doesn't exist, create default config
        // ...

        // Save the default config to config.txt
        // ...
    } else {
        // Load existing config
        std::string line;
        while (std::getline(configFile, line)) {
            size_t equalsPos = line.find('=');
            if (equalsPos != std::string::npos) {
                std::string key = line.substr(0, equalsPos);
                std::string value = line.substr(equalsPos + 1);

                if (key == "mode") {
                    mode = value;
                } else if (key == "hardware_mapping") {
                    matrixOptions.hardware_mapping = value;
                } else if (key == "rows") {
                    matrixOptions.rows = std::stoi(value);
                } else if (key == "cols") {
                    matrixOptions.cols = std::stoi(value);
                } else if (key == "chain_length") {
                    matrixOptions.chain_length = std::stoi(value);
                } else if (key == "parallel") {
                    matrixOptions.parallel = std::stoi(value);
                }
            }
        }
        configFile.close();
    }
}

std::string ConfigManager::getMode() {
    return mode;
}

RGBMatrix::Options ConfigManager::getMatrixOptions() {
    return matrixOptions;
}
