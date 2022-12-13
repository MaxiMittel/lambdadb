//
// Created by Maximilian Mittelhammer on 10.12.22.
//

#ifndef LAMBDADB_AST_EVALUATOR_H
#define LAMBDADB_AST_EVALUATOR_H

#include <vector>
#include "../../db/Database.h"
#include "../../db/EvaluationTable.h"
#include "AST.h"

namespace sql::ast {

class Evaluator {
private:
    AST &ast;
    Database &database;

public:
    Evaluator(AST &ast, Database &database);

    /**
     * Returns the database
     * @return Database
     */
    Database& getDatabase() const;

    /**
     * Evaluates the AST
     * @return Evaluation table
     */
    EvaluationTable evaluate();
};

} // namespace sql::ast

#endif // LAMBDADB_AST_EVALUATOR_H
