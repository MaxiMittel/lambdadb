//
// Created by Maximilian Mittelhammer on 01.01.23.
//

#include "DataType.h"
#include <iostream>

using namespace db;

DataType DataEntryNull::getType() const {
    return DataType::SQL_NULL;
}

int32_t DataEntryNull::getValue() const {
    return 0;
}

void DataEntryNull::setValue([[maybe_unused]] int32_t value) {
    // Do nothing
}

std::string DataEntryNull::toString() const {
    return "NULL";
}

DataEntryInteger::DataEntryInteger(int32_t data) : data(data) {}

DataType DataEntryInteger::getType() const {
    return DataType::INTEGER;
}

int32_t DataEntryInteger::getValue() const {
    return data;
}

void DataEntryInteger::setValue(int32_t data) {
    this->data = data;
}

std::string DataEntryInteger::toString() const {
    return std::to_string(data);
}

DataEntryVarchar::DataEntryVarchar(std::string data) : data(std::move(data)) {}

DataType DataEntryVarchar::getType() const {
    return DataType::VARCHAR;
}

std::string DataEntryVarchar::getValue() const {
    return data;
}

void DataEntryVarchar::setValue(const std::string &data) {
    this->data = data;
}

std::string DataEntryVarchar::toString() const {
    return data;
}