//
// Created by Maximilian Mittelhammer on 16.12.22.
//

#include "PrintVisitor.h"

using namespace sql::ast;

PrintVisitor::PrintVisitor(std::ostream &out) : out(out) {}

void PrintVisitor::visit(const Statement& node) {
    int current_node = ++node_count;
    out << "N" << current_node << "[label=\"Statement\"]" << std::endl;

    for (auto const &statement : node.getStatements()) {
        out << "N" << current_node << " -> ";
        statement->accept(*this);
    }
}

void PrintVisitor::visit(const SelectStatment& node) {
    int current_node = ++node_count;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"Select\"]" << std::endl;

    // Print columns
    node_count++;
    int columns_node = node_count;
    out << "N" << current_node << " -> N" << columns_node << std::endl;
    out << "N" << columns_node << "[label=\"Columns\"]" << std::endl;
    for (auto const &column : node.getColumns()) {
        node_count++;
        out << "N" << columns_node << " -> N" << node_count << std::endl;
        out << "N" << node_count << "[label=\"" << column.name << "\"]" << std::endl;
    }

    // Print tables
    node_count++;
    int tables_node = node_count;
    out << "N" << current_node << " -> N" << tables_node << std::endl;
    out << "N" << tables_node << "[label=\"Tables\"]" << std::endl;
    for (auto const &table : node.getTables()) {
        node_count++;
        out << "N" << tables_node << " -> N" << node_count << std::endl;
        out << "N" << node_count << "[label=\"" << table.name << "\"]" << std::endl;
    }

    if (node.getWhere() != nullptr) {
        out << "N" << current_node << " -> ";
        node.getWhere()->accept(*this);
    }

    if (node.getJoin() != nullptr) {
        out << "N" << current_node << " -> ";
        node.getJoin()->accept(*this);
    }
}

void PrintVisitor::visit(const JoinStatement& node) {
    int current_node = ++node_count;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"Join\"]" << std::endl;

    // Print type
    node_count++;
    int type_node = node_count;
    out << "N" << current_node << " -> N" << type_node << std::endl;
    out << "N" << type_node << "[label=\"Type\"]" << std::endl;
    node_count++;
    out << "N" << node_count - 1 << " -> N" << node_count << std::endl;
    out << "N" << node_count << "[label=\"" << node.getType() << "\"]" << std::endl;

    // Print table
    node_count++;
    int table_node = node_count;
    out << "N" << current_node << " -> N" << table_node << std::endl;
    out << "N" << table_node << "[label=\"Table\"]" << std::endl;
    node_count++;
    out << "N" << node_count - 1 << " -> N" << node_count << std::endl;
    out << "N" << node_count << "[label=\"" << node.getTable().name << "\"]" << std::endl;

    // Print left
    node_count++;
    int left_node = node_count;
    out << "N" << current_node << " -> N" << left_node << std::endl;
    out << "N" << left_node << "[label=\"Left\"]" << std::endl;
    node_count++;
    out << "N" << node_count - 1 << " -> N" << node_count << std::endl;
    out << "N" << node_count << "[label=\"" << node.getLeft().name << "\"]" << std::endl;

    // Print right
    node_count++;
    int right_node = node_count;
    out << "N" << current_node << " -> N" << right_node << std::endl;
    out << "N" << right_node << "[label=\"Right\"]" << std::endl;
    node_count++;
    out << "N" << node_count - 1 << " -> N" << node_count << std::endl;
    out << "N" << node_count << "[label=\"" << node.getRight().name << "\"]" << std::endl;
}

void PrintVisitor::visit(const ExpressionNode& node) {
    int current_node = ++node_count;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"Expression\"]" << std::endl;

    if (node.getAndExpr() != nullptr) {
        out << "N" << current_node << " -> ";
        node.getAndExpr()->accept(*this);
    }
}

void PrintVisitor::visit(const AndExpressionNode& node) {
    int current_node = ++node_count;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"And\"]" << std::endl;

    if (node.getOrExpr() != nullptr) {
        out << "N" << current_node << " -> ";
        node.getOrExpr()->accept(*this);
    }

    if (node.getAndExpr() != nullptr) {
        out << "N" << current_node << " -> ";
        node.getAndExpr()->accept(*this);
    }
}

void PrintVisitor::visit(const OrExpressionNode& node) {
    int current_node = ++node_count;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"Or\"]" << std::endl;

    if (node.getBoolExpr() != nullptr) {
        out << "N" << current_node << " -> ";
        node.getBoolExpr()->accept(*this);
    }

    if (node.getOrExpr() != nullptr) {
        out << "N" << current_node << " -> ";
        node.getOrExpr()->accept(*this);
    }
}

void PrintVisitor::visit(const BoolExpressionNode& node) {
    int current_node = ++node_count;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"Bool\"]" << std::endl;

    if (node.getLeft() != nullptr) {
        out << "N" << current_node << " -> ";
        node.getLeft()->accept(*this);
    }

    // Operator
    node_count++;
    int op_node = node_count;
    out << "N" << current_node << " -> N" << op_node << std::endl;
    out << "N" << op_node << "[label=\"Operator\"]" << std::endl;
    node_count++;
    out << "N" << node_count - 1 << " -> N" << node_count << std::endl;
    out << "N" << node_count << "[label=\"" << node.getOperator() << "\"]" << std::endl;

    if (node.getRight() != nullptr) {
        out << "N" << current_node << " -> ";
        node.getRight()->accept(*this);
    }
}

void PrintVisitor::visit(const UnaryExpressionNode& node) {
    int current_node = ++node_count;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"Unary\"]" << std::endl;

    // Operator
    node_count++;
    int op_node = node_count;
    out << "N" << current_node << " -> N" << op_node << std::endl;
    out << "N" << op_node << "[label=\"Operator\"]" << std::endl;
    node_count++;
    out << "N" << node_count - 1 << " -> N" << node_count << std::endl;
    out << "N" << node_count << "[label=\"" << node.getOperator() << "\"]" << std::endl;

    if (node.getPrimaryExpr() != nullptr) {
        out << "N" << current_node << " -> ";
        node.getPrimaryExpr()->accept(*this);
    }
}

void PrintVisitor::visit(const PrimaryExpressionNode& node) {
    std::ignore = node;
    int current_node = ++node_count;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"Primary\"]" << std::endl;
}