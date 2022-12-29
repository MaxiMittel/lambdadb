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
    std::shared_ptr<Node> parse_insert_stmt();
    std::shared_ptr<Node> parse_delete_stmt();
    std::shared_ptr<Node> parse_update_stmt();
    std::shared_ptr<Node> parse_create_stmt();
    std::shared_ptr<Node> parse_drop_stmt();
    std::shared_ptr<Node> parse_alter_stmt();

    std::shared_ptr<Node> parse_select_list();
    std::shared_ptr<Node> parse_select_list_item();
    std::shared_ptr<Node> parse_table_list();
    std::shared_ptr<Node> parse_table_list_item();
    std::shared_ptr<Node> parse_join_clause();
    std::shared_ptr<Node> parse_where_clause();
    std::shared_ptr<Node> parse_order_clause();
    std::shared_ptr<Node> parse_limit_clause();
    std::shared_ptr<Node> parse_expr();
    std::shared_ptr<Node> parse_and_expr();
    std::shared_ptr<Node> parse_or_expr();
    std::shared_ptr<Node> parse_bool_expr();
    std::shared_ptr<Node> parse_unary_expr();
    std::shared_ptr<Node> parse_primary_expr();
    std::shared_ptr<Node> parse_order_list();
    std::shared_ptr<Node> parse_order_list_item();
    std::shared_ptr<Node> parse_column_list();
    std::shared_ptr<Node> parse_column_list_item();
    std::shared_ptr<Node> parse_value_list();
    std::shared_ptr<Node> parse_value_list_item();
    std::shared_ptr<Node> parse_assignment_list();
    std::shared_ptr<Node> parse_assignment_list_item();
    std::shared_ptr<Node> parse_column_def();
    std::shared_ptr<Node> parse_column_def_list();
    std::shared_ptr<Node> parse_literal();

    void assertTokenType(sql::lexer::TokenType expected, sql::lexer::TokenType actual, Position position);

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

} // namespace sql::parser

#endif //LAMBDADB_PARSER_H