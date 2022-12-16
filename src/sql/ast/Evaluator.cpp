//
// Created by Maximilian Mittelhammer on 10.12.22.
//

#include "Evaluator.h"
#include "AST.h"

using namespace sql::ast;

Evaluator::Evaluator(AST &ast, Database &database) : ast(ast), database(database) {}

Database &Evaluator::getDatabase() const {
    return database;
}

EvaluationTable Evaluator::evaluate() {
    this->evaluationTable = std::make_shared<EvaluationTable>();
    ast.getRoot()->evaluate(*this);
    return *evaluationTable;
}

std::shared_ptr<EvaluationTable> Evaluator::getEvaluationTable() const {
    return evaluationTable;
}

void Evaluator::setEvaluationTable(std::shared_ptr<EvaluationTable> evaluationTable) {
    this->evaluationTable = evaluationTable;
}