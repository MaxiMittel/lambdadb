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

std::vector<ColumnRef> SelectStatment::getColumns() const {
    return columns;
}

std::vector<TableRef> SelectStatment::getTables() const {
    return tables;
}

void SelectStatment::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void SelectStatment::evaluate(Evaluator &evaluator) {
    for (auto &table : tables) {
        std::shared_ptr<Table> tableRef = evaluator.getDatabase().getTable(table.name);
        
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
                    newItems.push_back(newItem);
                }
            }

            evaluator.getEvaluationTable().get()->setItems(newItems);
        }
    }

    // TODO: Evaluate WHERE clause

    // TODO: Project columns, maybe in the JSON output
}