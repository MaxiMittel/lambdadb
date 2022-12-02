//
// Created by Maximilian Mittelhammer on 29.11.22.
//

#ifndef LAMBDADB_SERVER_H
#define LAMBDADB_SERVER_H

#include <string>
#include <vector>

class Server {
private:
    int socket{};
    int port;
    std::string address;
    std::vector<int> clients;
public:
    Server(int port, std::string address);
    ~Server();

    /**
     * Binds the server to the given address and port
     */
    void bind();

    /**
     * Accepts a new client
     * @return The client socket
     */
    int accept();

    /**
     * Closes the connection to the given client
     * @param client The client socket
     */
    void close(int client);

    /**
     * Sends a message to the given client
     * @param client The client socket
     * @param message The message to send
     */
    void send(int client, const std::string &message);

    /**
     * Receives a message from the given client
     * @param client The client socket
     * @return Message from the client
     */
    std::string receive(int client);
};


#endif //LAMBDADB_SERVER_H
