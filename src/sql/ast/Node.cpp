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

