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

    if (node.insert_stmt != nullptr) {
        out << "N" << current_node << " -> ";
        node.insert_stmt->accept(*this);
    }

    if (node.update_stmt != nullptr) {
        out << "N" << current_node << " -> ";
        node.update_stmt->accept(*this);
    }

    if (node.delete_stmt != nullptr) {
        out << "N" << current_node << " -> ";
        node.delete_stmt->accept(*this);
    }

    if (node.create_stmt != nullptr) {
        out << "N" << current_node << " -> ";
        node.create_stmt->accept(*this);
    }

    if (node.drop_stmt != nullptr) {
        out << "N" << current_node << " -> ";
        node.drop_stmt->accept(*this);
    }

    if (node.alter_stmt != nullptr) {
        out << "N" << current_node << " -> ";
        node.alter_stmt->accept(*this);
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

    if (node.table_list != nullptr) {
        out << "N" << current_node << " -> ";
        node.table_list->accept(*this);
    }

    if (node.join_clause != nullptr) {
        out << "N" << current_node << " -> ";
        node.join_clause->accept(*this);
    }

    if (node.where_clause != nullptr) {
        out << "N" << current_node << " -> ";
        node.where_clause->accept(*this);
    }

    if (node.order_by_clause != nullptr) {
        out << "N" << current_node << " -> ";
        node.order_by_clause->accept(*this);
    }

    if (node.limit_clause != nullptr) {
        out << "N" << current_node << " -> ";
        node.limit_clause->accept(*this);
    }
}

void PrintVisitor::visit(const NodeInsertStmt& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"insert_stmt\"]" << std::endl;

    if (node._insert != nullptr) {
        out << "N" << current_node << " -> ";
        node._insert->accept(*this);
    }

    if (node._into != nullptr) {
        out << "N" << current_node << " -> ";
        node._into->accept(*this);
    }

    if (node.table_name != nullptr) {
        out << "N" << current_node << " -> ";
        node.table_name->accept(*this);
    }

    if (node.brace_open_1 != nullptr) {
        out << "N" << current_node << " -> ";
        node.brace_open_1->accept(*this);
    }

    if (node.column_list != nullptr) {
        out << "N" << current_node << " -> ";
        node.column_list->accept(*this);
    }

    if (node.brace_close_1 != nullptr) {
        out << "N" << current_node << " -> ";
        node.brace_close_1->accept(*this);
    }

    if (node._values != nullptr) {
        out << "N" << current_node << " -> ";
        node._values->accept(*this);
    }

    if (node.brace_open_2 != nullptr) {
        out << "N" << current_node << " -> ";
        node.brace_open_2->accept(*this);
    }

    if (node.values != nullptr) {
        out << "N" << current_node << " -> ";
        node.values->accept(*this);
    }

    if (node.brace_close_2 != nullptr) {
        out << "N" << current_node << " -> ";
        node.brace_close_2->accept(*this);
    }
}

void PrintVisitor::visit(const NodeDeleteStmt& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"delete_stmt\"]" << std::endl;

    if (node._delete != nullptr) {
        out << "N" << current_node << " -> ";
        node._delete->accept(*this);
    }

    if (node._from != nullptr) {
        out << "N" << current_node << " -> ";
        node._from->accept(*this);
    }

    if (node.table_name != nullptr) {
        out << "N" << current_node << " -> ";
        node.table_name->accept(*this);
    }

    if (node.where_clause != nullptr) {
        out << "N" << current_node << " -> ";
        node.where_clause->accept(*this);
    }
}

void PrintVisitor::visit(const NodeUpdateStmt& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"update_stmt\"]" << std::endl;

    if (node._update != nullptr) {
        out << "N" << current_node << " -> ";
        node._update->accept(*this);
    }

    if (node.table_name != nullptr) {
        out << "N" << current_node << " -> ";
        node.table_name->accept(*this);
    }

    if (node._set != nullptr) {
        out << "N" << current_node << " -> ";
        node._set->accept(*this);
    }

    if (node.update_list != nullptr) {
        out << "N" << current_node << " -> ";
        node.update_list->accept(*this);
    }

    if (node.where_clause != nullptr) {
        out << "N" << current_node << " -> ";
        node.where_clause->accept(*this);
    }
}

