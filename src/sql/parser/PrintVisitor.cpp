//
// Created by Maximilian Mittelhammer on 10.12.22.
//

#include "PrintVisitor.h"
#include <iostream>

using namespace sql::parser;

PrintVisitor::PrintVisitor(std::ostream &out) : out(out) {}

void PrintVisitor::visit(const NodeStmt& node) {
    int current_node = node_count++;
    out << "N" << current_node << "[label=\"stmt\"]" << std::endl;

    if (node.select_stmt != nullptr) {
        out << "N" << current_node << " -> ";
        node.select_stmt->accept(*this);
    }
}

void PrintVisitor::visit(const NodeSelectStmt& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"select_stmt\"]" << std::endl;

    if (node._select != nullptr) {
        out << "N" << current_node << " -> ";
        node._select->accept(*this);
    }

    if (node.select_list != nullptr) {
        out << "N" << current_node << " -> ";
        node.select_list->accept(*this);
    }

    if (node._from != nullptr) {
        out << "N" << current_node << " -> ";
        node._from->accept(*this);
    }

    if (node.table_name != nullptr) {
        out << "N" << current_node << " -> ";
        node.table_name->accept(*this);
    }
}

void PrintVisitor::visit(const NodeInsertStmt& node) {
    std::ignore = node;
}

void PrintVisitor::visit(const NodeDeleteStmt& node) {
    std::ignore = node;
}

void PrintVisitor::visit(const NodeUpdateStmt& node) {
    std::ignore = node;
}

void PrintVisitor::visit(const NodeCreateStmt& node) {
    std::ignore = node;
}

void PrintVisitor::visit(const NodeDropStmt& node) {
    std::ignore = node;
}

void PrintVisitor::visit(const NodeAlterStmt& node) {
    std::ignore = node;
}

void PrintVisitor::visit(const NodeSelectList& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"select_list\"]" << std::endl;

    if (node.asteriks != nullptr) {
        out << "N" << current_node << " -> ";
        node.asteriks->accept(*this);
    }

    for (auto const &select_list_item : node.select_list_items) {
        out << "N" << current_node << " -> ";
        select_list_item->accept(*this);
    }
}

void PrintVisitor::visit(const NodeSelectListItem& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"select_list_item\"]" << std::endl;

    if (node.identifier != nullptr) {
        out << "N" << current_node << " -> ";
        node.identifier->accept(*this);
    }
}

void PrintVisitor::visit(const NodeWhereClause& node) {
    std::ignore = node;
}

void PrintVisitor::visit(const NodeOrderByClause& node) {
    std::ignore = node;
}

void PrintVisitor::visit(const NodeOrderByList& node) {
    std::ignore = node;
}

void PrintVisitor::visit(const NodeOrderByItem& node) {
    std::ignore = node;
}

void PrintVisitor::visit(const NodeLimitClause& node) {
    std::ignore = node;
}

void PrintVisitor::visit(const NodeUpdateList& node) {
    std::ignore = node;
}

void PrintVisitor::visit(const NodeUpdateListItem& node) {
    std::ignore = node;
}

void PrintVisitor::visit(const NodeColumnDef& node) {
    std::ignore = node;
}

void PrintVisitor::visit(const NodeExprList& node) {
    std::ignore = node;
}


void PrintVisitor::visit(const NodeSelect& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"SELECT\"]" << std::endl;
}

void PrintVisitor::visit(const NodeFrom& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"FROM\"]" << std::endl;
}

void PrintVisitor::visit(const NodeAsteriks& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"ASTERIKS\"]" << std::endl;
}

void PrintVisitor::visit(const NodeComma& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"COMMA\"]" << std::endl;
}

void PrintVisitor::visit(const NodeAs& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"AS\"]" << std::endl;
}

void PrintVisitor::visit(const NodeInsert& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"INSERT\"]" << std::endl;
}

void PrintVisitor::visit(const NodeInto& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"INTO\"]" << std::endl;
}

