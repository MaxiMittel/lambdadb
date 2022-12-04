//
// Created by Maximilian Mittelhammer on 04.12.22.
//

#ifndef LAMBDADB_REPOSITORY_H
#define LAMBDADB_REPOSITORY_H

#include <cstddef>
#include <string>


struct Position {
    size_t start;
    size_t end;
};

class Repository {
private:
    std::string content;
public:
    Repository(std::string content);

    /**
     * Get the content at the given position.
     * @param position The position to get the content from.
     * @return The content at the given position.
     */
    std::string_view get(Position position);

    /**
     * Get the char at the given position.
     * @param position The position to get the char from.
     * @return The char at the given position.
     */
    char at(size_t position);
    
    /**
     * Get the size of the content.
    */
    size_t size();

    /**
     * Log an error at the given position.
    */
    void logError(Position position, std::string message);
};

#endif //LAMBDADB_REPOSITORY_H