void PrintVisitor::visit(const NodeCreateStmt& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"create_stmt\"]" << std::endl;

    if (node._create != nullptr) {
        out << "N" << current_node << " -> ";
        node._create->accept(*this);
    }

    if (node._table != nullptr) {
        out << "N" << current_node << " -> ";
        node._table->accept(*this);
    }

    if (node.table_name != nullptr) {
        out << "N" << current_node << " -> ";
        node.table_name->accept(*this);
    }

    if (node._brace_open != nullptr) {
        out << "N" << current_node << " -> ";
        node._brace_open->accept(*this);
    }

    if (node.column_def != nullptr) {
        out << "N" << current_node << " -> ";
        node.column_def->accept(*this);
    }

    if (node._brace_close != nullptr) {
        out << "N" << current_node << " -> ";
        node._brace_close->accept(*this);
    }
}

void PrintVisitor::visit(const NodeDropStmt& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"drop_stmt\"]" << std::endl;

    if (node._drop != nullptr) {
        out << "N" << current_node << " -> ";
        node._drop->accept(*this);
    }

    if (node._table != nullptr) {
        out << "N" << current_node << " -> ";
        node._table->accept(*this);
    }

    if (node.table_name != nullptr) {
        out << "N" << current_node << " -> ";
        node.table_name->accept(*this);
    }
}

void PrintVisitor::visit(const NodeAlterStmt& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"alter_stmt\"]" << std::endl;

    if (node._alter != nullptr) {
        out << "N" << current_node << " -> ";
        node._alter->accept(*this);
    }

    if (node._table != nullptr) {
        out << "N" << current_node << " -> ";
        node._table->accept(*this);
    }

    if (node.table_name != nullptr) {
        out << "N" << current_node << " -> ";
        node.table_name->accept(*this);
    }

    if (node._add != nullptr) {
        out << "N" << current_node << " -> ";
        node._add->accept(*this);
    }

    if (node.column_def != nullptr) {
        out << "N" << current_node << " -> ";
        node.column_def->accept(*this);
    }
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

    if (node.expr != nullptr) {
        out << "N" << current_node << " -> ";
        node.expr->accept(*this);
    }

    if (node._as != nullptr) {
        out << "N" << current_node << " -> ";
        node._as->accept(*this);
    }

    if (node.identifier != nullptr) {
        out << "N" << current_node << " -> ";
        node.identifier->accept(*this);
    }
}

void PrintVisitor::visit(const NodeTableList& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"table_list\"]" << std::endl;

    for (auto const &table : node.table_list_items) {
        out << "N" << current_node << " -> ";
        table->accept(*this);
    }
}

void PrintVisitor::visit(const NodeTableListItem& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"table_list_item\"]" << std::endl;

    if (node.table_name != nullptr) {
        out << "N" << current_node << " -> ";
        node.table_name->accept(*this);
    }

    if (node.as != nullptr) {
        out << "N" << current_node << " -> ";
        node.as->accept(*this);
    }
}

void PrintVisitor::visit(const NodeJoinClause& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"join_clause\"]" << std::endl;

    if (node.join_type != nullptr) {
        out << "N" << current_node << " -> ";
        node.join_type->accept(*this);
    }

    if (node._join != nullptr) {
        out << "N" << current_node << " -> ";
        node._join->accept(*this);
    }

    if (node.table_list_item != nullptr) {
        out << "N" << current_node << " -> ";
        node.table_list_item->accept(*this);
    }

    if (node._on != nullptr) {
        out << "N" << current_node << " -> ";
        node._on->accept(*this);
    }

    if (node.expr != nullptr) {
        out << "N" << current_node << " -> ";
        node.expr->accept(*this);
    }
}

void PrintVisitor::visit(const NodeWhereClause& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"where_clause\"]" << std::endl;

    if (node._where != nullptr) {
        out << "N" << current_node << " -> ";
        node._where->accept(*this);
    }

    if (node.expr != nullptr) {
        out << "N" << current_node << " -> ";
        node.expr->accept(*this);
    }
}

