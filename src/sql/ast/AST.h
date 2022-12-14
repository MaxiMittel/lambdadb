//
// Created by Maximilian Mittelhammer on 10.12.22.
//

#ifndef LAMBDADB_AST_AST_H
#define LAMBDADB_AST_AST_H

#include <vector>
#include "../code/Repository.h"
#include "../parser/Parser.h"
#include "Node.h"

namespace sql::ast {

class AST {
    private:
    std::shared_ptr<Node> root;
    sql::parser::Parser& parser;
    Repository& repository;
    bool success = true;

    std::shared_ptr<Node> analyzeStatement(parser::NodeStmt* node);
    std::shared_ptr<Node> analyzeStatement(parser::NodeSelectStmt* node);
    std::shared_ptr<Node> analyzeStatement(parser::NodeWhereClause* node);
    std::shared_ptr<Node> analyzeStatement(parser::NodeInsertStmt* node);

    std::shared_ptr<ExpressionBaseNode> analyzeStatement(parser::NodeAndExpression* node);
    std::shared_ptr<ExpressionBaseNode> analyzeStatement(parser::NodeOrExpression* node);
    std::shared_ptr<ExpressionBaseNode> analyzeStatement(parser::NodeBoolExpression* node);
    std::shared_ptr<ExpressionBaseNode> analyzeStatement(parser::NodeUnaryExpression* node);
    std::shared_ptr<ExpressionBaseNode> analyzeStatement(parser::NodePrimaryExpression* node);

    public:
    AST(sql::parser::Parser& parser, Repository& repository);

    /**
     * Returns the root node of the AST
     * @return Root node
     */
    [[nodiscard]] std::shared_ptr<Node> getRoot() const;

    /**
     * Analyzes the Parse Tree
     * @return True if successful
     */
    bool analyze();

    /**
     * Prints the AST in the DOT Format
     * @param out Output stream
     */
    void print(std::ostream& out);
};

} // namespace sql::ast

#endif // LAMBDADB_AST_AST_H