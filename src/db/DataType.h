//
// Created by Maximilian Mittelhammer on 30.11.22.
//

#ifndef LAMBDADB_DATATYPE_H
#define LAMBDADB_DATATYPE_H

#include <cstddef>
#include <string>
#include <iostream>

namespace db {

enum class DataType {
    INTEGER,
    VARCHAR,
    SQL_NULL
};

class DataEntry {
    protected:
        DataType type;
public:
    /**
     * Returns the data type
     * @return Data type
     */
    [[nodiscard]] virtual DataType getType() const = 0;

    /**
     * Overloads the << operator to print the value of the data type
     * @tparam T The type of the value
     */
    friend std::ostream& operator<<(std::ostream& os, const DataEntryBase& entry) {
        os << entry.toString();
        return os;
    }

    /**
     * Overloads the == operator to compare the value of the data type
     * @tparam T The type of the value
     */
    friend bool operator==(const DataEntryBase& lhs, const DataEntryBase& rhs) {
        if (lhs.getType() != rhs.getType()) {
            return false;
        }

        // TODO
        /*switch (lhs.getType()) {
            case DataType::INTEGER: {
                int32_t left = static_cast<DataEntry<int32_t> const&>(lhs).getValue();
                int32_t right = static_cast<DataEntry<int32_t> const&>(rhs).getValue();
                return left == right;
            }
            case DataType::VARCHAR: {
                std::string left = static_cast<DataEntry<std::string> const&>(lhs).getValue();
                std::string right = static_cast<DataEntry<std::string> const&>(rhs).getValue();
                return left == right;
            }
            default:
                return false;
        }*/

        return false;
    }

    /**
     * Overloads the != operator to compare the value of the data type
     * @tparam T The type of the value
     */
    friend bool operator!=(const DataEntryBase& lhs, const DataEntryBase& rhs) {
        return !(lhs == rhs);
    }

    /**
     * Overloads the < operator to compare the value of the data type
     * @tparam T The type of the value
     */
    friend bool operator<(const DataEntryBase& lhs, const DataEntryBase& rhs) {
        if (lhs.getType() != rhs.getType()) {
            return false;
        }

        // TODO
        /*switch (lhs.getType()) {
            case DataType::INTEGER:
                return dynamic_cast<const DataEntry<int32_t>&>(lhs).getValue() < dynamic_cast<const DataEntry<int32_t>&>(rhs).getValue();
            case DataType::VARCHAR:
                return dynamic_cast<const DataEntry<std::string>&>(lhs).getValue() < dynamic_cast<const DataEntry<std::string>&>(rhs).getValue();
        }*/
        return false;
    }

    /**
     * Overloads the > operator to compare the value of the data type
     * @tparam T The type of the value
     */
    friend bool operator>(const DataEntryBase& lhs, const DataEntryBase& rhs) {
        return rhs < lhs;
    }

    /**
     * Overloads the <= operator to compare the value of the data type
     * @tparam T The type of the value
     */
    friend bool operator<=(const DataEntryBase& lhs, const DataEntryBase& rhs) {
        return !(lhs > rhs);
    }

    /**
     * Overloads the >= operator to compare the value of the data type
     * @tparam T The type of the value
     */
    friend bool operator>=(const DataEntryBase& lhs, const DataEntryBase& rhs) {
        return !(lhs < rhs);
    }

    /**
     * Get the string representation of the data type
     * @return String representation of the data type
     */
    virtual std::string toString() const = 0;
};

class DataEntryNull: public DataEntryBase {
public:
    DataEntryNull() {}
    virtual ~DataEntryNull() = default;

    /**
     * Returns the data type
     * @return Data type
     */
    [[nodiscard]] DataType getType() const override;

    /**
     * Get the string representation of the data type
     * @return String representation of the data type
     */
    std::string toString() const override {
        return "NULL";
    }
};


class DataEntryInteger: public DataEntryBase {
private:
    int32_t data;
public:
    DataEntryInteger(int32_t data, DataType type);

    /**
     * Returns the stored value
     * @return Stored value
     */
    [[nodiscard]] int32_t getValue() const;

    /**
     * Sets the value of the data type
     * @tparam T The type of the value
     * @param value The value to set
     */
    void setValue(int32_t value);

    /**
     * Returns the data type
     * @return Data type
     */
    [[nodiscard]] DataType getType() const override;

    /**
     * Get the string representation of the data type
     * @return String representation of the data type
     */
    std::string toString() const override;

    /**
     * Overloads the << operator to print the value of the data type
     * @tparam T The type of the value
    */
    friend std::ostream& operator<<(std::ostream& os, const DataEntryInteger& entry) {
        os << entry.getValue();
        return os;
    }
};

class DataEntryVarchar: public DataEntryBase {
private:
    std::string data;
public:
    DataEntryVarchar(std::string data);

    /**
     * Returns the stored value
     * @return Stored value
     */
    [[nodiscard]] std::string getValue() const;

    /**
     * Sets the value of the data type
     * @tparam T The type of the value
     * @param value The value to set
     */
    void setValue(std::string value);

    /**
     * Returns the data type
     * @return Data type
     */
    [[nodiscard]] DataType getType() const override;

    /**
     * Get the string representation of the data type
     * @return String representation of the data type
     */
    std::string toString() const override;

    /**
     * Overloads the << operator to print the value of the data type
     * @tparam T The type of the value
    */
    friend std::ostream& operator<<(std::ostream& os, const DataEntryInteger& entry) {
        os << entry.getValue();
        return os;
    }
};

} // namespace db

#endif //LAMBDADB_DATATYPE_H
