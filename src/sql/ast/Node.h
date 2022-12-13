//
// Created by Maximilian Mittelhammer on 10.12.22.
//

#ifndef LAMBDADB_AST_NODE_H
#define LAMBDADB_AST_NODE_H

#include <vector>
#include "Evaluator.h"
#include "../code/Repository.h"
#include "../../db/EvaluationTable.h"
// #include "../../db/DataType.h"

namespace sql::ast {

class Visitor;

struct ColumnRef {
    bool is_star = false;
    std::string table;
    std::string name;
    std::string alias;
    // TODO: DataType const_value;
};

struct TableRef {
    std::string name;
    std::string alias;
};

enum ASTNodeType {
    STATEMENT,

    // Data Manipulation Statements
    SELECT_STATEMENT,
    INSERT_STATEMENT,
    UPDATE_STATEMENT,
    DELETE_STATEMENT,

    // Data Definition Statements
    CREATE_TABLE_STATEMENT,
    DROP_TABLE_STATEMENT,
    CREATE_INDEX_STATEMENT,
    DROP_INDEX_STATEMENT,

    LITERAL,
    IDENTIFIER
};

class Node {
private:
    ASTNodeType type;
    Position position;

protected:
    Repository &repository;

public:
    Node(ASTNodeType type, Position position, Repository &repository);
    virtual ~Node() = default;

    /**
     * Returns the type of the node
     * @return Type of the node
    */
    ASTNodeType getType() const;

    /**
     * Returns the position of the node
     * @return Position of the node
    */
    Position getPosition() const;

    /**
     * Implements the visitor pattern
     * @param visitor Visitor
    */
    virtual void accept(Visitor &visitor) = 0;

    /**
     * Evaluates the node with the given context and returns the evaluation table
     * @param context Evaluation context
     * @return Evaluation table
    */
    virtual EvaluationTable evaluate(Evaluator& context) = 0;
};

class Statement : public Node {
private:
    std::vector<Node> statements;

public:
    Statement(Position position, Repository &repository);

    void setStatements(std::vector<Node> statements);

    std::vector<Node> getStatements() const;

    void accept(Visitor &visitor) override;

    EvaluationTable evaluate(Evaluator& context) override;
};

class SelectStatment : public Node {
private:
    std::vector<ColumnRef> columns;
    std::vector<TableRef> tables;

public:
    SelectStatment(Position position, Repository &repository);

    void setColumns(std::vector<ColumnRef> columns);

    void setTables(std::vector<TableRef> tables);

    std::vector<ColumnRef> getColumns() const;

    std::vector<TableRef> getTables() const;

    void accept(Visitor &visitor) override;

    EvaluationTable evaluate(Evaluator& context) override;
};

} // namespace sql::ast

#endif // LAMBDADB_AST_NODE_H