//
// Created by Maximilian Mittelhammer on 05.12.22.
//

#ifndef LAMBDADB_PARSER_NODE_H
#define LAMBDADB_PARSER_NODE_H

#include <memory>
#include <string_view>
#include <vector>
#include "../code/Repository.h"

namespace sql::parser {

class Visitor;

enum NodeType {
    // Non-terminals
    STMT,
    SELECT_STMT,
    INSERT_STMT,
    DELETE_STMT,
    UPDATE_STMT,
    CREATE_STMT,
    DROP_STMT,
    ALTER_STMT,
    SELECT_LIST,
    TABLE_NAME,
    WHERE_CLAUSE,
    ORDER_CLAUSE,
    LIMIT_CLAUSE,
    SELECT_LIST_ITEM,
    EXPR,
    COLUMN_LIST,
    EXPR_LIST,
    UPDATE_LIST,
    UPDATE_LIST_ITEM,
    COLUMN_NAME,
    COLUMN_DEF,
    COLUMN_TYPE,
    COLUMN_CONSTRAINT,
    ORDER_BY_LIST,
    ORDER_LIST_ITEM,
    EXPRESSION,
    UNARY_EXPRESSION,
    PRIMARY_EXPRESSION,
    TABLE_LIST,
    TABLE_LIST_ITEM,
    JOIN_CLAUSE,
    VALUE_LIST,
    ASSIGNMENT_LIST,
    ASSIGNMENT_LIST_ITEM,

    // Terminals
    SELECT,
    FROM,
    ASTERIKS,
    COMMA,
    AS,
    INSERT,
    INTO,
    BRACE_OPEN,
    BRACE_CLOSE,
    VALUES,
    DELETE,
    UPDATE,
    SET,
    CREATE,
    TABLE,
    PRIMARY,
    KEY,
    NOT,
    _NULL,
    DROP,
    ALTER,
    ADD,
    WHERE,
    ORDER,
    BY,
    ASC,
    DESC,
    LIMIT,
    OR,
    AND,
    EQUAL,
    NOT_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,
    PLUS,
    MINUS,
    SLASH,
    PERCENT,
    INNER,
    LEFT,
    RIGHT,
    FULL,
    JOIN,
    ON,
    DOT,
    INTEGER_LITERAL,
    FLOAT_LITERAL,
    STRING_LITERAL,
    IDENTIFIER,

    // Datatypes
    INT,
    VARCHAR
};

class Node {
    private:
    NodeType type;
    Position position;

    public:
    Node(NodeType type, Position position);
    virtual ~Node() = default;
    
    /**
     * Get the type of the node
     * @return The type of the node
     */
    NodeType getType() const;

    /**
     * Get the position of the node
     * @return The position of the node
     */
    Position getPosition() const;

