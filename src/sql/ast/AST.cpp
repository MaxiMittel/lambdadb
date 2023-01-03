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
    if (node->select_stmt != nullptr) {
        return analyzeStatement((parser::NodeSelectStmt*) node->select_stmt.get());
    } else if (node->insert_stmt != nullptr) {
        return analyzeStatement((parser::NodeInsertStmt*) node->insert_stmt.get());
    } else {
        assert(false);
    }
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
            tables.emplace_back(TableRef{tableName->value, alias->value});
        } else {
            tables.emplace_back(TableRef{tableName->value, ""});
        }
    }

    selectStatement->setTables(tables);

    // Analyze join conditions
    if (node->join_clause != nullptr) {
        parser::NodeJoinClause* joinClause = static_cast<parser::NodeJoinClause*>(node->join_clause.get());
        std::shared_ptr<JoinStatement> joinStatement = std::make_shared<JoinStatement>(joinClause->getPosition(), repository);
        
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
            joinStatement->setTable(TableRef{tableName->value, alias->value});
        } else {
            joinStatement->setTable(TableRef{tableName->value, ""});
        }
        
        parser::NodePrimaryExpression* left = static_cast<parser::NodePrimaryExpression*>(joinClause->left.get());
        parser::NodePrimaryExpression* right = static_cast<parser::NodePrimaryExpression*>(joinClause->right.get());

        if (left->ident1 != nullptr && left->ident2 != nullptr) {
            parser::NodeIdentifier* ident1 = static_cast<parser::NodeIdentifier*>(left->ident1.get());
            parser::NodeIdentifier* ident2 = static_cast<parser::NodeIdentifier*>(left->ident2.get());
            joinStatement->setLeft(ColumnRef{false, std::string(ident1->value), std::string(ident2->value), ""});
        } else if (left->ident1 != nullptr) {
            parser::NodeIdentifier* ident = static_cast<parser::NodeIdentifier*>(left->ident1.get());
            joinStatement->setLeft(ColumnRef{false, "", std::string(ident->value), ""});
        }
        // TODO: Support literal values
        //joinStatement->setLeftColumn(ColumnRef{false, "", std::string(left->literal_value->value), ""});
        

        if (right->ident1 != nullptr && right->ident2 != nullptr) {
            parser::NodeIdentifier* ident1 = static_cast<parser::NodeIdentifier*>(right->ident1.get());
            parser::NodeIdentifier* ident2 = static_cast<parser::NodeIdentifier*>(right->ident2.get());
            joinStatement->setRight(ColumnRef{false, std::string(ident1->value), std::string(ident2->value), ""});
        } else if (right->ident1 != nullptr) {
            parser::NodeIdentifier* ident = static_cast<parser::NodeIdentifier*>(right->ident1.get());
            joinStatement->setRight(ColumnRef{false, "", std::string(ident->value), ""});
        } 
        // TODO: Support literal values
        //joinStatement->setRightColumn(ColumnRef{false, "", std::string(right->literal_value->value), ""});

        selectStatement->setJoin(joinStatement);
    }

    // Analyze the where clause
    if (node->where_clause != nullptr) {
        selectStatement->setWhere(analyzeStatement(static_cast<parser::NodeWhereClause*>(node->where_clause.get())));
    }

    return selectStatement;
}

std::shared_ptr<Node> AST::analyzeStatement(parser::NodeWhereClause* node) {
    std::shared_ptr<ExpressionNode> expressionNode = std::make_shared<ExpressionNode>(node->getPosition(), repository);
    parser::NodeExpression* expr = static_cast<parser::NodeExpression*>(node->expr.get());
    parser::NodeAndExpression* andExpr = static_cast<parser::NodeAndExpression*>(expr->and_expr.get());

    expressionNode->setAndExpr(analyzeStatement(static_cast<parser::NodeAndExpression*>(andExpr)));

    return expressionNode;
}

