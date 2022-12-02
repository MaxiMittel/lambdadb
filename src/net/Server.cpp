//
// Created by Maximilian Mittelhammer on 29.11.22.
//

#include "Server.h"
#include <utility>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <stdexcept>

Server::Server(int port, std::string address): port(port), address(std::move(address)) {}

Server::~Server() {
    for (int client : clients) {
        close(client);
    }
    ::close(this->socket);
    std::cout << "[Server] Server stopped" << std::endl;
}

void Server::bind() {
    this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket == -1) {
        throw std::runtime_error("[Server] Socket creation failed");
    }
    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(this->port);
    inet_pton(AF_INET, this->address.c_str(), &hint.sin_addr);

    int bind_res = ::bind(this->socket, (sockaddr *) &hint, sizeof(hint));
    if (bind_res == -1) {
        throw std::runtime_error("[Server] Could not bind to IP/port");
    }
    listen(this->socket, SOMAXCONN);
}

int Server::accept() {
    sockaddr_in client{};
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    int clientSocket = 0;
    do {
       clientSocket = ::accept(this->socket, (sockaddr *) &client, &clientSize);
    } while (clientSocket == -1);

    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    if (getnameinfo((sockaddr *) &client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        std::cout << "[Server] " << host << " connected on port " << service << std::endl;
    } else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << "[Server] " << host << " connected on port " << ntohs(client.sin_port) << std::endl;
    }

    clients.emplace_back(clientSocket);

    return clientSocket;
}

void Server::close(int client) {
    ::close(client);
    //clients.erase(std::remove(clients.begin(), clients.end(), socket), clients.end());
    std::cout << "[Server] Client disconnected" << std::endl;
}

void Server::send(int client, const std::string& message) {
    ssize_t sendRes = ::send(client, message.c_str(), message.size() + 1, 0);
    if (sendRes == -1) {
        throw std::runtime_error("[Server] Message failed to send");
    }
}

std::string Server::receive(int client) {
    char buf[4096];
    while (true) {
        memset(buf, 0, 4096);
        ssize_t bytesReceived = recv(client, buf, 4096, 0);
        if (bytesReceived == -1) {
            throw std::runtime_error("[Server] Connection was interrupted");
        }
        if (bytesReceived == 0) {
            std::cout << "[Server] Client disconnected" << std::endl;
            break;
        }
        return {buf, 0, static_cast<size_t>(bytesReceived)};
    }

    return "";
}