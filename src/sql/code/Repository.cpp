//
// Created by Maximilian Mittelhammer on 04.12.22.
//

#include <iostream>
#include "Repository.h"

Repository::Repository(std::string content) : content(std::move(content)) {}

std::string_view Repository::get(Position position) {
    return std::string_view(content).substr(position.start, position.end - position.start);
}

char Repository::at(size_t position) {
    return content.at(position);
}

size_t Repository::size() {
    return content.size();
}

void Repository::logError(Position position, std::string message) {
    std::cout << position.start << ": ";

    // Change color to red
    std::cout << "\033[31mError: ";

    // Change color to default
    std::cout << "\033[0m" << message << std::endl;

    std::cout << " " << get(position) << std::endl;

    std::cout << std::endl;
}