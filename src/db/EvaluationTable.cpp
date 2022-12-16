//
// Created by Maximilian Mittelhammer on 11.12.22.
//

#include "EvaluationTable.h"

EvaluationTable::EvaluationTable(std::vector<Column> columns, std::vector<std::vector<std::shared_ptr<DataEntryBase>>> items) : columns(std::move(columns)), items(std::move(items)) {}

std::vector<Column> EvaluationTable::getColumns() const {
    return columns;
}

std::vector<std::vector<std::shared_ptr<DataEntryBase>>> EvaluationTable::getItems() const {
    return items;
}

void EvaluationTable::setItems(const std::vector<std::vector<std::shared_ptr<DataEntryBase>>> &items) {
    this->items = items;
}