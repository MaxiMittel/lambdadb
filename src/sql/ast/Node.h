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
    std::string_view name;
    std::string_view alias;
};

enum ASTNodeType {
    STATEMENT,

    // Data Manipulation Statements
    SELECT_STATEMENT,
    INSERT_STATEMENT,
    UPDATE_STATEMENT,
    DELETE_STATEMENT,

    // Clauses
    JOIN_CLAUSE,
    WHERE_CLAUSE,

    // Expressions
    EXPRESSION,
    AND_EXPRESSION,
    OR_EXPRESSION,
    BOOL_EXPRESSION,
    UNARY_EXPRESSION,
    PRIMARY_EXPRESSION,

    // Data Definition Statements
    CREATE_TABLE_STATEMENT,
    DROP_TABLE_STATEMENT,
    CREATE_INDEX_STATEMENT,
    DROP_INDEX_STATEMENT,

    LITERAL,
    IDENTIFIER
};

enum JoinType {
    INNER,
    LEFT,
    RIGHT,
    FULL
};

enum BoolOperator {
    EQUAL,
    NOT_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL
};

enum UnaryOperator {
    NOT,
    MINUS
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
    */
    virtual void evaluate(Evaluator& context) = 0;
};

class Statement : public Node {
private:
    std::vector<std::shared_ptr<Node>> statements;

public:
    Statement(Position position, Repository &repository);

    void setStatements(std::vector<std::shared_ptr<Node>> statements);

    std::vector<std::shared_ptr<Node>> getStatements() const;

    void accept(Visitor &visitor) override;

    void evaluate(Evaluator& context) override;
};

class SelectStatment : public Node {
private:
    std::vector<ColumnRef> columns;
    std::vector<TableRef> tables;
    std::shared_ptr<Node> where;
    std::shared_ptr<Node> join;

public:
    SelectStatment(Position position, Repository &repository);

    void setColumns(std::vector<ColumnRef> columns);

    void setTables(std::vector<TableRef> tables);

    void setWhere(std::shared_ptr<Node> where);

    void setJoin(std::shared_ptr<Node> join);

    std::vector<ColumnRef> getColumns() const;

    std::vector<TableRef> getTables() const;

    std::shared_ptr<Node> getWhere() const;

    std::shared_ptr<Node> getJoin() const;

    void accept(Visitor &visitor) override;

    void evaluate(Evaluator& context) override;
};

class JoinStatement : public Node {
private:
    JoinType type;
    TableRef table;
    ColumnRef left;
    ColumnRef right;

public:
    JoinStatement(Position position, Repository &repository);

    void setType(JoinType type);

    JoinType getType() const;

    void setTable(TableRef table);

    TableRef getTable() const;

    void setLeft(ColumnRef left);

    ColumnRef getLeft() const;

    void setRight(ColumnRef right);

    ColumnRef getRight() const;

    void accept(Visitor &visitor) override;

    void evaluate(Evaluator& context) override;
};

class ExpressionBaseNode : public Node {
public:
    ExpressionBaseNode(ASTNodeType type, Position position, Repository &repository);

    virtual bool evaluateExpression(std::vector<std::shared_ptr<db::DataEntryBase>> const& entry) = 0;
};

class ExpressionNode : public ExpressionBaseNode {
    private:
    std::shared_ptr<ExpressionBaseNode> and_expr;

public:
    ExpressionNode(Position position, Repository &repository);

    void setAndExpr(std::shared_ptr<ExpressionBaseNode> and_expr);

    std::shared_ptr<ExpressionBaseNode> getAndExpr() const;

    void accept(Visitor &visitor) override;

    void evaluate(Evaluator& context) override;

    bool evaluateExpression(std::vector<std::shared_ptr<db::DataEntryBase>> const& entry) override;
};

class AndExpressionNode : public ExpressionBaseNode {
private:
    std::shared_ptr<ExpressionBaseNode> or_expr;
    std::shared_ptr<ExpressionBaseNode> and_expr;

public:
    AndExpressionNode(Position position, Repository &repository);

    void setAndExpr(std::shared_ptr<ExpressionBaseNode> and_expr);

    void setOrExpr(std::shared_ptr<ExpressionBaseNode> or_expr);

    std::shared_ptr<ExpressionBaseNode> getAndExpr() const;

    std::shared_ptr<ExpressionBaseNode> getOrExpr() const;

    void accept(Visitor &visitor) override;

    void evaluate(Evaluator& context) override;

    bool evaluateExpression(std::vector<std::shared_ptr<db::DataEntryBase>> const& entry) override;
};

class OrExpressionNode : public ExpressionBaseNode {
private:
    std::shared_ptr<ExpressionBaseNode> bool_expr;
    std::shared_ptr<ExpressionBaseNode> or_expr;

public:
    OrExpressionNode(Position position, Repository &repository);

    void setOrExpr(std::shared_ptr<ExpressionBaseNode> or_expr);

    void setBoolExpr(std::shared_ptr<ExpressionBaseNode> bool_expr);

    std::shared_ptr<ExpressionBaseNode> getOrExpr() const;

    std::shared_ptr<ExpressionBaseNode> getBoolExpr() const;

    void accept(Visitor &visitor) override;

    void evaluate(Evaluator& context) override;

    bool evaluateExpression(std::vector<std::shared_ptr<db::DataEntryBase>> const& entry) override;
};

class BoolExpressionNode : public ExpressionBaseNode {
private:
    std::shared_ptr<ExpressionBaseNode> left;
    std::shared_ptr<ExpressionBaseNode> right;
    BoolOperator op; // TODO: Split into seperate nodes?

public:
    BoolExpressionNode(Position position, Repository &repository);

    void setLeft(std::shared_ptr<ExpressionBaseNode> left);

    void setRight(std::shared_ptr<ExpressionBaseNode> right);

    void setOperator(BoolOperator op);

    std::shared_ptr<ExpressionBaseNode> getLeft() const;

    std::shared_ptr<ExpressionBaseNode> getRight() const;

    BoolOperator getOperator() const;

    void accept(Visitor &visitor) override;

    void evaluate(Evaluator& context) override;

    bool evaluateExpression(std::vector<std::shared_ptr<db::DataEntryBase>> const& entry) override;
};

class UnaryExpressionNode : public ExpressionBaseNode {
private:
    std::shared_ptr<ExpressionBaseNode> primary_expr;
    UnaryOperator op;

public:
    UnaryExpressionNode(Position position, Repository &repository);

    void setPrimaryExpr(std::shared_ptr<ExpressionBaseNode> primary_expr);

    void setOperator(UnaryOperator op);

    std::shared_ptr<ExpressionBaseNode> getPrimaryExpr() const;

    UnaryOperator getOperator() const;

    void accept(Visitor &visitor) override;

    void evaluate(Evaluator& context) override;

    bool evaluateExpression(std::vector<std::shared_ptr<db::DataEntryBase>> const& entry) override;
};

class PrimaryExpressionNode : public ExpressionBaseNode {
public:
    PrimaryExpressionNode(Position position, Repository &repository);

    void accept(Visitor &visitor) override;

    void evaluate(Evaluator& context) override;

    bool evaluateExpression(std::vector<std::shared_ptr<db::DataEntryBase>> const& entry) override;
};


} // namespace sql::ast

#endif // LAMBDADB_AST_NODE_H