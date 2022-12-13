//
// Created by Maximilian Mittelhammer on 10.12.22.
//

#include "Evaluator.h"

using namespace sql::ast;

Evaluator::Evaluator(AST &ast, Database &database) : ast(ast), database(database) {}

Database &Evaluator::getDatabase() const {
    return database;
}

EvaluationTable Evaluator::evaluate() {
    return ast.getRoot()->accept(*this);
}