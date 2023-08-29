#ifndef ARTNET_RECEIVER_H
#define ARTNET_RECEIVER_H

#include <iostream>
#include <asio.hpp>

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

    void processArtnetData(const char* data, std::size_t length) {
        // Implement your Artnet data processing logic here
        // Parse and interpret the received data according to the Artnet protocol
    }

    asio::ip::udp::socket socket_;
    asio::ip::udp::endpoint senderEndpoint_;
    char buffer_[ArtnetMaxPacketSize]; // Define ArtnetMaxPacketSize appropriately
};

#endif // ARTNET_RECEIVER_H
