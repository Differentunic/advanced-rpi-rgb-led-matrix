#include <iostream>
#include <asio.hpp>
#include "ArtnetReceiver.h"
#include "E131Receiver.h"
#include "ConfigManager.h"
#include <thread>
#include <signal.h>
#include <unistd.h>
#include <led-matrix.h>

using namespace rgb_matrix;

bool shouldExit = false;

void sigintHandler(int signal) {
    shouldExit = true;
}

int main(int argc, char** argv) {
    signal(SIGINT, sigintHandler);

    asio::io_context ioContext;

    ConfigManager configManager;
    configManager.loadOrCreateConfig();

    std::string mode = configManager.getMode();
    RGBMatrix::Options matrixOptions = configManager.getMatrixOptions();


    if (mode == "artnet") {
        ArtnetReceiver artnetReceiver(ioContext, "0.0.0.0", 6454); // Art-Net port
        artnetReceiver.startListening();
    } else if (mode == "e131") {
        E131Receiver e131Receiver(ioContext, "0.0.0.0", 5568); // E1.31 port
        e131Receiver.startListening();
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
        // DrawText(canvas, font, x, y, color, "Received Data");
        // canvas = matrix->SwapOnVSync(canvas);
        
        ioContext.poll();
    }

    delete matrix;
    return 0;
}