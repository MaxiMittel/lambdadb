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
};

} // namespace sql::ast

#endif //LAMBDADB_AST_VISITOR_H