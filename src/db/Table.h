//
// Created by Maximilian Mittelhammer on 30.11.22.
//

#ifndef LAMBDADB_TABLE_H
#define LAMBDADB_TABLE_H

#include <string>
#include <vector>
#include <unordered_map>
#include "DataType.h"
#include "../storage/StorageService.h"

struct Column {
    std::string name;
    DataType type;
    size_t size;
    bool primary_key;
};

enum JoinType {
    INNER,
    LEFT,
    RIGHT,
    FULL,
    CROSS
};

class Table {
private:
    std::string name;
    std::string database;
    std::vector<Column> columns;
    StorageService& storageService;
    size_t item_length = 0;

    std::string OFFSET_MAP_KEY;
    std::string DATA_KEY;

    std::unordered_map<std::string, size_t> offsetMap;
    bool offsetMapInitialized = false;

    /**
     * Get the offset map
     * @return If the offset map is initialized return the offset map, otherwise initialize it and return it
     */
    std::unordered_map<std::string, size_t> getOffsetMap();

public:
    Table(std::string name, std::string database, std::vector<Column> columns, StorageService& storageService);

    /**
     * Returns the name of the table
     * @return Name of the table
     */
    [[nodiscard]] std::string getName() const;

    /**
     * Returns the columns of the table
     * @return Columns of the table
     */
    [[nodiscard]] std::vector<Column> getColumns() const;

    /**
     * Returns the items of the table
     * @return Items of the table
     */
    /*[[nodiscard]]*/ std::vector<std::vector<std::shared_ptr<DataEntryBase>>> getItems() const;

    /**
     * Returns the row count of the table
     * @return Row count of the table
     */
    [[nodiscard]] size_t getRowCount() const;

    /**
     * Inserts a new item into the table
     * @param item The item to insert
     */
    void insertItem(std::vector<std::shared_ptr<DataEntryBase>> item);

    /**
     * Deletes an item from the table
     * @param key The key of the item to delete
     */
    void deleteItem(std::string key);

    /**
     * Updates an item in the table
     * @param key The key of the item to update
     * @param item The new item
     */
    void updateItem(std::string key, std::vector<std::shared_ptr<DataEntryBase>> item);


    /**
     * Joins the table with another table
     * @param other The other table
     * @param type The type of join
     * @param leftColumn The column to join on
     * @param rightColumn The column to join on
     * @return Joined table
     */
    Table join(Table other, JoinType type, std::string leftColumn, std::string rightColumn) const;
};


#endif //LAMBDADB_TABLE_H
