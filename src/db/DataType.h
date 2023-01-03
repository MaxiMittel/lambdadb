//
// Created by Maximilian Mittelhammer on 30.11.22.
//

#ifndef LAMBDADB_DATATYPE_H
#define LAMBDADB_DATATYPE_H

#include <cstddef>
#include <string>
#include <iostream>
#include <stdexcept>

namespace db {

enum class DataType {
    INTEGER,
    VARCHAR,
    SQL_NULL
};

class DataEntry {
public:
    virtual ~DataEntry() = default;

    /**
     * Returns the data type
     * @return Data type
     */
    [[nodiscard]] virtual DataType getType() const = 0;


    /**
     * Get the string representation of the data type
     * @return String representation of the data type
     */
    virtual std::string toString() const = 0;

    /**
     * Compares the value of the data type
     * @param other The data other
    */
    virtual int compare(const DataEntry &other) const = 0;
};

class DataEntryNull: public DataEntry {
public:
    DataEntryNull() = default;
    ~DataEntryNull() override = default;

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
     * Overloads the <=> operator to compare the value of the data type
     * @param entry The data entry
     * @return The comparison result
    */
    // TODO: Does not work :(
    //auto operator<=>(const DataEntry& entry) const;

    /**
     * Compares the value of the data type
     * @param other The data other
    */
    int compare(const DataEntry &other) const override;
};


class DataEntryInteger: public DataEntry {
private:
    int32_t data;
public:
    DataEntryInteger(int32_t data);
    ~DataEntryInteger() override = default;

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
     * @param os The output stream
     * @param entry The data entry
    */
    friend std::ostream& operator<<(std::ostream& os, const DataEntryInteger& entry) {
        os << entry.getValue();
        return os;
    }

    /**
     * Overloads the <=> operator to compare the value of the data type
     * @param entry The data entry
     * @return The comparison result
    */
    // TODO: Does not work :(
    //auto operator<=>(const DataEntry& entry) const;

    /**
     * Compares the value of the data type
     * @param other The data other
    */
    int compare(const DataEntry &other) const override;
};

class DataEntryVarchar: public DataEntry {
private:
    std::string data;
public:
    DataEntryVarchar(std::string data);
    ~DataEntryVarchar() = default;

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
    void setValue(const std::string &value);

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
    friend std::ostream& operator<<(std::ostream& os, const DataEntryVarchar& entry) {
        os << entry.getValue();
        return os;
    }

    /**
     * Overloads the <=> operator to compare the value of the data type
     * @param entry The data entry
     * @return The comparison result
    */
    // TODO: Does not work :(
    //auto operator<=>(const DataEntry& entry) const;

    /**
     * Compares the value of the data type
     * @param other The data other
    */
    int compare(const DataEntry &other) const override;
};

} // namespace db

#endif //LAMBDADB_DATATYPE_H
