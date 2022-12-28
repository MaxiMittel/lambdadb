//
// Created by Maximilian Mittelhammer on 10.12.22.
//

#include "AST.h"
#include "PrintVisitor.h"
#include "../parser/Parser.h"
#include "../code/Repository.h"
#include <cassert>
#include <vector>

using namespace sql::ast;

AST::AST(sql::parser::Parser &parser, Repository &repository) : parser(parser), repository(repository) {}

std::shared_ptr<Node> AST::getRoot() const {
    return root;
}

bool AST::analyze() {
    assert(parser.getRoot()->getType() == parser::NodeType::STMT);
    root = analyzeStatement((parser::NodeStmt*) parser.getRoot().get());
    return success;
}

void AST::print(std::ostream &out) {
    PrintVisitor visitor(out);

    out << "digraph AST {" << std::endl;
    root->accept(visitor);
    out << "}" << std::endl;
}

std::shared_ptr<Node> AST::analyzeStatement(parser::NodeStmt* node) {
    return analyzeStatement((parser::NodeSelectStmt*) node->select_stmt.get());
}

std::shared_ptr<Node> AST::analyzeStatement(parser::NodeSelectStmt* node) {
    auto selectStatement = std::make_shared<SelectStatment>(node->getPosition(), repository);
    
    // Analyze the select columns
    std::vector<ColumnRef> columns;
    if (node->select_list->getType() == parser::NodeType::SELECT_LIST) {
        auto selectList = (parser::NodeSelectList*) node->select_list.get();

        if (selectList->asteriks != nullptr) {
            columns.emplace_back(ColumnRef{true, "", "" ,""});
        } else {
            for (auto& select_list_item : selectList->select_list_items) {
                parser::NodeSelectListItem* selectListItem = static_cast<parser::NodeSelectListItem*>(select_list_item.get());
                parser::NodeIdentifier* expr = static_cast<parser::NodeIdentifier*>(selectListItem->expr.get());
                parser::NodeIdentifier* alias = static_cast<parser::NodeIdentifier*>(selectListItem->identifier.get());

                std::string name = std::string(expr->value);

                if (alias != nullptr) {
                    columns.emplace_back(ColumnRef{false, "", name, std::string(alias->value)});
                } else {
                    columns.emplace_back(ColumnRef{false, "", name, ""});
                }
            }
        }
    }

    selectStatement->setColumns(columns);

    // Analyze the table name
    std::vector<TableRef> tables;
    parser::NodeTableList* tableList = static_cast<parser::NodeTableList*>(node->table_list.get());
    
    for (auto& table: tableList->table_list_items) {
        parser::NodeTableListItem* tableListItem = static_cast<parser::NodeTableListItem*>(table.get());
        parser::NodeIdentifier* tableName = static_cast<parser::NodeIdentifier*>(tableListItem->table_name.get());
        parser::NodeIdentifier* alias = static_cast<parser::NodeIdentifier*>(tableListItem->as.get());

        if (alias != nullptr) {
            tables.emplace_back(TableRef{std::string(tableName->value), std::string(alias->value)});
        } else {
            tables.emplace_back(TableRef{std::string(tableName->value), ""});
        }
    }

    selectStatement->setTables(tables);

    // Analyze join conditions
    if (node->join_clause != nullptr) {
        std::shared_ptr<JoinStatement> joinStatement = std::make_shared<JoinStatement>();
        parser::NodeJoinClause* joinClause = static_cast<parser::NodeJoinClause*>(node->join_clause.get());
        
        switch (joinClause->join_type.get()->getType()) {
            case parser::NodeType::LEFT:
                joinStatement->setType(JoinType::LEFT);
                break;
            case parser::NodeType::RIGHT:
                joinStatement->setType(JoinType::RIGHT);
                break;
            case parser::NodeType::FULL:
                joinStatement->setType(JoinType::FULL);
                break;
            case parser::NodeType::INNER:
                joinStatement->setType(JoinType::INNER);
                break;
            default:
                break;
        }

        parser::NodeTableListItem* tableListItem = static_cast<parser::NodeTableListItem*>(joinClause->table_list_item.get());
        parser::NodeIdentifier* tableName = static_cast<parser::NodeIdentifier*>(tableListItem->table_name.get());
        parser::NodeIdentifier* alias = static_cast<parser::NodeIdentifier*>(tableListItem->as.get());

        if (alias != nullptr) {
            joinStatement->setTable(TableRef{std::string(tableName->value), std::string(alias->value)});
        } else {
            joinStatement->setTable(TableRef{std::string(tableName->value), ""});
        }
        
        parser::NodeExpression* expr = static_cast<parser::NodeExpression*>(joinClause->expr.get());
        parser::NodeUnaryExpression* left = static_cast<parser::NodeUnaryExpression*>(expr->left.get());
        parser::NodeUnaryExpression* right = static_cast<parser::NodeUnaryExpression*>(expr->right.get());
        parser::NodePrimaryExpression* leftPrimary = static_cast<parser::NodePrimaryExpression*>(left->primary_expr.get());
        parser::NodePrimaryExpression* rightPrimary = static_cast<parser::NodePrimaryExpression*>(right->primary_expr.get());

        if (leftPrimary->ident1 != nullptr && leftPrimary->ident2 != nullptr) {
            parser::NodeIdentifier* ident1 = static_cast<parser::NodeIdentifier*>(leftPrimary->ident1.get());
            parser::NodeIdentifier* ident2 = static_cast<parser::NodeIdentifier*>(leftPrimary->ident2.get());
            joinStatement->setLeft(ColumnRef{false, std::string(ident1->value), std::string(ident2->value), ""});
        } else if (leftPrimary->identifier != nullptr) {
            parser::NodeIdentifier* ident = static_cast<parser::NodeIdentifier*>(leftPrimary->identifier.get());
            joinStatement->setLeft(ColumnRef{false, "", std::string(ident->value), ""});
        } else if (leftPrimary->literal_value != nullptr) {
            // TODO: Support literal values
            //joinStatement->setLeftColumn(ColumnRef{false, "", std::string(leftPrimary->literal_value->value), ""});
        }

        if (rightPrimary->ident1 != nullptr && rightPrimary->ident2 != nullptr) {
            parser::NodeIdentifier* ident1 = static_cast<parser::NodeIdentifier*>(rightPrimary->ident1.get());
            parser::NodeIdentifier* ident2 = static_cast<parser::NodeIdentifier*>(rightPrimary->ident2.get());
            joinStatement->setRight(ColumnRef{false, std::string(ident1->value), std::string(ident2->value), ""});
        } else if (rightPrimary->identifier != nullptr) {
            parser::NodeIdentifier* ident = static_cast<parser::NodeIdentifier*>(rightPrimary->identifier.get());
            joinStatement->setRight(ColumnRef{false, "", std::string(ident->value), ""});
        } else if (rightPrimary->literal_value != nullptr) {
            // TODO: Support literal values
            //joinStatement->setRightColumn(ColumnRef{false, "", std::string(rightPrimary->literal_value->value), ""});
        }

        selectStatement->setJoin(joinStatement);
    }

    // Analyze the where clause
    if (node->where_clause != nullptr) {
        selectStatement->setWhere(analyzeStatement(static_cast<parser::NodeWhereClause*>(node->where_clause.get())));
    }

    return selectStatement;
}