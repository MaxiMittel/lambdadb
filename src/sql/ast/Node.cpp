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
        std::shared_ptr<db::Table> tableRef = evaluator.getDatabase().getTable(std::string(table.name));
        
        if (evaluator.getEvaluationTable().get()->getRowCount() == 0) {
            evaluator.getEvaluationTable().get()->setColumns(tableRef->getColumns());
            evaluator.getEvaluationTable().get()->setItems(tableRef->getItems());
        } else {
            // Merge tables
            std::vector<db::Column> newColumns;
            newColumns.insert(newColumns.end(), evaluator.getEvaluationTable().get()->getColumns().begin(), evaluator.getEvaluationTable().get()->getColumns().end());
            newColumns.insert(newColumns.end(), tableRef->getColumns().begin(), tableRef->getColumns().end());
            
            // Cross join
            std::vector<std::vector<std::shared_ptr<db::DataEntry>>> newItems;
            for (auto &item1 : evaluator.getEvaluationTable().get()->getItems()) {
                for (auto &item2 : tableRef->getItems()) {
                    std::vector<std::shared_ptr<db::DataEntry>> newItem;
                    newItem.insert(newItem.end(), item1.begin(), item1.end());
                    newItem.insert(newItem.end(), item2.begin(), item2.end());
                    newItems.emplace_back(newItem);
                }
            }

            evaluator.getEvaluationTable().get()->setItems(newItems);
        }
    }

    if (join) {
        join->evaluate(evaluator);
    }

    if (where) {
        std::vector<std::vector<std::shared_ptr<db::DataEntry>>> newItems;

        for (auto &item : evaluator.getEvaluationTable().get()->getItems()) {
            bool res = static_cast<ExpressionNode*>(where.get())->evaluateExpression(evaluator, item);
            if (res) {
                newItems.emplace_back(item);
            }
        }

        evaluator.getEvaluationTable().get()->setItems(newItems);
    }

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
    // TODO: Check if key is in left or right of the expression
    std::unordered_map<std::string, std::vector<std::shared_ptr<db::DataEntry>>> items = evaluator.getDatabase().getTable(std::string(this->getTable().name)).get()->getItems(this->getRight().name);

    std::vector<std::vector<std::shared_ptr<db::DataEntry>>> newItems;

    // TODO: Check if key exists in table

    size_t key_index = 0;
    for (auto &column : evaluator.getEvaluationTable().get()->getColumns()) {
        if (column.name == this->getLeft().name) {
            break;
        }
        key_index++;
    }

    switch (type) {
        case JoinType::LEFT:
            for (auto &item : evaluator.getEvaluationTable().get()->getItems()) {
                std::vector<std::shared_ptr<db::DataEntry>> newItem;

                // Check if key exists in map
                bool found = items.count(item[key_index].get()->toString()) > 0;

                if (found) {
                    newItem.insert(newItem.end(), item.begin(), item.end());
                    newItem.insert(newItem.end(), items[item[key_index].get()->toString()].begin(), items[item[key_index].get()->toString()].end());
                } else {
                    newItem.insert(newItem.end(), item.begin(), item.end());
                    for (size_t i = 0; i < evaluator.getDatabase().getTable(std::string(this->getTable().name)).get()->getColumns().size(); i++) {
                        newItem.emplace_back(std::make_shared<db::DataEntryNull>());
                    }
                }

                newItems.emplace_back(newItem);
            }
            break;
        case JoinType::RIGHT:
            for (auto &item : evaluator.getEvaluationTable().get()->getItems()) {
                std::vector<std::shared_ptr<db::DataEntry>> newItem;

                // Check if key exists in map
                bool found = items.count(item[key_index].get()->toString()) > 0;

                if (found) {
                    newItem.insert(newItem.end(), items[item[key_index].get()->toString()].begin(), items[item[key_index].get()->toString()].end());
                    newItem.insert(newItem.end(), item.begin(), item.end());
                } else {
                    for (size_t i = 0; i < evaluator.getDatabase().getTable(std::string(this->getTable().name)).get()->getColumns().size(); i++) {
                        newItem.emplace_back(std::make_shared<db::DataEntryNull>());
                    }
                    newItem.insert(newItem.end(), item.begin(), item.end());
                }

                newItems.emplace_back(newItem);
            }
            break;
        case JoinType::INNER:
            for (auto &item : evaluator.getEvaluationTable().get()->getItems()) {
                std::vector<std::shared_ptr<db::DataEntry>> newItem;

                // Check if key exists in map
                bool found = items.count(item[key_index].get()->toString()) > 0;

                if (found) {
                    newItem.insert(newItem.end(), item.begin(), item.end());
                    newItem.insert(newItem.end(), items[item[key_index].get()->toString()].begin(), items[item[key_index].get()->toString()].end());
                }

                newItems.emplace_back(newItem);
            }
            break;
        case JoinType::FULL:
            // TODO: Implement
            break;
        default:
            break;
    }

    evaluator.getEvaluationTable().get()->setItems(newItems);
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

