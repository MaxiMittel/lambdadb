//
// Created by Maximilian Mittelhammer on 10.12.22.
//

#include "Evaluator.h"
#include "AST.h"

using namespace sql::ast;

Evaluator::Evaluator(AST &ast, db::Database &database) : ast(ast), database(database) {}

db::Database &Evaluator::getDatabase() const {
    return database;
}

db::EvaluationTable Evaluator::evaluate() {
    this->evaluationTable = std::make_shared<db::EvaluationTable>();
    ast.getRoot()->evaluate(*this);
    return *evaluationTable;
}

std::shared_ptr<db::EvaluationTable> Evaluator::getEvaluationTable() const {
    return evaluationTable;
}

void Evaluator::setEvaluationTable(std::shared_ptr<db::EvaluationTable> evaluationTable) {
    this->evaluationTable = evaluationTable;
}