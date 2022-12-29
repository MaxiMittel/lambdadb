//
// Created by Maximilian Mittelhammer on 10.12.22.
//

#include "Node.h"
#include "Visitor.h"
#include <ranges>

using namespace sql::ast;

Node::Node(ASTNodeType type, Position position, Repository &repository) : type(type), position(position), repository(repository) {}

ASTNodeType Node::getType() const {
    return type;
}

Position Node::getPosition() const {
    return position;
}

Statement::Statement(Position position, Repository &repository) : Node(ASTNodeType::STATEMENT, position, repository) {}

void Statement::setStatements(std::vector<std::shared_ptr<Node>> statements) {
    this->statements = statements;
}

std::vector<std::shared_ptr<Node>> Statement::getStatements() const {
    return statements;
}

void Statement::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void Statement::evaluate(Evaluator &evaluator) {
    // TODO: Handle multiple statements
    statements[0]->evaluate(evaluator);
}

SelectStatment::SelectStatment(Position position, Repository &repository) : Node(ASTNodeType::SELECT_STATEMENT, position, repository) {}

void SelectStatment::setColumns(std::vector<ColumnRef> columns) {
    this->columns = columns;
}

void SelectStatment::setTables(std::vector<TableRef> tables) {
    this->tables = tables;
}

void SelectStatment::setWhere(std::shared_ptr<Node> where) {
    this->where = where;
}

void SelectStatment::setJoin(std::shared_ptr<Node> join) {
    this->join = join;
}

std::vector<ColumnRef> SelectStatment::getColumns() const {
    return columns;
}

std::vector<TableRef> SelectStatment::getTables() const {
    return tables;
}

std::shared_ptr<Node> SelectStatment::getWhere() const {
    return where;
}

std::shared_ptr<Node> SelectStatment::getJoin() const {
    return join;
}

void SelectStatment::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void SelectStatment::evaluate(Evaluator &evaluator) {
    for (auto &table : tables) {
        std::shared_ptr<Table> tableRef = evaluator.getDatabase().getTable(std::string(table.name));
        
        if (evaluator.getEvaluationTable().get()->getRowCount() == 0) {
            evaluator.getEvaluationTable().get()->setColumns(tableRef->getColumns());
            evaluator.getEvaluationTable().get()->setItems(tableRef->getItems());
        } else {
            // Merge tables
            std::vector<Column> newColumns;
            newColumns.insert(newColumns.end(), evaluator.getEvaluationTable().get()->getColumns().begin(), evaluator.getEvaluationTable().get()->getColumns().end());
            newColumns.insert(newColumns.end(), tableRef->getColumns().begin(), tableRef->getColumns().end());
            
            // Cross join
            std::vector<std::vector<std::shared_ptr<DataEntryBase>>> newItems;
            for (auto &item1 : evaluator.getEvaluationTable().get()->getItems()) {
                for (auto &item2 : tableRef->getItems()) {
                    std::vector<std::shared_ptr<DataEntryBase>> newItem;
                    newItem.insert(newItem.end(), item1.begin(), item1.end());
                    newItem.insert(newItem.end(), item2.begin(), item2.end());
                    newItems.emplace_back(newItem);
                }
            }

            evaluator.getEvaluationTable().get()->setItems(newItems);
        }
    }

    // TODO: Evaluate WHERE clause

    // TODO: Project columns, maybe in the JSON output
}

JoinStatement::JoinStatement(Position position, Repository &repository) : Node(ASTNodeType::JOIN_CLAUSE, position, repository) {}

void JoinStatement::setType(sql::ast::JoinType type) {
    this->type = type;
}

sql::ast::JoinType JoinStatement::getType() const {
    return type;
}

void JoinStatement::setTable(TableRef table) {
    this->table = table;
}

TableRef JoinStatement::getTable() const {
    return table;
}

void JoinStatement::setLeft(ColumnRef column) {
    this->left = column;
}

ColumnRef JoinStatement::getLeft() const {
    return left;
}

void JoinStatement::setRight(ColumnRef column) {
    this->right = column;
}

ColumnRef JoinStatement::getRight() const {
    return right;
}

void JoinStatement::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void JoinStatement::evaluate(Evaluator &evaluator) {
    std::ignore = evaluator;
}

ExpressionBaseNode::ExpressionBaseNode(ASTNodeType type, Position position, Repository &repository) : Node(type, position, repository) {}

ExpressionNode::ExpressionNode(Position position, Repository &repository) : ExpressionBaseNode(ASTNodeType::EXPRESSION, position, repository) {}

void ExpressionNode::setAndExpr(std::shared_ptr<ExpressionBaseNode> and_expr) {
    this->and_expr = and_expr;
}

std::shared_ptr<ExpressionBaseNode> ExpressionNode::getAndExpr() const {
    return and_expr;
}

void ExpressionNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void ExpressionNode::evaluate(Evaluator &evaluator) {
    and_expr->evaluate(evaluator);
}

bool ExpressionNode::evaluateExpression(std::vector<std::shared_ptr<DataEntryBase>> const& entry) {
    std::ignore = entry;
    return true;
}

AndExpressionNode::AndExpressionNode(Position position, Repository &repository) : ExpressionBaseNode(ASTNodeType::AND_EXPRESSION, position, repository) {}

void AndExpressionNode::setOrExpr(std::shared_ptr<ExpressionBaseNode> or_expr) {
    this->or_expr = or_expr;
}

