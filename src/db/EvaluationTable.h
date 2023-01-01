//
// Created by Maximilian Mittelhammer on 11.12.22.
//

#ifndef LAMBDADB_EVALUATION_TABLE_H
#define LAMBDADB_EVALUATION_TABLE_H

#include "Table.h"

namespace db {

class EvaluationTable {
    private:
    std::vector<Column> columns;
    std::vector<std::vector<std::shared_ptr<DataEntry>>> items;

    public:
    /**
     * Creates a new evaluation table
     * @param columns Columns of the table
     * @param items Items of the table
     */
    EvaluationTable(std::vector<Column> columns, std::vector<std::vector<std::shared_ptr<DataEntry>>> items);

    /**
     * Creates a new evaluation table
     */
    EvaluationTable();

    /**
     * Returns the columns of the table
     * @return Columns of the table
     */
    [[nodiscard]] std::vector<Column> getColumns() const;

    /**
     * Returns the items of the table
     * @return Items of the table
     */
    [[nodiscard]] std::vector<std::vector<std::shared_ptr<DataEntry>>> getItems() const;

    /**
     * Sets the items of the table
     * @param items Items of the table
     */
    void setItems(const std::vector<std::vector<std::shared_ptr<DataEntry>>> &items);

    /**
     * Set the columns of the table
     * @param columns Columns of the table
     */
    void setColumns(const std::vector<Column> &columns);

    /**
     * Returns the number of rows
     * @return Number of rows
     */
    [[nodiscard]] size_t getRowCount() const;

    /**
     * Prints the table
     * @param out Output stream
     */
    void print(std::ostream& out);
};

} // namespace db

#endif //LAMBDADB_EVALUATION_TABLE_H