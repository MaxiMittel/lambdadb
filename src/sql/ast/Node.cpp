//
// Created by Maximilian Mittelhammer on 10.12.22.
//

#include "Node.h"

using namespace sql::ast;

Node::Node(ASTNodeType type, Position position, Repository &repository) : type(type), position(position), repository(repository) {}

ASTNodeType Node::getType() const {
    return type;
}

Position Node::getPosition() const {
    return position;
}

SelectStatment::SelectStatment(Position position, Repository &repository) : Node(ASTNodeType::SELECT_STATEMENT, position, repository) {}

void SelectStatment::accept(Visitor &visitor) {
    visitor.visit(*this);
}

EvaluationTable SelectStatment::evaluate(Evaluator &evaluator) {
    return evaluator.evaluate(*this);
}