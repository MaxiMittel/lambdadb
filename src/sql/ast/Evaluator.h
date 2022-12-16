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
    std::shared_ptr<EvaluationTable> evaluationTable;

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

    /**
     * Get the evaluation table
     * @return Evaluation table
     */
    std::shared_ptr<EvaluationTable> getEvaluationTable() const;

    /**
     * Set the evaluation table
     * @param evaluationTable Evaluation table
     */
    void setEvaluationTable(std::shared_ptr<EvaluationTable> evaluationTable);
};

} // namespace sql::ast

#endif // LAMBDADB_AST_EVALUATOR_H