std::shared_ptr<ExpressionBaseNode> AST::analyzeStatement(parser::NodeAndExpression* node) {
    std::shared_ptr<AndExpressionNode> andExpressionNode = std::make_shared<AndExpressionNode>(node->getPosition(), repository);
    parser::NodeOrExpression* orExpr = static_cast<parser::NodeOrExpression*>(node->or_expr.get());
    parser::NodeAndExpression* andExpr = static_cast<parser::NodeAndExpression*>(node->and_expr.get());

    if (orExpr != nullptr) {
        andExpressionNode->setOrExpr(analyzeStatement(static_cast<parser::NodeOrExpression*>(orExpr)));
    }

    if (andExpr != nullptr) {
        andExpressionNode->setAndExpr(analyzeStatement(static_cast<parser::NodeAndExpression*>(andExpr)));
    }

    return andExpressionNode;
}

std::shared_ptr<ExpressionBaseNode> AST::analyzeStatement(parser::NodeOrExpression* node) {
    std::shared_ptr<OrExpressionNode> orExpressionNode = std::make_shared<OrExpressionNode>(node->getPosition(), repository);
    parser::NodeBoolExpression* boolExpr = static_cast<parser::NodeBoolExpression*>(node->bool_expr.get());
    parser::NodeOrExpression* orExpr = static_cast<parser::NodeOrExpression*>(node->or_expr.get());

    if (boolExpr != nullptr) {
        orExpressionNode->setBoolExpr(analyzeStatement(static_cast<parser::NodeBoolExpression*>(boolExpr)));
    }

    if (orExpr != nullptr) {
        orExpressionNode->setOrExpr(analyzeStatement(static_cast<parser::NodeOrExpression*>(orExpr)));
    }

    return orExpressionNode;
}

std::shared_ptr<ExpressionBaseNode> AST::analyzeStatement(parser::NodeBoolExpression* node) {
    std::shared_ptr<BoolExpressionNode> boolExpressionNode = std::make_shared<BoolExpressionNode>(node->getPosition(), repository);
    parser::NodePrimaryExpression* left = static_cast<parser::NodePrimaryExpression*>(node->left.get());
    parser::NodePrimaryExpression* right = static_cast<parser::NodePrimaryExpression*>(node->right.get());

    if (left != nullptr) {
        boolExpressionNode->setLeft(analyzeStatement(static_cast<parser::NodePrimaryExpression*>(left)));
    }

    if (right != nullptr) {
        boolExpressionNode->setRight(analyzeStatement(static_cast<parser::NodePrimaryExpression*>(right)));
    }

    if (node->_equal != nullptr) {
        boolExpressionNode->setOperator(BoolOperator::EQUAL);
    }

    if (node->_not_equal != nullptr) {
        boolExpressionNode->setOperator(BoolOperator::NOT_EQUAL);
    }

    if (node->_less != nullptr) {
        boolExpressionNode->setOperator(BoolOperator::LESS);
    }

    if (node->_greater != nullptr) {
        boolExpressionNode->setOperator(BoolOperator::GREATER);
    }

    if (node->_less_equal != nullptr) {
        boolExpressionNode->setOperator(BoolOperator::LESS_EQUAL);
    }

    if (node->_greater_equal != nullptr) {
        boolExpressionNode->setOperator(BoolOperator::GREATER_EQUAL);
    }

    return boolExpressionNode;
}

std::shared_ptr<ExpressionBaseNode> AST::analyzeStatement(parser::NodeUnaryExpression* node) {
    std::shared_ptr<UnaryExpressionNode> unaryExpressionNode = std::make_shared<UnaryExpressionNode>(node->getPosition(), repository);
    parser::NodePrimaryExpression* primaryExpr = static_cast<parser::NodePrimaryExpression*>(node->primary_expr.get());

    if (node->_not != nullptr) {
        unaryExpressionNode->setOperator(UnaryOperator::NOT);
    }

    if (node->_minus != nullptr) {
        unaryExpressionNode->setOperator(UnaryOperator::MINUS);
    }

    if (primaryExpr != nullptr) {
        unaryExpressionNode->setPrimaryExpr(analyzeStatement(static_cast<parser::NodePrimaryExpression*>(primaryExpr)));
    }
    
    return unaryExpressionNode;
}

