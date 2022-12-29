//
// Created by Maximilian Mittelhammer on 10.12.22.
//

#ifndef LAMBDADB_PARSER_PRINTVISITOR_H
#define LAMBDADB_PARSER_PRINTVISITOR_H

#include "Node.h"
#include "Visitor.h"

namespace sql::parser
{

    class PrintVisitor : public Visitor
    {
    private:
        std::ostream &out;
        int node_count = 0;

    public:
        PrintVisitor(std::ostream &out);

        virtual void visit(const NodeStmt &node) override;
        virtual void visit(const NodeSelectStmt &node) override;
        virtual void visit(const NodeInsertStmt &node) override;
        virtual void visit(const NodeDeleteStmt &node) override;
        virtual void visit(const NodeUpdateStmt &node) override;
        virtual void visit(const NodeCreateStmt &node) override;
        virtual void visit(const NodeDropStmt &node) override;
        virtual void visit(const NodeAlterStmt &node) override;
        virtual void visit(const NodeSelectList &node) override;
        virtual void visit(const NodeSelectListItem &node) override;
        virtual void visit(const NodeTableList &node) override;
        virtual void visit(const NodeTableListItem &node) override;
        virtual void visit(const NodeJoinClause &node) override;
        virtual void visit(const NodeWhereClause &node) override;
        virtual void visit(const NodeOrderClause &node) override;
        virtual void visit(const NodeOrderList &node) override;
        virtual void visit(const NodeOrderListItem &node) override;
        virtual void visit(const NodeLimitClause &node) override;
        virtual void visit(const NodeExpression& node) override;
        virtual void visit(const NodeAndExpression& node) override;
        virtual void visit(const NodeOrExpression& node) override;
        virtual void visit(const NodeBoolExpression& node) override;
        virtual void visit(const NodeUnaryExpression &node) override;
        virtual void visit(const NodePrimaryExpression &node) override;
        virtual void visit(const NodeUpdateList &node) override;
        virtual void visit(const NodeUpdateListItem &node) override;
        virtual void visit(const NodeValueList& node) override;
        virtual void visit(const NodeColumnList& node) override;
        virtual void visit(const NodeAssignmentList& node) override;
        virtual void visit(const NodeAssignmentListItem& node) override;
        virtual void visit(const NodeColumnDef &node) override;
        virtual void visit(const NodeSelect &node) override;
        virtual void visit(const NodeFrom &node) override;
        virtual void visit(const NodeAsteriks &node) override;
        virtual void visit(const NodeComma &node) override;
        virtual void visit(const NodeAs &node) override;
        virtual void visit(const NodeInsert &node) override;
        virtual void visit(const NodeInto &node) override;
        virtual void visit(const NodeBraceOpen &node) override;
        virtual void visit(const NodeBraceClose &node) override;
        virtual void visit(const NodeValues &node) override;
        virtual void visit(const NodeDelete &node) override;
        virtual void visit(const NodeUpdate &node) override;
        virtual void visit(const NodeSet &node) override;
        virtual void visit(const NodeCreate &node) override;
        virtual void visit(const NodeTable &node) override;
        virtual void visit(const NodePrimary &node) override;
        virtual void visit(const NodeKey &node) override;
        virtual void visit(const NodeNot &node) override;
        virtual void visit(const NodeNull &node) override;
        virtual void visit(const NodeDrop &node) override;
        virtual void visit(const NodeWhere &node) override;
        virtual void visit(const NodeOrder &node) override;
        virtual void visit(const NodeBy &node) override;
        virtual void visit(const NodeAsc &node) override;
        virtual void visit(const NodeDesc &node) override;
        virtual void visit(const NodeLimit &node) override;
        virtual void visit(const NodeAdd &node) override;
        virtual void visit(const NodeOr &node) override;
        virtual void visit(const NodeAnd &node) override;
        virtual void visit(const NodeEqual &node) override;
        virtual void visit(const NodeNotEqual &node) override;
        virtual void visit(const NodeLess &node) override;
        virtual void visit(const NodeLessEqual &node) override;
        virtual void visit(const NodeGreater &node) override;
        virtual void visit(const NodeGreaterEqual &node) override;
        virtual void visit(const NodePlus &node) override;
        virtual void visit(const NodeMinus &node) override;
        virtual void visit(const NodeDot &node) override;
        virtual void visit(const NodeSlash &node) override;
        virtual void visit(const NodePercent &node) override;
        virtual void visit(const NodeInt &node) override;
        virtual void visit(const NodeVarchar &node) override;
        virtual void visit(const NodeJoin &node) override;
        virtual void visit(const NodeAlter &node) override;
        virtual void visit(const NodeLeft &node) override;
        virtual void visit(const NodeRight &node) override;
        virtual void visit(const NodeInner &node) override;
        virtual void visit(const NodeFull &node) override;
        virtual void visit(const NodeOn& node) override;
        virtual void visit(const NodeIntegerLiteral& node) override;
        virtual void visit(const NodeFloatLiteral& node) override;
        virtual void visit(const NodeStringLiteral& node) override;
        virtual void visit(const NodeIdentifier &node) override;
    };

}

#endif // LAMBDADB_PARSER_PRINTVISITOR_H