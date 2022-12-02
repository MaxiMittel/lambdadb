//
// Created by Maximilian Mittelhammer on 30.11.22.
//

#ifndef LAMBDADB_DATATYPE_H
#define LAMBDADB_DATATYPE_H

#include <cstddef>
#include <string>
#include <iostream>

enum class DataType {
    INTEGER,
    VARCHAR
};

class DataEntryBase {
public:
    /**
     * Returns the data type
     * @return Data type
     */
    [[nodiscard]] virtual DataType getType() const = 0;
};

template<typename T>
class DataEntry: public DataEntryBase {
private:
    T data;
    DataType type;
public:
    DataEntry(T data, DataType type) : data(data), type(type) {}

    /**
     * Returns the stored value
     * @return Stored value
     */
    [[nodiscard]] T getValue() const {
        return data;
    }

    /**
     * Sets the value of the data type
     * @tparam T The type of the value
     * @param value The value to set
     */
    void setValue(T value) {
        data = value;
    }

    /**
     * Returns the data type
     * @return Data type
     */
    [[nodiscard]] DataType getType() const {
        return type;
    }

    /**
     * Overloads the << operator to print the value of the data type
     * @tparam T The type of the value
    */
    friend std::ostream& operator<<(std::ostream& os, const DataEntry<T>& entry) {
        os << entry.getValue();
        return os;
    }
};


#endif //LAMBDADB_DATATYPE_H
