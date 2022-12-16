//
// Created by Maximilian Mittelhammer on 16.12.22.
//

#include "PrintVisitor.h"

using namespace sql::ast;

PrintVisitor::PrintVisitor(std::ostream &out) : out(out) {}

void PrintVisitor::visit(const Statement& node) {
    int current_node = node_count++;
    out << "N" << current_node << "[label=\"Statement\"]" << std::endl;

    for (auto const &statement : node.getStatements()) {
        out << "N" << current_node << " -> ";
        statement->accept(*this);
    }
}

void PrintVisitor::visit(const SelectStatment& node) {
    int current_node = node_count++;
    out << "N" << current_node << "[label=\"Select\"]" << std::endl;

    // Print columns
    out << "N" << current_node << " -> ";
    for (auto const &column : node.getColumns()) {
        node_count++;
        out << "N" << node_count << "[label=\"COL#" << column.name << "\"]" << std::endl;
        out << "N" << current_node << " -> N" << node_count << std::endl;
    }

    // Print tables
    out << "N" << current_node << " -> ";
    for (auto const &table : node.getTables()) {
        node_count++;
        out << "N" << node_count << "[label=\"TAB#" << table.name << "\"]" << std::endl;
        out << "N" << current_node << " -> N" << node_count << std::endl;
    }
}

