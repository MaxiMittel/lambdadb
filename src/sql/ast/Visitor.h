//
// Created by Maximilian Mittelhammer on 11.12.22.
//

#ifndef LAMBDADB_AST_VISITOR_H
#define LAMBDADB_AST_VISITOR_H

#include "Node.h"

namespace sql::ast {

class Visitor {
    public:
        virtual void visit(const Statement& node) = 0;
        virtual void visit(const SelectStatment& node) = 0;
        virtual void visit(const JoinStatement& node) = 0;
        virtual void visit(const ExpressionNode& node) = 0;
        virtual void visit(const AndExpressionNode& node) = 0;
        virtual void visit(const OrExpressionNode& node) = 0;
        virtual void visit(const BoolExpressionNode& node) = 0;
        virtual void visit(const UnaryExpressionNode& node) = 0;
        virtual void visit(const PrimaryExpressionNode& node) = 0;
        virtual void visit(const InsertStatement& node) = 0;
};

} // namespace sql::ast

#endif //LAMBDADB_AST_VISITOR_H