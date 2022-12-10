//
// Created by Maximilian Mittelhammer on 06.12.22.
//

#include "Node.h"
#include "Visitor.h"

using namespace sql::parser;

Node::Node(NodeType type, Position position): type(type), position(position) {}

Position Node::getPosition() const {
    return position;
}

NodeType Node::getType() const {
    return type;
}

NodeStmt::NodeStmt(Position position): Node(NodeType::STMT, position) {}

void NodeStmt::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeSelectStmt::NodeSelectStmt(Position position): Node(NodeType::SELECT_STMT, position) {}

void NodeSelectStmt::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeInsertStmt::NodeInsertStmt(Position position): Node(NodeType::INSERT_STMT, position) {}

void NodeInsertStmt::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeDeleteStmt::NodeDeleteStmt(Position position): Node(NodeType::DELETE_STMT, position) {}

void NodeDeleteStmt::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeUpdateStmt::NodeUpdateStmt(Position position): Node(NodeType::UPDATE_STMT, position) {}

void NodeUpdateStmt::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeCreateStmt::NodeCreateStmt(Position position): Node(NodeType::CREATE_STMT, position) {}

void NodeCreateStmt::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeDropStmt::NodeDropStmt(Position position): Node(NodeType::DROP_STMT, position) {}

void NodeDropStmt::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeAlterStmt::NodeAlterStmt(Position position): Node(NodeType::ALTER_STMT, position) {}

void NodeAlterStmt::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeSelectList::NodeSelectList(Position position): Node(NodeType::SELECT_LIST, position) {}

void NodeSelectList::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeSelectListItem::NodeSelectListItem(Position position): Node(NodeType::SELECT_LIST_ITEM, position) {}

void NodeSelectListItem::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeWhereClause::NodeWhereClause(Position position): Node(NodeType::WHERE_CLAUSE, position) {}

void NodeWhereClause::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeOrderByClause::NodeOrderByClause(Position position): Node(NodeType::ORDER_BY_CLAUSE, position) {}

void NodeOrderByClause::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeOrderByList::NodeOrderByList(Position position): Node(NodeType::ORDER_BY_LIST, position) {}

void NodeOrderByList::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeOrderByItem::NodeOrderByItem(Position position): Node(NodeType::ORDER_BY_ITEM, position) {}

void NodeOrderByItem::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeLimitClause::NodeLimitClause(Position position): Node(NodeType::LIMIT_CLAUSE, position) {}

void NodeLimitClause::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeUpdateList::NodeUpdateList(Position position): Node(NodeType::UPDATE_LIST, position) {}

void NodeUpdateList::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeUpdateListItem::NodeUpdateListItem(Position position): Node(NodeType::UPDATE_LIST_ITEM, position) {}

void NodeUpdateListItem::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeColumnDef::NodeColumnDef(Position position): Node(NodeType::COLUMN_DEF, position) {}

void NodeColumnDef::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeExprList::NodeExprList(Position position): Node(NodeType::EXPR_LIST, position) {}

void NodeExprList::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeSelect::NodeSelect(Position position): Node(NodeType::SELECT, position) {}

void NodeSelect::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeFrom::NodeFrom(Position position): Node(NodeType::FROM, position) {}

void NodeFrom::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeAsteriks::NodeAsteriks(Position position): Node(NodeType::ASTERIKS, position) {}

void NodeAsteriks::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeComma::NodeComma(Position position): Node(NodeType::COMMA, position) {}

void NodeComma::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeAs::NodeAs(Position position): Node(NodeType::AS, position) {}

void NodeAs::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeInsert::NodeInsert(Position position): Node(NodeType::INSERT, position) {}

void NodeInsert::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeInto::NodeInto(Position position): Node(NodeType::INTO, position) {}

void NodeInto::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeBraceOpen::NodeBraceOpen(Position position): Node(NodeType::BRACE_OPEN, position) {}