bool ExpressionNode::evaluateExpression(Evaluator &evaluator,  std::vector<std::shared_ptr<db::DataEntry>> const& entry) {
    std::ignore = entry;
    std::ignore = evaluator;
    return and_expr->evaluateExpression(evaluator, entry);
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

bool AndExpressionNode::evaluateExpression(Evaluator &evaluator,  std::vector<std::shared_ptr<db::DataEntry>> const& entry) {
    bool or_result = or_expr->evaluateExpression(evaluator, entry);
    if (!or_result) return false;

    if (and_expr != nullptr) {
        bool and_result = and_expr->evaluateExpression(evaluator, entry);
        return and_result;
    }

    return true;
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

bool OrExpressionNode::evaluateExpression(Evaluator &evaluator,  std::vector<std::shared_ptr<db::DataEntry>> const& entry) {
    bool bool_result = bool_expr->evaluateExpression(evaluator, entry);
    if (bool_result) return true;

    if (or_expr != nullptr) {
        bool or_result = or_expr->evaluateExpression(evaluator, entry);
        return or_result;
    }

    return false;
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

bool BoolExpressionNode::evaluateExpression(Evaluator &evaluator,  std::vector<std::shared_ptr<db::DataEntry>> const& entry) {
    std::ignore = entry;
    PrimaryExpressionNode* left_primary = static_cast<PrimaryExpressionNode*>(left.get());
    PrimaryExpressionNode* right_primary = static_cast<PrimaryExpressionNode*>(right.get());

    int result = left_primary->getData(evaluator, entry)->compare(*right_primary->getData(evaluator, entry).get());

    switch (op) {
        case sql::ast::BoolOperator::EQUAL:
            return result == 0;
        case sql::ast::BoolOperator::NOT_EQUAL:
            return result != 0;
        case sql::ast::BoolOperator::LESS:
            return result < 0;
        case sql::ast::BoolOperator::LESS_EQUAL:
            return result <= 0;
        case sql::ast::BoolOperator::GREATER:
            return result > 0;
        case sql::ast::BoolOperator::GREATER_EQUAL:
            return result >= 0;
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

bool UnaryExpressionNode::evaluateExpression(Evaluator &evaluator,  std::vector<std::shared_ptr<db::DataEntry>> const& entry) {
    bool primary_result = primary_expr->evaluateExpression(evaluator, entry);

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

void PrimaryExpressionNode::setColumn(std::string table, std::string column) {
    this->table = table;
    this->column = column;
}

std::string PrimaryExpressionNode::getTable() const {
    return table;
}

std::string PrimaryExpressionNode::getColumn() const {
    return column;
}

void PrimaryExpressionNode::setData(std::shared_ptr<db::DataEntry> data) {
    this->data = data;
}

std::shared_ptr<db::DataEntry> PrimaryExpressionNode::getData(Evaluator &evaluator, std::vector<std::shared_ptr<db::DataEntry>> const& entry) const {
    if (data != nullptr) return data;
    if (column != "") {
        if (table != "") {
            for (size_t i = 0; i < evaluator.getEvaluationTable().get()->getColumns().size(); i++) {
                auto column_ref = evaluator.getEvaluationTable().get()->getColumns()[i];
                if (column_ref.name == column && column_ref.table == table) {
                    return entry[i];
                }
            }
        }

        for (size_t i = 0; i < evaluator.getEvaluationTable().get()->getColumns().size(); i++) {
            auto column_ref = evaluator.getEvaluationTable().get()->getColumns()[i];
            if (column_ref.name == column) {
                return entry[i];
            }
        }
    }
    
    return std::make_shared<db::DataEntryNull>();
}

void PrimaryExpressionNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}

bool PrimaryExpressionNode::evaluateExpression(Evaluator &evaluator,  std::vector<std::shared_ptr<db::DataEntry>> const& entry) {
    std::ignore = entry;
    std::ignore = evaluator;
    return false;
}

void PrimaryExpressionNode::evaluate(Evaluator &evaluator) {
    std::ignore = evaluator;
}

InsertStatement::InsertStatement(Position position, Repository &repository) : Node(ASTNodeType::INSERT_STATEMENT, position, repository) {}

void InsertStatement::setTable(std::string table) {
    this->table = table;
}

std::string InsertStatement::getTable() const {
    return table;
}

void InsertStatement::setColumns(std::vector<std::string> columns) {
    this->columns = columns;
}

std::vector<std::string> InsertStatement::getColumns() const {
    return columns;
}

void InsertStatement::setValues(std::vector<std::shared_ptr<db::DataEntry>> values) {
    this->values = values;
}

std::vector<std::shared_ptr<db::DataEntry>> InsertStatement::getValues() const {
    return values;
}

void InsertStatement::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void InsertStatement::evaluate(Evaluator &evaluator) {
    std::vector<db::Column> table_columns = evaluator.getDatabase().getTable(table).get()->getColumns();

    std::vector<std::shared_ptr<db::DataEntry>> data;
    for (size_t i = 0; i < table_columns.size(); i++) {
        for (size_t j = 0; j < columns.size(); j++) {
            if (table_columns[i].name == columns[j]) {
                data.push_back(values[j]);
            }
        }
    }

    evaluator.getDatabase().getTable(table).get()->insertItem(data);
}