std::shared_ptr<ExpressionBaseNode> AndExpressionNode::getOrExpr() const {
    return or_expr;
}

void AndExpressionNode::setAndExpr(std::shared_ptr<ExpressionBaseNode> and_expr) {
    this->and_expr = and_expr;
}

std::shared_ptr<ExpressionBaseNode> AndExpressionNode::getAndExpr() const {
    return and_expr;
}

void AndExpressionNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}

bool AndExpressionNode::evaluateExpression(std::vector<std::shared_ptr<DataEntryBase>> const& entry) {
    bool or_result = or_expr->evaluateExpression(entry);
    if (!or_result) return false;

    return and_expr == nullptr || and_expr->evaluateExpression(entry);
}

void AndExpressionNode::evaluate(Evaluator &evaluator) {
    std::ignore = evaluator;
}

OrExpressionNode::OrExpressionNode(Position position, Repository &repository) : ExpressionBaseNode(ASTNodeType::OR_EXPRESSION, position, repository) {}

void OrExpressionNode::setOrExpr(std::shared_ptr<ExpressionBaseNode> or_expr) {
    this->or_expr = or_expr;
}

std::shared_ptr<ExpressionBaseNode> OrExpressionNode::getOrExpr() const {
    return or_expr;
}

void OrExpressionNode::setBoolExpr(std::shared_ptr<ExpressionBaseNode> bool_expr) {
    this->bool_expr = bool_expr;
}

std::shared_ptr<ExpressionBaseNode> OrExpressionNode::getBoolExpr() const {
    return bool_expr;
}

void OrExpressionNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}

bool OrExpressionNode::evaluateExpression(std::vector<std::shared_ptr<DataEntryBase>> const& entry) {
    bool bool_result = bool_expr->evaluateExpression(entry);
    if (bool_result) return true;

    return or_expr == nullptr || or_expr->evaluateExpression(entry);
}

void OrExpressionNode::evaluate(Evaluator &evaluator) {
    std::ignore = evaluator;
}

BoolExpressionNode::BoolExpressionNode(Position position, Repository &repository) : ExpressionBaseNode(ASTNodeType::BOOL_EXPRESSION, position, repository) {}

void BoolExpressionNode::setLeft(std::shared_ptr<ExpressionBaseNode> left) {
    this->left = left;
}

std::shared_ptr<ExpressionBaseNode> BoolExpressionNode::getLeft() const {
    return left;
}

void BoolExpressionNode::setRight(std::shared_ptr<ExpressionBaseNode> right) {
    this->right = right;
}

std::shared_ptr<ExpressionBaseNode> BoolExpressionNode::getRight() const {
    return right;
}

void BoolExpressionNode::setOperator(sql::ast::BoolOperator op) {
    this->op = op;
}

sql::ast::BoolOperator BoolExpressionNode::getOperator() const {
    return op;
}

void BoolExpressionNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}

bool BoolExpressionNode::evaluateExpression(std::vector<std::shared_ptr<DataEntryBase>> const& entry) {
    bool left_result = left->evaluateExpression(entry);
    bool right_result = right->evaluateExpression(entry);

    // TODO: Return type not bool
    switch (op) {
        case sql::ast::BoolOperator::EQUAL:
            return left_result == right_result;
        case sql::ast::BoolOperator::NOT_EQUAL:
            return left_result != right_result;
        case sql::ast::BoolOperator::LESS:
            return left_result < right_result;
        case sql::ast::BoolOperator::LESS_EQUAL:
            return left_result <= right_result;
        case sql::ast::BoolOperator::GREATER:
            return left_result > right_result;
        case sql::ast::BoolOperator::GREATER_EQUAL:
            return left_result >= right_result;
        default:
            return false;
    }
}

void BoolExpressionNode::evaluate(Evaluator &evaluator) {
    std::ignore = evaluator;
}

UnaryExpressionNode::UnaryExpressionNode(Position position, Repository &repository) : ExpressionBaseNode(ASTNodeType::UNARY_EXPRESSION, position, repository) {}

void UnaryExpressionNode::setPrimaryExpr(std::shared_ptr<ExpressionBaseNode> primary_expr) {
    this->primary_expr = primary_expr;
}

std::shared_ptr<ExpressionBaseNode> UnaryExpressionNode::getPrimaryExpr() const {
    return primary_expr;
}

void UnaryExpressionNode::setOperator(sql::ast::UnaryOperator op) {
    this->op = op;
}

sql::ast::UnaryOperator UnaryExpressionNode::getOperator() const {
    return op;
}

void UnaryExpressionNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}

bool UnaryExpressionNode::evaluateExpression(std::vector<std::shared_ptr<DataEntryBase>> const& entry) {
    bool primary_result = primary_expr->evaluateExpression(entry);

    switch (op) {
        case sql::ast::UnaryOperator::NOT:
            return !primary_result;
        default:
            return false;
    }
}

void UnaryExpressionNode::evaluate(Evaluator &evaluator) {
    std::ignore = evaluator;
}

PrimaryExpressionNode::PrimaryExpressionNode(Position position, Repository &repository) : ExpressionBaseNode(ASTNodeType::PRIMARY_EXPRESSION, position, repository) {}

void PrimaryExpressionNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}

bool PrimaryExpressionNode::evaluateExpression(std::vector<std::shared_ptr<DataEntryBase>> const& entry) {
    std::ignore = entry;
    return false;
}

void PrimaryExpressionNode::evaluate(Evaluator &evaluator) {
    std::ignore = evaluator;
}

