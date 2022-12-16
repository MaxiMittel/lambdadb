//
// Created by Maximilian Mittelhammer on 05.12.22.
//

#include "Parser.h"
#include "PrintVisitor.h"

using namespace sql::parser;
using namespace sql::lexer;

Parser::Parser(Lexer& lexer, Repository& repository) : lexer(lexer), repository(repository) {}

bool Parser::parse() {
    root = parse_stmt();
    return success;
}

std::shared_ptr<Node> Parser::getRoot() const {
    return root;
}

void Parser::print(std::ostream& out) {
    PrintVisitor visitor(out);

    out << "digraph PARSER {" << std::endl;
    this->root->accept(visitor);
    out << "}" << std::endl;
}

std::shared_ptr<Node> Parser::parse_stmt() {
    std::shared_ptr<NodeStmt> stmt = std::make_shared<NodeStmt>(Position{0,0});

    auto token = lexer.peek_next();

    switch (token.type) {
        case TokenType::SELECT:
            stmt->select_stmt = parse_select_stmt();
            break;
        default:
            std::cout << "Unexpected token: " << token << std::endl;
            success = false;
            return nullptr;
    }

    return stmt;
}

std::shared_ptr<Node> Parser::parse_select_stmt() {
    auto token = lexer.next();
    std::shared_ptr<NodeSelectStmt> select_stmt = std::make_shared<NodeSelectStmt>(token.position);

    select_stmt->_select = std::make_shared<NodeSelect>(token.position);
    select_stmt->select_list = parse_select_list();

    token = lexer.next(); // TODO: Check if token is FROM
    select_stmt->_from = std::make_shared<NodeFrom>(token.position);
    
    token = lexer.next();
    select_stmt->table_name = std::make_shared<NodeIdentifier>(token.position, token.value);

    return select_stmt;
}

std::shared_ptr<Node> Parser::parse_select_list() {
    auto token = lexer.peek_next();
    std::shared_ptr<NodeSelectList> select_list = std::make_shared<NodeSelectList>(token.position);

    if (token.type == TokenType::ASTERISK) {
        lexer.next();
        select_list->asteriks = std::make_shared<NodeAsteriks>(token.position);
        return select_list;
    }

    select_list->select_list_items.push_back(parse_select_list_item());

    token = lexer.peek_next();
    while (token.type == TokenType::COMMA) {
        lexer.next();
        select_list->select_list_items.push_back(parse_select_list_item());
        token = lexer.peek_next();
    }

    return select_list;
}

std::shared_ptr<Node> Parser::parse_select_list_item() {
    auto token = lexer.next();
    std::shared_ptr<NodeSelectListItem> select_list_item = std::make_shared<NodeSelectListItem>(token.position);

    select_list_item->expr = std::make_shared<NodeIdentifier>(token.position, token.value);

    token = lexer.peek_next();
    if (token.type == TokenType::AS) {
        lexer.next();
        token = lexer.next();
        select_list_item->_as = std::make_shared<NodeAs>(token.position);
        select_list_item->identifier = std::make_shared<NodeIdentifier>(token.position, token.value);
    }

    return select_list_item;
}