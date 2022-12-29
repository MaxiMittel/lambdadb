//
// Created by Maximilian Mittelhammer on 16.12.22.
//

#ifndef LAMBDADB_AST_PRINTVISITOR_H
#define LAMBDADB_AST_PRINTVISITOR_H

#include "Node.h"
#include "Visitor.h"

namespace sql::ast
{

    class PrintVisitor : public Visitor
    {
    private:
        std::ostream &out;
        int node_count = 0;

    public:
        PrintVisitor(std::ostream &out);

        virtual void visit(const Statement &node) override;
        virtual void visit(const SelectStatment &node) override;
        virtual void visit(const JoinStatement& node) override;
        virtual void visit(const ExpressionNode& node) override;
        virtual void visit(const AndExpressionNode& node) override;
        virtual void visit(const OrExpressionNode& node) override;
        virtual void visit(const BoolExpressionNode& node) override;
        virtual void visit(const UnaryExpressionNode& node) override;
        virtual void visit(const PrimaryExpressionNode& node) override;
    };

}

#endif // LAMBDADB_AST_PRINTVISITOR_H