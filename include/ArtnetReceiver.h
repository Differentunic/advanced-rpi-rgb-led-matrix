#ifndef ARTNET_RECEIVER_H
#define ARTNET_RECEIVER_H

#include <iostream>
#include <asio.hpp>

constexpr std::size_t ArtnetMaxPacketSize = 2048; // Define an appropriate value

// Define the ArtnetOpCodes enum
enum class ArtnetOpCodes {
    OpDmx, // Add more op codes as needed
    // ...
};

// Define the ArtnetPacketHeader struct
struct ArtnetPacketHeader {
    uint16_t opCode; // Add other fields as needed
    // ...
};

class ArtnetReceiver {
public:
    ArtnetReceiver(asio::io_context& ioContext, const std::string& bindAddress, unsigned short port)
        : socket_(ioContext, asio::ip::udp::endpoint(asio::ip::address::from_string(bindAddress), port)) {
    }

    void startListening() {
        receiveData();
    }

private:
    void receiveData() {
        socket_.async_receive_from(asio::buffer(buffer_), senderEndpoint_,
            [this](std::error_code ec, std::size_t bytesReceived) {
                if (!ec) {
                    // Handle received Artnet data from the buffer
                    
                    // Call your custom processing function here
                    processArtnetData(buffer_, bytesReceived);
                    
                    // Continue listening for more data
                    receiveData();
                } else {
                    std::cerr << "Error receiving data: " << ec.message() << std::endl;
                }
            });
    }

// Function to process the received Artnet data
void processArtnetData(const char* data, std::size_t length) {
    // Check if the data length is at least the size of an Artnet packet header
    if (length >= sizeof(ArtnetPacketHeader)) {
        // Cast the received data to a pointer of the ArtnetPacketHeader type
        const ArtnetPacketHeader* packetHeader = reinterpret_cast<const ArtnetPacketHeader*>(data);

        // Check if the packet is an Artnet ArtDmx packet
        if (static_cast<ArtnetOpCodes>(packetHeader->opCode) == ArtnetOpCodes::OpDmx) {
            // Calculate the size of the DMX data in the packet
            std::size_t dmxDataSize = length - sizeof(ArtnetPacketHeader);

            // Cast the DMX data pointer to the appropriate type
            const uint8_t* dmxData = reinterpret_cast<const uint8_t*>(data + sizeof(ArtnetPacketHeader));

            // Now you can use 'dmxData' and 'dmxDataSize' to work with the DMX data
            // For example, you might want to process and display the DMX values

            // Print the DMX values as an example
            for (std::size_t i = 0; i < dmxDataSize; ++i) {
                std::cout << "Channel " << i << ": " << static_cast<int>(dmxData[i]) << std::endl;
            }
        }
    }
}

    asio::ip::udp::socket socket_;
    asio::ip::udp::endpoint senderEndpoint_;
    char buffer_[ArtnetMaxPacketSize]; // Define ArtnetMaxPacketSize appropriately
};

#endif // ARTNET_RECEIVER_H
