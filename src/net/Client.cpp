//
// Created by Maximilian Mittelhammer on 29.11.22.
//

#include "Client.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdexcept>
#include <unistd.h>

Client::Client(int port, std::string address): port(port), address(std::move(address)) {}

Client::~Client() {
    disconnect();
}

void Client::connect() {
    this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket == -1) {
        throw std::runtime_error("[Client] Socket creation failed");
    }
    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(this->port);
    inet_pton(AF_INET, this->address.c_str(), &hint.sin_addr);

    int connectRes = ::connect(this->socket, (sockaddr *) &hint, sizeof(hint));
    if (connectRes == -1) {
        throw std::runtime_error("[Client] Could not connect to server");
    }
}

void Client::disconnect() const {
    ::close(this->socket);
}

void Client::send(const std::string& message) const {
    ssize_t sendRes = ::send(this->socket, message.c_str(), message.size() + 1, 0);
    if (sendRes == -1) {
        throw std::runtime_error("Could not send to server");
    }
}

std::string Client::receive() const {
    char buf[4096];
    ssize_t bytesReceived = recv(this->socket, buf, 4096, 0);
    if (bytesReceived == -1) {
        throw std::runtime_error("Could not receive from server");
    }
    if (bytesReceived == 0) {
        throw std::runtime_error("Server disconnected");
    }
    return {buf, 0, static_cast<size_t>(bytesReceived)};
}
