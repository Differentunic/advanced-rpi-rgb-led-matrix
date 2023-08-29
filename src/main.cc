#include <iostream>
#include <fstream>
#include <asio.hpp>
#include "modules\ArtnetReceiver.h"
#include "modules\E131Receiver.h"
#include <thread>
#include <signal.h>
#include <unistd.h>
#include <led-matrix.h>

using namespace rgb_matrix;

bool shouldExit = false;

void sigintHandler(int signal) {
    shouldExit = true;
}

// Function to load or create default configuration
void loadOrCreateConfig(std::string& mode, RGBMatrix::Options& matrixOptions) {
    std::ifstream configFile("config.txt");
    if (!configFile.good()) {
        // If config file doesn't exist, create default config
        mode = "artnet";
        matrixOptions.hardware_mapping = "regular";
        matrixOptions.rows = 32;
        matrixOptions.cols = 64;
        matrixOptions.chain_length = 1;
        matrixOptions.parallel = 1;

        std::ofstream defaultConfigFile("config.txt");
        if (defaultConfigFile.good()) {
            defaultConfigFile << "mode=artnet\n";
            defaultConfigFile << "hardware_mapping=regular\n";
            defaultConfigFile << "rows=32\n";
            defaultConfigFile << "cols=64\n";
            defaultConfigFile << "chain_length=1\n";
            defaultConfigFile << "parallel=1\n";
            defaultConfigFile.close();
        } else {
            std::cerr << "Error creating default config file." << std::endl;
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

int main(int argc, char** argv) {
    signal(SIGINT, sigintHandler);

    std::string mode;
    RGBMatrix::Options matrixOptions;
    loadOrCreateConfig(mode, matrixOptions);

    asio::io_context ioContext;

    if (mode == "artnet") {
        ArtnetReceiver artnetReceiver(ioContext, "0.0.0.0", 6454); // Art-Net port
        artnetReceiver.startListening();
    } else if (mode == "e131") {
        E131Receiver e131Receiver(ioContext, "0.0.0.0", 5568); // E1.31 port
        e131Receiver.startListening();
    } else if (mode == "airplay") {
        AirPlayReceiver airplayReceiver("MyAirPlayReceiver", "password", "00:11:22:33:44:55");
        airplayReceiver.start();
    } else {
        std::cerr << "Invalid mode specified in config: " << mode << std::endl;
        return 1;
    }

    RGBMatrix* matrix = CreateMatrixFromOptions(matrixOptions);
    if (matrix == nullptr) {
        std::cerr << "Error creating RGBMatrix instance." << std::endl;
        return 1;
    }

    Canvas* canvas = matrix->CreateFrameCanvas();

    while (!shouldExit) {
        // Update LED matrix using received data
        // Implement your logic to update the matrix based on the received data
        
        // For example, you can clear the canvas and draw something like this:
        canvas->Clear();
        DrawText(canvas, font, x, y, color, "Received Data");
        canvas = matrix->SwapOnVSync(canvas);
        
        ioContext.poll();
    }

    delete matrix;
    return 0;
}