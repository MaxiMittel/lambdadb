//
// Created by Maximilian Mittelhammer on 02.12.22.
//

#ifndef LAMBDADB_DATABASE_H
#define LAMBDADB_DATABASE_H

#include <string>
#include <unordered_map>
#include "Table.h"
#include "../storage/StorageService.h"

class Database {
private:
    std::string name;
    StorageService& storageService;
    std::unordered_map<std::string, std::shared_ptr<Table>> tables;

    DataType getDataType(std::string type);
    std::string getDataTypeString(DataType type);

public:
    Database(std::string name, StorageService& storageService);

    /**
     * Returns the name of the database
     * @return Name of the database
     */
    [[nodiscard]] std::string getName() const;

    /**
     * Returns the tables of the database
     * @return Tables of the database
     */
    [[nodiscard]] std::vector<std::shared_ptr<Table>> getTables() const;

    /**
     * Returns the table with the given name
     * @param name Name of the table
     * @return Table with the given name
     */
    [[nodiscard]] std::shared_ptr<Table> getTable(std::string name) const;

    /**
     * Creates a table with the given name and columns
     * @param name Name of the table
     * @param columns Columns of the table
     */
    void createTable(std::string name, std::vector<Column> columns);

    /**
     * Drops the table with the given name
     * @param name Name of the table
     */
    void dropTable(std::string name);
};


#endif //LAMBDADB_DATABASE_H
