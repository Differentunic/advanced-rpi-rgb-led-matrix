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
        mode = "artnet"; // Set your default mode
        matrixOptions.rows = 32; // Set default rows
        matrixOptions.cols = 32; // Set default cols
        matrixOptions.chain_length = 1; // Set default chain length
        matrixOptions.parallel = 1; // Set default parallel

        // Save the default config to config.txt
        std::ofstream outFile("config.txt");
        if (outFile.is_open()) {
            outFile << "mode=artnet" << std::endl;
            outFile << "hardware_mapping=default" << std::endl;
            outFile << "rows=32" << std::endl;
            outFile << "cols=32" << std::endl;
            outFile << "chain_length=1" << std::endl;
            outFile << "parallel=1" << std::endl;
            outFile.close();
        } else {
            std::cerr << "Error saving default config." << std::endl;
        }
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

rgb_matrix::RGBMatrix::Options ConfigManager::getMatrixOptions() {
    return matrixOptions;
}