void NodeBraceOpen::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeBraceClose::NodeBraceClose(Position position): Node(NodeType::BRACE_CLOSE, position) {}

void NodeBraceClose::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeValues::NodeValues(Position position): Node(NodeType::VALUES, position) {}

void NodeValues::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeDelete::NodeDelete(Position position): Node(NodeType::DELETE, position) {}

void NodeDelete::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeUpdate::NodeUpdate(Position position): Node(NodeType::UPDATE, position) {}

void NodeUpdate::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeSet::NodeSet(Position position): Node(NodeType::SET, position) {}

void NodeSet::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeCreate::NodeCreate(Position position): Node(NodeType::CREATE, position) {}

void NodeCreate::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodePrimary::NodePrimary(Position position): Node(NodeType::PRIMARY, position) {}

void NodePrimary::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeKey::NodeKey(Position position): Node(NodeType::KEY, position) {}

void NodeKey::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeNot::NodeNot(Position position): Node(NodeType::NOT, position) {}

void NodeNot::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeNull::NodeNull(Position position): Node(NodeType::_NULL, position) {}

void NodeNull::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeDrop::NodeDrop(Position position): Node(NodeType::DROP, position) {}

void NodeDrop::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeTable::NodeTable(Position position): Node(NodeType::TABLE, position) {}

void NodeTable::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeAdd::NodeAdd(Position position): Node(NodeType::ADD, position) {}

void NodeAdd::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeWhere::NodeWhere(Position position): Node(NodeType::WHERE, position) {}

void NodeWhere::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeOrder::NodeOrder(Position position): Node(NodeType::ORDER, position) {}

void NodeOrder::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeBy::NodeBy(Position position): Node(NodeType::BY, position) {}

void NodeBy::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeAsc::NodeAsc(Position position): Node(NodeType::ASC, position) {}

void NodeAsc::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeDesc::NodeDesc(Position position): Node(NodeType::DESC, position) {}

void NodeDesc::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeLimit::NodeLimit(Position position): Node(NodeType::LIMIT, position) {}

void NodeLimit::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeOr::NodeOr(Position position): Node(NodeType::OR, position) {}

void NodeOr::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeEqual::NodeEqual(Position position): Node(NodeType::EQUAL, position) {}

void NodeEqual::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeNotEqual::NodeNotEqual(Position position): Node(NodeType::NOT_EQUAL, position) {}

void NodeNotEqual::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeLess::NodeLess(Position position): Node(NodeType::LESS, position) {}

void NodeLess::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeLessEqual::NodeLessEqual(Position position): Node(NodeType::LESS_EQUAL, position) {}

void NodeLessEqual::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeGreater::NodeGreater(Position position): Node(NodeType::GREATER, position) {}

void NodeGreater::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeGreaterEqual::NodeGreaterEqual(Position position): Node(NodeType::GREATER_EQUAL, position) {}

void NodeGreaterEqual::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodePlus::NodePlus(Position position): Node(NodeType::PLUS, position) {}

void NodePlus::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeMinus::NodeMinus(Position position): Node(NodeType::MINUS, position) {}

void NodeMinus::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeSlash::NodeSlash(Position position): Node(NodeType::SLASH, position) {}

void NodeSlash::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodePercent::NodePercent(Position position): Node(NodeType::PERCENT, position) {}

void NodePercent::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeInt::NodeInt(Position position): Node(NodeType::INT, position) {}

void NodeInt::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeVarchar::NodeVarchar(Position position): Node(NodeType::VARCHAR, position) {}

void NodeVarchar::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeLiteral::NodeLiteral(Position position, std::string_view value): Node(NodeType::LITERAL, position), value(value) {}

void NodeLiteral::accept(Visitor& visitor) {
    visitor.visit(*this);
}

NodeIdentifier::NodeIdentifier(Position position, std::string_view value): Node(NodeType::IDENTIFIER, position), value(value) {}

void NodeIdentifier::accept(Visitor& visitor) {
    visitor.visit(*this);
}