void PrintVisitor::visit(const NodeOrderClause& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"order_clause\"]" << std::endl;

    if (node._order != nullptr) {
        out << "N" << current_node << " -> ";
        node._order->accept(*this);
    }

    if (node._by != nullptr) {
        out << "N" << current_node << " -> ";
        node._by->accept(*this);
    }

    if (node.order_list != nullptr) {
        out << "N" << current_node << " -> ";
        node.order_list->accept(*this);
    }
}

void PrintVisitor::visit(const NodeOrderList& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"order_list\"]" << std::endl;

    for (auto const &order_list_item : node.order_list_items) {
        out << "N" << current_node << " -> ";
        order_list_item->accept(*this);
    }
}

void PrintVisitor::visit(const NodeOrderListItem& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"order_list_item\"]" << std::endl;

    if (node.expr != nullptr) {
        out << "N" << current_node << " -> ";
        node.expr->accept(*this);
    }

    if (node._asc != nullptr) {
        out << "N" << current_node << " -> ";
        node._asc->accept(*this);
    }

    if (node._desc != nullptr) {
        out << "N" << current_node << " -> ";
        node._desc->accept(*this);
    }
}

void PrintVisitor::visit(const NodeLimitClause& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"limit_clause\"]" << std::endl;

    if (node._limit != nullptr) {
        out << "N" << current_node << " -> ";
        node._limit->accept(*this);
    }

    if (node.expr != nullptr) {
        out << "N" << current_node << " -> ";
        node.expr->accept(*this);
    }
}

void PrintVisitor::visit(const NodeExpression& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"expression\"]" << std::endl;

    if (node.left != nullptr) {
        out << "N" << current_node << " -> ";
        node.left->accept(*this);
    }

    if (node._and != nullptr) {
        out << "N" << current_node << " -> ";
        node._and->accept(*this);
    }

    if (node._or != nullptr) {
        out << "N" << current_node << " -> ";
        node._or->accept(*this);
    }

    if (node._eq != nullptr) {
        out << "N" << current_node << " -> ";
        node._eq->accept(*this);
    }

    if (node._neq != nullptr) {
        out << "N" << current_node << " -> ";
        node._neq->accept(*this);
    }

    if (node._lt != nullptr) {
        out << "N" << current_node << " -> ";
        node._lt->accept(*this);
    }

    if (node._gt != nullptr) {
        out << "N" << current_node << " -> ";
        node._gt->accept(*this);
    }

    if (node._lte != nullptr) {
        out << "N" << current_node << " -> ";
        node._lte->accept(*this);
    }

    if (node._gte != nullptr) {
        out << "N" << current_node << " -> ";
        node._gte->accept(*this);
    }

    if (node.right != nullptr) {
        out << "N" << current_node << " -> ";
        node.right->accept(*this);
    }
}

void PrintVisitor::visit(const NodeUnaryExpression& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"unary_expression\"]" << std::endl;

    if (node._not != nullptr) {
        out << "N" << current_node << " -> ";
        node._not->accept(*this);
    }

    if (node._minus != nullptr) {
        out << "N" << current_node << " -> ";
        node._minus->accept(*this);
    }

    if (node._plus != nullptr) {
        out << "N" << current_node << " -> ";
        node._plus->accept(*this);
    }

    if (node.primary_expr != nullptr) {
        out << "N" << current_node << " -> ";
        node.primary_expr->accept(*this);
    }
}

void PrintVisitor::visit(const NodePrimaryExpression& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"primary_expression\"]" << std::endl;

    if (node._lparen != nullptr) {
        out << "N" << current_node << " -> ";
        node._lparen->accept(*this);
    }

    if (node.expr != nullptr) {
        out << "N" << current_node << " -> ";
        node.expr->accept(*this);
    }

    if (node.ident1 != nullptr) {
        out << "N" << current_node << " -> ";
        node.ident1->accept(*this);
    }

    if (node._dot != nullptr) {
        out << "N" << current_node << " -> ";
        node._dot->accept(*this);
    }

    if (node.ident2 != nullptr) {
        out << "N" << current_node << " -> ";
        node.ident2->accept(*this);
    }

    if (node.identifier != nullptr) {
        out << "N" << current_node << " -> ";
        node.identifier->accept(*this);
    }

    if (node.literal_value != nullptr) {
        out << "N" << current_node << " -> ";
        node.literal_value->accept(*this);
    }

    if (node._rparen != nullptr) {
        out << "N" << current_node << " -> ";
        node._rparen->accept(*this);
    }
}

