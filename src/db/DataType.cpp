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

/*auto DataEntryNull::operator<=>(const DataEntry &other) const {
    std::cout << "Comparing NULL and " << other.toString() << std::endl;
    if (other.getType() == DataType::SQL_NULL) {
        return 0 <=> 0;
    } else {
        return 1 <=> 0;
    }
}*/

int DataEntryNull::compare(const DataEntry &other) const {
    if (other.getType() == DataType::SQL_NULL) {
        return 0;
    } else {
        return 1;
    }
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

/*auto DataEntryInteger::operator<=>(const DataEntry &other) const {
    std::cout << "Comparing " << data << " and " << other.toString() << std::endl;
    if (other.getType() == DataType::INTEGER) {
        std::cout << "Comparing " << data << " and " << static_cast<const DataEntryInteger &>(other).data << std::endl;
        return data <=> static_cast<const DataEntryInteger &>(other).data;
    } else {
        return 1 <=> 0;
    }
}*/

int DataEntryInteger::compare(const DataEntry &other) const {
    if (other.getType() == DataType::INTEGER) {
        return data - static_cast<const DataEntryInteger &>(other).data;
    } else {
        return 1;
    }
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

/*auto DataEntryVarchar::operator<=>(const DataEntry &other) const {
    std::cout << "Comparing " << data << " and " << other.toString() << std::endl;
    if (other.getType() == DataType::VARCHAR) {
        return data <=> static_cast<const DataEntryVarchar &>(other).data;
    } else {
        return 1 <=> 0;
    }
}*/

int DataEntryVarchar::compare(const DataEntry &other) const {
    if (other.getType() == DataType::VARCHAR) {
        return data.compare(other.toString()) > 0; // TODO: Check
    } else {
        return 1;
    }
}