void PrintVisitor::visit(const NodeBraceOpen& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"BRACE_OPEN\"]" << std::endl;
}

void PrintVisitor::visit(const NodeBraceClose& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"BRACE_CLOSE\"]" << std::endl;
}

void PrintVisitor::visit(const NodeValues& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"VALUES\"]" << std::endl;
}

void PrintVisitor::visit(const NodeDelete& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"DELETE\"]" << std::endl;
}

void PrintVisitor::visit(const NodeUpdate& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"UPDATE\"]" << std::endl;
}

void PrintVisitor::visit(const NodeSet& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"SET\"]" << std::endl;
}

void PrintVisitor::visit(const NodeCreate& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"CREATE\"]" << std::endl;
}

void PrintVisitor::visit(const NodeTable& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"TABLE\"]" << std::endl;
}

void PrintVisitor::visit(const NodePrimary& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"PRIMARY\"]" << std::endl;
}

void PrintVisitor::visit(const NodeKey& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"KEY\"]" << std::endl;
}

void PrintVisitor::visit(const NodeNot& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"NOT\"]" << std::endl;
}

void PrintVisitor::visit(const NodeNull& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"NULL\"]" << std::endl;
}

void PrintVisitor::visit(const NodeDrop& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"DROP\"]" << std::endl;
}

void PrintVisitor::visit(const NodeAdd& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"ADD\"]" << std::endl;
}

void PrintVisitor::visit(const NodeWhere& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"WHERE\"]" << std::endl;
}

void PrintVisitor::visit(const NodeOrder& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"ORDER\"]" << std::endl;
}

void PrintVisitor::visit(const NodeBy& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"BY\"]" << std::endl;
}

void PrintVisitor::visit(const NodeAsc& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"ASC\"]" << std::endl;
}

void PrintVisitor::visit(const NodeDesc& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"DESC\"]" << std::endl;
}

void PrintVisitor::visit(const NodeLimit& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"LIMIT\"]" << std::endl;
}

void PrintVisitor::visit(const NodeOr& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"OR\"]" << std::endl;
}

void PrintVisitor::visit(const NodeEqual& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"EQUAL\"]" << std::endl;
}

void PrintVisitor::visit(const NodeNotEqual& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"NOT_EQUAL\"]" << std::endl;
}

void PrintVisitor::visit(const NodeLess& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"LESS\"]" << std::endl;
}

void PrintVisitor::visit(const NodeGreater& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"GREATER\"]" << std::endl;
}

void PrintVisitor::visit(const NodeLessEqual& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"LESS_EQUAL\"]" << std::endl;
}

void PrintVisitor::visit(const NodeGreaterEqual& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"GREATER_EQUAL\"]" << std::endl;
}

void PrintVisitor::visit(const NodePlus& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"PLUS\"]" << std::endl;
}

void PrintVisitor::visit(const NodeMinus& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"MINUS\"]" << std::endl;
}

void PrintVisitor::visit(const NodeSlash& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"SLASH\"]" << std::endl;
}

void PrintVisitor::visit(const NodePercent& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"PERCENT\"]" << std::endl;
}

void PrintVisitor::visit(const NodeInt& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"INT\"]" << std::endl;
}

void PrintVisitor::visit(const NodeVarchar& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"VARCHAR\"]" << std::endl;
}

void PrintVisitor::visit(const NodeLiteral& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"literal\"]" << std::endl;
    out << "NI" << (current_node + 1) << "[label=\"" << node.value << "\"]" << std::endl;
    out << "N" << current_node << " -> "
           << "NI" << (current_node + 1) << std::endl;
    current_node++;
}

void PrintVisitor::visit(const NodeIdentifier& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"identifier\"]" << std::endl;
    out << "NI" << (current_node + 1) << "[label=\"'" << node.value << "'\"]" << std::endl;
    out << "N" << current_node << " -> "
           << "NI" << (current_node + 1) << std::endl;
    current_node++;
}