std::shared_ptr<ExpressionBaseNode> AST::analyzeStatement(parser::NodePrimaryExpression* node) {
    std::shared_ptr<PrimaryExpressionNode> primaryExpressionNode = std::make_shared<PrimaryExpressionNode>(node->getPosition(), repository);

    if (node->integer_literal != nullptr) {
        parser::NodeIntegerLiteral* integerLiteral = static_cast<parser::NodeIntegerLiteral*>(node->integer_literal.get());
        primaryExpressionNode->setData(std::make_shared<db::DataEntryInteger>(std::stoi(std::string(integerLiteral->value))));
    }

    if (node->string_literal != nullptr) {
        parser::NodeStringLiteral* stringLiteral = static_cast<parser::NodeStringLiteral*>(node->string_literal.get());
        primaryExpressionNode->setData(std::make_shared<db::DataEntryVarchar>(std::string(stringLiteral->value)));
    }

    if (node->ident2 != nullptr && node->ident1 != nullptr) {
        parser::NodeIdentifier* ident1 = static_cast<parser::NodeIdentifier*>(node->ident1.get());
        parser::NodeIdentifier* ident2 = static_cast<parser::NodeIdentifier*>(node->ident2.get());
        primaryExpressionNode->setColumn(std::string(ident1->value), std::string(ident2->value));
    }

    if (node->ident1 != nullptr) {
        parser::NodeIdentifier* ident1 = static_cast<parser::NodeIdentifier*>(node->ident1.get());
        primaryExpressionNode->setColumn("", std::string(ident1->value));
    }

    return primaryExpressionNode;
}

std::shared_ptr<Node> AST::analyzeStatement(parser::NodeInsertStmt* node) {
    std::shared_ptr<InsertStatement> insertStatement = std::make_shared<InsertStatement>(node->getPosition(), repository);
    parser::NodeIdentifier* table_name = static_cast<parser::NodeIdentifier*>(node->table_name.get());

    insertStatement->setTable(std::string(table_name->value));

    if (node->column_list != nullptr) {
        std::vector<std::string> columns;
        parser::NodeColumnList* columnList = static_cast<parser::NodeColumnList*>(node->column_list.get());

        for (auto column_list_item : columnList->column_list) {
            if (column_list_item.get()->getType() == parser::NodeType::IDENTIFIER) {
                parser::NodeIdentifier* column = static_cast<parser::NodeIdentifier*>(column_list_item.get());
                columns.push_back(std::string(column->value));
            }
        }

        insertStatement->setColumns(columns);
    }

    if (node->values != nullptr) {
        std::vector<std::shared_ptr<db::DataEntry>> values;
        parser::NodeValueList* valuesList = static_cast<parser::NodeValueList*>(node->values.get());

        for (auto value_list_item : valuesList->value_list) {
            if (value_list_item.get()->getType() == parser::NodeType::INTEGER_LITERAL) {
                parser::NodeIntegerLiteral* value = static_cast<parser::NodeIntegerLiteral*>(value_list_item.get());
                values.push_back(std::make_shared<db::DataEntryInteger>(std::stoi(std::string(value->value))));
            }

            if (value_list_item.get()->getType() == parser::NodeType::STRING_LITERAL) {
                parser::NodeStringLiteral* value = static_cast<parser::NodeStringLiteral*>(value_list_item.get());
                values.push_back(std::make_shared<db::DataEntryVarchar>(std::string(value->value)));
            }
        }

        insertStatement->setValues(values);
    }
    

    return insertStatement;
}