//
// Created by Maximilian Mittelhammer on 11.12.22.
//

#ifndef LAMBDADB_EVALUATION_TABLE_H
#define LAMBDADB_EVALUATION_TABLE_H

#include "Table.h"

class EvaluationTable {
    private:
    std::vector<Column> columns;
    std::vector<std::vector<std::shared_ptr<DataEntryBase>>> items;

    public:

    EvaluationTable(std::vector<Column> columns, std::vector<std::vector<std::shared_ptr<DataEntryBase>>> items);

    /**
     * Returns the columns of the table
     * @return Columns of the table
     */
    [[nodiscard]] std::vector<Column> getColumns() const;

    /**
     * Returns the items of the table
     * @return Items of the table
     */
    [[nodiscard]] std::vector<std::vector<std::shared_ptr<DataEntryBase>>> getItems() const;

    /**
     * Sets the items of the table
     * @param items Items of the table
     */
    void setItems(const std::vector<std::vector<std::shared_ptr<DataEntryBase>>> &items);
};

#endif //LAMBDADB_EVALUATION_TABLE_H