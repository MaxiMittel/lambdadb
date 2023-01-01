//
// Created by Maximilian Mittelhammer on 11.12.22.
//

#include "EvaluationTable.h"

using namespace db;

EvaluationTable::EvaluationTable() = default;

EvaluationTable::EvaluationTable(std::vector<Column> columns, std::vector<std::vector<std::shared_ptr<DataEntry>>> items) : columns(std::move(columns)), items(std::move(items)) {}

std::vector<Column> EvaluationTable::getColumns() const {
    return columns;
}

std::vector<std::vector<std::shared_ptr<DataEntry>>> EvaluationTable::getItems() const {
    return items;
}

void EvaluationTable::setItems(const std::vector<std::vector<std::shared_ptr<DataEntry>>> &items) {
    this->items = items;
}

void EvaluationTable::setColumns(const std::vector<Column> &columns) {
    this->columns = columns;
}

size_t EvaluationTable::getRowCount() const {
    return items.size();
}

void EvaluationTable::print(std::ostream &out) {
    // Print the header
    for (const auto &column : columns) {
        out << column.name << " ";
    }
    out << std::endl;

    // Print the items
    for (const auto &item : items) {
        for (size_t i = 0; i < columns.size(); i++) {
            switch (columns[i].type) {
                case DataType::INTEGER:
                    std::cout << static_cast<DataEntryInteger*>(item[i].get())->getValue() << " ";
                    break;
                case DataType::VARCHAR:
                    std::cout << static_cast<DataEntryVarchar*>(item[i].get())->getValue() << " ";
                    break;
                case DataType::SQL_NULL:
                    std::cout << "NULL ";
                    break;
            }
        }
        out << std::endl;
    }
}