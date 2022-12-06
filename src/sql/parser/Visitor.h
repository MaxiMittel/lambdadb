//
// Created by Maximilian Mittelhammer on 06.12.22.
//

#ifndef LAMBDADB_PARSER_VISITOR_H
#define LAMBDADB_PARSER_VISITOR_H

#include "Node.h"

class Visitor {
    public:
        virtual void visit(const NodeStmt& node) = 0;
        virtual void visit(const NodeSelectStmt& node) = 0;
        virtual void visit(const NodeInsertStmt& node) = 0;
        virtual void visit(const NodeDeleteStmt& node) = 0;
        virtual void visit(const NodeUpdateStmt& node) = 0;
        virtual void visit(const NodeCreateStmt& node) = 0;
        virtual void visit(const NodeDropStmt& node) = 0;
        virtual void visit(const NodeAlterStmt& node) = 0;
        virtual void visit(const NodeSelectList& node) = 0;
        virtual void visit(const NodeSelectListItem& node) = 0;
        virtual void visit(const NodeWhereClause& node) = 0;
        virtual void visit(const NodeOrderByClause& node) = 0;
        virtual void visit(const NodeOrderByList& node) = 0;
        virtual void visit(const NodeOrderByItem& node) = 0;
        virtual void visit(const NodeLimitClause& node) = 0;
        virtual void visit(const NodeUpdateList& node) = 0;
        virtual void visit(const NodeUpdateListItem& node) = 0;
        virtual void visit(const NodeColumnDef& node) = 0;
        virtual void visit(const NodeExprList& node) = 0;
        virtual void visit(const NodeSelect& node) = 0;
        virtual void visit(const NodeFrom& node) = 0;
        virtual void visit(const NodeAsteriks& node) = 0;
        virtual void visit(const NodeComma& node) = 0;
        virtual void visit(const NodeAs& node) = 0;
        virtual void visit(const NodeInsert& node) = 0;
        virtual void visit(const NodeInto& node) = 0;
        virtual void visit(const NodeBraceOpen& node) = 0;
        virtual void visit(const NodeBraceClose& node) = 0;
        virtual void visit(const NodeValues& node) = 0;
        virtual void visit(const NodeDelete& node) = 0;
        virtual void visit(const NodeUpdate& node) = 0;
        virtual void visit(const NodeSet& node) = 0;
        virtual void visit(const NodeCreate& node) = 0;
        virtual void visit(const NodeTable& node) = 0;
        virtual void visit(const NodePrimary& node) = 0;
        virtual void visit(const NodeKey& node) = 0;
        virtual void visit(const NodeNot& node) = 0;
        virtual void visit(const NodeNull& node) = 0;
        virtual void visit(const NodeDrop& node) = 0;
        virtual void visit(const NodeAdd& node) = 0;
        virtual void visit(const NodeWhere& node) = 0;
        virtual void visit(const NodeOrder& node) = 0;
        virtual void visit(const NodeBy& node) = 0;
        virtual void visit(const NodeAsc& node) = 0;
        virtual void visit(const NodeDesc& node) = 0;
        virtual void visit(const NodeLimit& node) = 0;
        virtual void visit(const NodeOr& node) = 0;
        virtual void visit(const NodeEqual& node) = 0;
        virtual void visit(const NodeNotEqual& node) = 0;
        virtual void visit(const NodeLess& node) = 0;
        virtual void visit(const NodeLessEqual& node) = 0;
        virtual void visit(const NodeGreater& node) = 0;
        virtual void visit(const NodeGreaterEqual& node) = 0;
        virtual void visit(const NodePlus& node) = 0;
        virtual void visit(const NodeMinus& node) = 0;
        virtual void visit(const NodeSlash& node) = 0;
        virtual void visit(const NodePercent& node) = 0;
        virtual void visit(const NodeInt& node) = 0;
        virtual void visit(const NodeVarchar& node) = 0;
        virtual void visit(const NodeLiteral& node) = 0;
        virtual void visit(const NodeIdentifier& node) = 0;
};

#endif //LAMBDADB_PARSER_VISITOR_H