void PrintVisitor::visit(const NodeUpdateList& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"update_list\"]" << std::endl;

    for (auto const &update_list_item : node.update_list) {
        out << "N" << current_node << " -> ";
        update_list_item->accept(*this);
    }
}

void PrintVisitor::visit(const NodeUpdateListItem& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"update_list_item\"]" << std::endl;

    if (node.identifier != nullptr) {
        out << "N" << current_node << " -> ";
        node.identifier->accept(*this);
    }

    if (node._eq != nullptr) {
        out << "N" << current_node << " -> ";
        node._eq->accept(*this);
    }

    if (node.expr != nullptr) {
        out << "N" << current_node << " -> ";
        node.expr->accept(*this);
    }
}

void PrintVisitor::visit(const NodeValueList& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"value_list\"]" << std::endl;

    if (node._brace_open != nullptr) {
        out << "N" << current_node << " -> ";
        node._brace_open->accept(*this);
    }

    for (auto const &expr : node.value_list) {
        out << "N" << current_node << " -> ";
        expr->accept(*this);
    }

    if (node._brace_close != nullptr) {
        out << "N" << current_node << " -> ";
        node._brace_close->accept(*this);
    }
}

void PrintVisitor::visit(const NodeColumnList& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"column_list\"]" << std::endl;

    if (node._brace_open != nullptr) {
        out << "N" << current_node << " -> ";
        node._brace_open->accept(*this);
    }

    for (auto const &identifier : node.column_list) {
        out << "N" << current_node << " -> ";
        identifier->accept(*this);
    }

    if (node._brace_close != nullptr) {
        out << "N" << current_node << " -> ";
        node._brace_close->accept(*this);
    }
}

void PrintVisitor::visit(const NodeAssignmentList& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"assignment_list\"]" << std::endl;

    if (node._brace_open != nullptr) {
        out << "N" << current_node << " -> ";
        node._brace_open->accept(*this);
    }

    for (auto const &assignment_list_item : node.assignment_list) {
        out << "N" << current_node << " -> ";
        assignment_list_item->accept(*this);
    }

    if (node._brace_close != nullptr) {
        out << "N" << current_node << " -> ";
        node._brace_close->accept(*this);
    }
}

void PrintVisitor::visit(const NodeColumnDef& node) {
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"column_def\"]" << std::endl;

    if (node.identifier != nullptr) {
        out << "N" << current_node << " -> ";
        node.identifier->accept(*this);
    }

    if (node.data_type != nullptr) {
        out << "N" << current_node << " -> ";
        node.data_type->accept(*this);
    }

    if (node._not != nullptr) {
        out << "N" << current_node << " -> ";
        node._not->accept(*this);
    }

    if (node._null != nullptr) {
        out << "N" << current_node << " -> ";
        node._null->accept(*this);
    }
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

void PrintVisitor::visit(const NodeAnd& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"AND\"]" << std::endl;
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

void PrintVisitor::visit(const NodeDot& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"DOT\"]" << std::endl;
}

void PrintVisitor::visit(const NodeFull& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"FULL\"]" << std::endl;
}

void PrintVisitor::visit(const NodeInner& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"INNER\"]" << std::endl;
}

void PrintVisitor::visit(const NodeLeft& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"LEFT\"]" << std::endl;
}

void PrintVisitor::visit(const NodeRight& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"RIGHT\"]" << std::endl;
}

void PrintVisitor::visit(const NodeAlter& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"ALTER\"]" << std::endl;
}

void PrintVisitor::visit(const NodeJoin& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"JOIN\"]" << std::endl;
}

void PrintVisitor::visit(const NodeOn& node) {
    std::ignore = node;
    int current_node = node_count++;
    out << "N" << current_node << std::endl;
    out << "N" << current_node << "[label=\"ON\"]" << std::endl;
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
