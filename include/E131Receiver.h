#ifndef E131_RECEIVER_H
#define E131_RECEIVER_H

#include <iostream>
#include <asio.hpp>
#include <vector>

constexpr std::size_t ArtnetMaxPacketSize = 2048; // Define an appropriate value
class E131Receiver {
public:
    E131Receiver(asio::io_context& ioContext, const std::string& bindAddress, unsigned short port)
        : socket_(ioContext, asio::ip::udp::endpoint(asio::ip::address::from_string(bindAddress), port)) {
    }

    void startListening() {
        receiveData();
    }

private:
    struct E131Header {
        char preamble[7];
        uint16_t flags_length;
        uint8_t vector;
        uint8_t sourceName[64];
        uint8_t priority;
        uint16_t syncAddress;
        uint8_t sequenceNumber;
        uint8_t options;
        uint16_t universe;
        char data[512]; // Max DMP data size
    };

    void receiveData() {
        socket_.async_receive_from(asio::buffer(buffer_), senderEndpoint_,
            [this](std::error_code ec, std::size_t bytesReceived) {
                if (!ec) {
                    if (bytesReceived >= sizeof(E131Header)) {
                        E131Header* header = reinterpret_cast<E131Header*>(buffer_);
                        if (validateE131Header(header)) {
                            processE131Data(header);
                        }
                    }
                    receiveData();
                } else {
                    std::cerr << "Error receiving data: " << ec.message() << std::endl;
                }
            });
    }

    bool validateE131Header(const E131Header* header) {
        // Implement header validation logic here
        // Check for correct Preamble, Vector, etc.
        return true; // Return true if valid
    }

    void processE131Data(const E131Header* header) {
        // Implement E1.31 data processing logic here
        // Extract and process data from the header and DMP layer
    }

    asio::ip::udp::socket socket_;
    asio::ip::udp::endpoint senderEndpoint_;
    char buffer_[E131MaxPacketSize]; // Define E131MaxPacketSize appropriately
};

#endif // E131_RECEIVER_H
