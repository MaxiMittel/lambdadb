//
// Created by Maximilian Mittelhammer on 10.12.22.
//

#include "AST.h"
#include <cassert>

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
                parser::NodeSelectListItem* selectListItem = (parser::NodeSelectListItem*) select_list_item.get();
                parser::NodeIdentifier* expr = (parser::NodeIdentifier*) selectListItem->expr.get();
                parser::NodeIdentifier* alias = (parser::NodeIdentifier*) selectListItem->identifier.get();

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
    parser::NodeIdentifier* tableName = (parser::NodeIdentifier*) node->table_name.get();

    tables.emplace_back(TableRef{std::string(tableName->value), ""});
    selectStatement->setTables(tables);

    return selectStatement;
}