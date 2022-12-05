//
// Created by Maximilian Mittelhammer on 05.12.22.
//

#ifndef LAMBDADB_PARSER_NODE_H
#define LAMBDADB_PARSER_NODE_H

#include "../code/Repository.h"

class Visitor;

enum NodeType {
    // Non-terminals
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
    ORDER_BY_CLAUSE,
    LIMIT_CLAUSE,
    SELECT_LIST_ITEM,
    EXPR,
    IDENT,
    COLUMN_LIST,
    EXPR_LIST,
    UPDATE_LIST,
    UPDATE_LIST_ITEM,
    COLUMN_NAME,
    COLUMN_DEF,
    COLUMN_TYPE,
    COLUMN_CONSTRAINT,
    ORDER_BY_LIST,
    ORDER_BY_ITEM,

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
    TABLE,
    ALTER,
    TABLE,
    ADD,
    WHERE,
    ORDER,
    BY,
    ASC,
    DESC,
    LIMIT,
    OR,
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
    LITERAL,

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

#endif //LAMBDADB_PARSER_NODE_H