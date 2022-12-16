//
// Created by Maximilian Mittelhammer on 05.12.22.
//

#ifndef LAMBDADB_PARSER_H
#define LAMBDADB_PARSER_H

#include "../code/Repository.h"
#include "../lexer/Lexer.h"
#include "../lexer/Token.h"
#include "Node.h"
#include <iostream>
#include <ostream>

namespace sql::parser {

class Parser {
private:
    sql::lexer::Lexer& lexer;
    Repository& repository;
    std::shared_ptr<Node> root;
    bool success = true;

    std::shared_ptr<Node> parse_stmt();
    std::shared_ptr<Node> parse_select_stmt();

    std::shared_ptr<Node> parse_select_list();
    std::shared_ptr<Node> parse_select_list_item();

public:
    Parser(sql::lexer::Lexer& lexer, Repository& repository);

    /**
     * Parses the lexer input and creates a syntax tree
     * @return true if the parsing was successful
    */
    bool parse();

    /**
     * Prints the syntax tree to the given output stream
     * @param out the output stream
    */
    void print(std::ostream& out);

    /**
     * Returns the root node of the syntax tree
     * @return the root node
    */
    std::shared_ptr<Node> getRoot() const;
};
}

#endif //LAMBDADB_PARSER_H