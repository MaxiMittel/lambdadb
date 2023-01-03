//
// Created by Maximilian Mittelhammer on 11.12.22.
//

#include "EvaluationTable.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
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
            out << item[i]->toString() << " ";
        }
        out << std::endl;
    }
}

std::string EvaluationTable::toJSON() const {
    json j;

    for (const auto &item : items) {
        json itemJson;
        for (size_t i = 0; i < columns.size(); i++) {
            itemJson[columns[i].name] = item[i]->toString();
        }
        j.push_back(itemJson);
    }

    return j.dump();
}