    /**
     * Accept a visitor
     * @param visitor The visitor
     */
    virtual void accept(Visitor& visitor) = 0;
};

class NodeStmt: public Node {
    public:
    NodeStmt(Position position);
    std::shared_ptr<Node> select_stmt = nullptr;
    std::shared_ptr<Node> insert_stmt = nullptr;
    std::shared_ptr<Node> delete_stmt = nullptr;
    std::shared_ptr<Node> update_stmt = nullptr;
    std::shared_ptr<Node> create_stmt = nullptr;
    std::shared_ptr<Node> drop_stmt = nullptr;
    std::shared_ptr<Node> alter_stmt = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeSelectStmt: public Node {
    public:
    NodeSelectStmt(Position position);
    std::shared_ptr<Node> _select = nullptr;
    std::shared_ptr<Node> select_list = nullptr;
    std::shared_ptr<Node> _from = nullptr;
    std::shared_ptr<Node> table_list = nullptr;

    // Optional
    std::shared_ptr<Node> join_clause = nullptr;
    std::shared_ptr<Node> where_clause = nullptr;
    std::shared_ptr<Node> order_by_clause = nullptr;
    std::shared_ptr<Node> limit_clause = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeInsertStmt: public Node {
    public:
    NodeInsertStmt(Position position);
    std::shared_ptr<Node> _insert = nullptr;
    std::shared_ptr<Node> _into = nullptr;
    std::shared_ptr<Node> table_name = nullptr;

    // Optional
    std::shared_ptr<Node> brace_open_1 = nullptr;
    std::shared_ptr<Node> column_list = nullptr;
    std::shared_ptr<Node> brace_close_1 = nullptr;
    // Optional end

    std::shared_ptr<Node> _values = nullptr;
    std::shared_ptr<Node> brace_open_2 = nullptr;
    std::shared_ptr<Node> values = nullptr;
    std::shared_ptr<Node> brace_close_2 = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeDeleteStmt: public Node {
    public:
    NodeDeleteStmt(Position position);
    std::shared_ptr<Node> _delete = nullptr;
    std::shared_ptr<Node> _from = nullptr;
    std::shared_ptr<Node> table_name = nullptr;

    // Optional
    std::shared_ptr<Node> where_clause = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeUpdateStmt: public Node {
    public:
    NodeUpdateStmt(Position position);
    std::shared_ptr<Node> _update = nullptr;
    std::shared_ptr<Node> table_name = nullptr;
    std::shared_ptr<Node> _set = nullptr;
    std::shared_ptr<Node> update_list = nullptr;
    std::shared_ptr<Node> where_clause = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeCreateStmt: public Node {
    public:
    NodeCreateStmt(Position position);
    std::shared_ptr<Node> _create = nullptr;
    std::shared_ptr<Node> _table = nullptr;
    std::shared_ptr<Node> table_name = nullptr;
    std::shared_ptr<Node> _brace_open = nullptr;
    std::shared_ptr<Node> column_def = nullptr; // TODO: Check if this is correct
    std::shared_ptr<Node> _brace_close = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeDropStmt: public Node {
    public:
    NodeDropStmt(Position position);
    std::shared_ptr<Node> _drop = nullptr;
    std::shared_ptr<Node> _table = nullptr;
    std::shared_ptr<Node> table_name = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeAlterStmt: public Node {
    public:
    NodeAlterStmt(Position position);
    std::shared_ptr<Node> _alter = nullptr;
    std::shared_ptr<Node> _table = nullptr;
    std::shared_ptr<Node> table_name = nullptr;
    std::shared_ptr<Node> _add = nullptr;
    std::shared_ptr<Node> column_def = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeSelectList: public Node {
    public:
    NodeSelectList(Position position);
    std::shared_ptr<Node> asteriks = nullptr;

    // Or
    std::vector<std::shared_ptr<Node>> select_list_items;

    void accept(Visitor& visitor) override;
};

class NodeSelectListItem: public Node {
    public:
    NodeSelectListItem(Position position);
    std::shared_ptr<Node> expr = nullptr;

    // Optional
    std::shared_ptr<Node> _as = nullptr;
    std::shared_ptr<Node> identifier = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeTableList: public Node {
    public:
    NodeTableList(Position position);
    std::vector<std::shared_ptr<Node>> table_list_items;

    void accept(Visitor& visitor) override;
};

class NodeTableListItem: public Node {
    public:
    NodeTableListItem(Position position);
    std::shared_ptr<Node> table_name = nullptr;

    // Optional
    std::shared_ptr<Node> as = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeJoinClause: public Node {
    public:
    NodeJoinClause(Position position);
    std::shared_ptr<Node> join_type = nullptr;
    std::shared_ptr<Node> _join = nullptr;
    std::shared_ptr<Node> table_list_item = nullptr;
    std::shared_ptr<Node> _on = nullptr;
    std::shared_ptr<Node> expr = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeOrderList: public Node {
    public:
    NodeOrderList(Position position);
    std::vector<std::shared_ptr<Node>> order_list_items;

    void accept(Visitor& visitor) override;
};

class NodeOrderListItem: public Node {
    public:
    NodeOrderListItem(Position position);
    std::shared_ptr<Node> expr = nullptr;
    std::shared_ptr<Node> _asc = nullptr;
    std::shared_ptr<Node> _desc = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeWhereClause: public Node {
    public:
    NodeWhereClause(Position position);
    std::shared_ptr<Node> _where = nullptr;
    std::shared_ptr<Node> expr = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeOrderClause: public Node {
    public:
    NodeOrderClause(Position position);
    std::shared_ptr<Node> _order = nullptr;
    std::shared_ptr<Node> _by = nullptr;
    std::shared_ptr<Node> order_list = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeLimitClause: public Node {
    public:
    NodeLimitClause(Position position);
    std::shared_ptr<Node> _limit = nullptr;
    std::shared_ptr<Node> expr = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeExpression: public Node {
    public:
    NodeExpression(Position position);
    std::shared_ptr<Node> left = nullptr;
    std::shared_ptr<Node> _and = nullptr;
    std::shared_ptr<Node> _or = nullptr;
    std::shared_ptr<Node> _lt = nullptr;
    std::shared_ptr<Node> _gt = nullptr;
    std::shared_ptr<Node> _eq = nullptr;
    std::shared_ptr<Node> _neq = nullptr;
    std::shared_ptr<Node> _lte = nullptr;
    std::shared_ptr<Node> _gte = nullptr;
    std::shared_ptr<Node> right = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeUnaryExpression: public Node {
    public:
    NodeUnaryExpression(Position position);
    std::shared_ptr<Node> _plus = nullptr;
    std::shared_ptr<Node> _minus = nullptr;
    std::shared_ptr<Node> _not = nullptr;
    std::shared_ptr<Node> primary_expr = nullptr;

    void accept(Visitor& visitor) override;
};

class NodePrimaryExpression: public Node {
    public:
    NodePrimaryExpression(Position position);
    std::shared_ptr<Node> _lparen = nullptr;
    std::shared_ptr<Node> expr = nullptr;
    std::shared_ptr<Node> _rparen = nullptr;

    std::shared_ptr<Node> ident1 = nullptr;
    std::shared_ptr<Node> _dot = nullptr;
    std::shared_ptr<Node> ident2 = nullptr;

    std::shared_ptr<Node> literal_value = nullptr;
    std::shared_ptr<Node> identifier = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeUpdateList: public Node {
    public:
    NodeUpdateList(Position position);
    std::shared_ptr<Node> update_item = nullptr;
    std::vector<std::shared_ptr<Node>> update_list;

    void accept(Visitor& visitor) override;
};

class NodeUpdateListItem: public Node {
    public:
    NodeUpdateListItem(Position position);
    std::shared_ptr<Node> identifier = nullptr;
    std::shared_ptr<Node> _eq = nullptr;
    std::shared_ptr<Node> expr = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeColumnDef: public Node {
    public:
    NodeColumnDef(Position position);
    std::shared_ptr<Node> identifier = nullptr;
    std::shared_ptr<Node> data_type = nullptr;

    // Optional
    std::shared_ptr<Node> _not = nullptr;
    std::shared_ptr<Node> _null = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeValueList: public Node {
    public:
    NodeValueList(Position position);
    std::shared_ptr<Node> _brace_open = nullptr;
    std::vector<std::shared_ptr<Node>> value_list;
    std::shared_ptr<Node> _brace_close = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeColumnList: public Node {
    public:
    NodeColumnList(Position position);
    std::shared_ptr<Node> _brace_open = nullptr;
    std::vector<std::shared_ptr<Node>> column_list;
    std::shared_ptr<Node> _brace_close = nullptr;

    void accept(Visitor& visitor) override;
};

class NodeAssignmentList: public Node {
    public:
    NodeAssignmentList(Position position);
    std::vector<std::shared_ptr<Node>> assignment_list;

    void accept(Visitor& visitor) override;
};

class NodeAssignmentListItem: public Node {
    public:
    NodeAssignmentListItem(Position position);
    std::shared_ptr<Node> identifier = nullptr;
    std::shared_ptr<Node> _eq = nullptr;
    std::shared_ptr<Node> expr = nullptr;

    void accept(Visitor& visitor) override;
};

// Terminals
class NodeSelect: public Node {
    public:
    NodeSelect(Position position);

    void accept(Visitor& visitor) override;
};

class NodeFrom: public Node {
    public:
    NodeFrom(Position position);

    void accept(Visitor& visitor) override;
};

class NodeAsteriks: public Node {
    public:
    NodeAsteriks(Position position);

    void accept(Visitor& visitor) override;
};

class NodeComma: public Node {
    public:
    NodeComma(Position position);

    void accept(Visitor& visitor) override;
};

class NodeAs: public Node {
    public:
    NodeAs(Position position);

    void accept(Visitor& visitor) override;
};

class NodeInsert: public Node {
    public:
    NodeInsert(Position position);

    void accept(Visitor& visitor) override;
};

class NodeInto: public Node {
    public:
    NodeInto(Position position);

    void accept(Visitor& visitor) override;
};

class NodeBraceOpen: public Node {
    public:
    NodeBraceOpen(Position position);

    void accept(Visitor& visitor) override;
};

class NodeBraceClose: public Node {
    public:
    NodeBraceClose(Position position);

    void accept(Visitor& visitor) override;
};

class NodeValues: public Node {
    public:
    NodeValues(Position position);

    void accept(Visitor& visitor) override;
};

class NodeDelete: public Node {
    public:
    NodeDelete(Position position);

    void accept(Visitor& visitor) override;
};

class NodeUpdate: public Node {
    public:
    NodeUpdate(Position position);

    void accept(Visitor& visitor) override;
};

class NodeSet: public Node {
    public:
    NodeSet(Position position);

    void accept(Visitor& visitor) override;
};

class NodeCreate: public Node {
    public:
    NodeCreate(Position position);

    void accept(Visitor& visitor) override;
};

class NodePrimary: public Node {
    public:
    NodePrimary(Position position);

    void accept(Visitor& visitor) override;
};

class NodeKey: public Node {
    public:
    NodeKey(Position position);

    void accept(Visitor& visitor) override;
};

class NodeNot: public Node {
    public:
    NodeNot(Position position);

    void accept(Visitor& visitor) override;
};

class NodeNull: public Node {
    public:
    NodeNull(Position position);

    void accept(Visitor& visitor) override;
};

class NodeDrop: public Node {
    public:
    NodeDrop(Position position);

    void accept(Visitor& visitor) override;
};

class NodeTable: public Node {
    public:
    NodeTable(Position position);

    void accept(Visitor& visitor) override;
};

class NodeAdd: public Node {
    public:
    NodeAdd(Position position);

    void accept(Visitor& visitor) override;
};

class NodeWhere: public Node {
    public:
    NodeWhere(Position position);

    void accept(Visitor& visitor) override;
};

class NodeOrder: public Node {
    public:
    NodeOrder(Position position);

    void accept(Visitor& visitor) override;
};

class NodeBy: public Node {
    public:
    NodeBy(Position position);

    void accept(Visitor& visitor) override;
};

class NodeAsc: public Node {
    public:
    NodeAsc(Position position);

    void accept(Visitor& visitor) override;
};

class NodeDesc: public Node {
    public:
    NodeDesc(Position position);

    void accept(Visitor& visitor) override;
};

class NodeLimit: public Node {
    public:
    NodeLimit(Position position);

    void accept(Visitor& visitor) override;
};

class NodeAnd: public Node {
    public:
    NodeAnd(Position position);

    void accept(Visitor& visitor) override;
};

class NodeOr: public Node {
    public:
    NodeOr(Position position);

    void accept(Visitor& visitor) override;
};

class NodeEqual: public Node {
    public:
    NodeEqual(Position position);

    void accept(Visitor& visitor) override;
};

class NodeNotEqual: public Node {
    public:
    NodeNotEqual(Position position);

    void accept(Visitor& visitor) override;
};

class NodeLess: public Node {
    public:
    NodeLess(Position position);

    void accept(Visitor& visitor) override;
};

class NodeLessEqual: public Node {
    public:
    NodeLessEqual(Position position);

    void accept(Visitor& visitor) override;
};

class NodeGreater: public Node {
    public:
    NodeGreater(Position position);

    void accept(Visitor& visitor) override;
};

class NodeGreaterEqual: public Node {
    public:
    NodeGreaterEqual(Position position);

    void accept(Visitor& visitor) override;
};

class NodePlus: public Node {
    public:
    NodePlus(Position position);

    void accept(Visitor& visitor) override;
};

class NodeMinus: public Node {
    public:
    NodeMinus(Position position);

    void accept(Visitor& visitor) override;
};

class NodeDot: public Node {
    public:
    NodeDot(Position position);

    void accept(Visitor& visitor) override;
};

class NodeSlash: public Node {
    public:
    NodeSlash(Position position);

    void accept(Visitor& visitor) override;
};

class NodePercent: public Node {
    public:
    NodePercent(Position position);

    void accept(Visitor& visitor) override;
};

class NodeInt: public Node {
    public:
    NodeInt(Position position);

    void accept(Visitor& visitor) override;
};

class NodeVarchar: public Node {
    public:
    NodeVarchar(Position position);

    void accept(Visitor& visitor) override;
};

class NodeJoin: public Node {
    public:
    NodeJoin(Position position);

    void accept(Visitor& visitor) override;
};

class NodeOn: public Node {
    public:
    NodeOn(Position position);

    void accept(Visitor& visitor) override;
};

class NodeAlter: public Node {
    public:
    NodeAlter(Position position);

    void accept(Visitor& visitor) override;
};

class NodeLeft: public Node {
    public:
    NodeLeft(Position position);

    void accept(Visitor& visitor) override;
};

class NodeRight: public Node {
    public:
    NodeRight(Position position);

    void accept(Visitor& visitor) override;
};

class NodeInner: public Node {
    public:
    NodeInner(Position position);

    void accept(Visitor& visitor) override;
};

class NodeFull: public Node {
    public:
    NodeFull(Position position);

    void accept(Visitor& visitor) override;
};

class NodeIntegerLiteral: public Node {
    public:
    NodeIntegerLiteral(Position position, std::string_view value);
    std::string_view value;

    void accept(Visitor& visitor) override;
};

class NodeFloatLiteral: public Node {
    public:
    NodeFloatLiteral(Position position, std::string_view value);
    std::string_view value;

    void accept(Visitor& visitor) override;
};

class NodeStringLiteral: public Node {
    public:
    NodeStringLiteral(Position position, std::string_view value);
    std::string_view value;

    void accept(Visitor& visitor) override;
};

class NodeIdentifier: public Node {
    public:
    NodeIdentifier(Position position, std::string_view value);
    std::string_view value;

    void accept(Visitor& visitor) override;
};
} // namespace parser

#endif //LAMBDADB_PARSER_NODE_H