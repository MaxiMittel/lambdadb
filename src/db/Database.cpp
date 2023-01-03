//
// Created by Maximilian Mittelhammer on 02.12.22.
//

#include "Database.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace db;

Database::Database(std::string name, StorageService& storageService): name(std::move(name)), storageService(storageService) {
    if(!storageService.exists(this->name + "/config.json")) {
        std::cout << "Creating database " << this->name << std::endl;
        std::string empty_config = R"({
            "name": ")" + this->name + R"(",
            "tables": []
        })";
        storageService.writeObject(this->name + "/config.json", Buffer{(uint8_t*)empty_config.c_str(), empty_config.length()});
    }

    auto content = storageService.readObject(this->name + "/config.json");

    if(content.size == 0) {
        return;
    }

    auto data = json::parse(content.data);

    for (auto table: data["tables"]) {
        std::vector<Column> columns;
        for (auto column: table["columns"]) {
            columns.emplace_back(Column{
                    table["name"],
                    column["name"],
                    getDataType(column["type"]),
                    column["size"],
                    column["primary_key"]
            });
        }

        tables.emplace(table["name"], std::make_shared<Table>(table["name"], this->name, columns, storageService));
    }
}

DataType Database::getDataType(std::string type) {
    if(type == "INTEGER") {
        return DataType::INTEGER;
    } else if(type == "VARCHAR") {
        return DataType::VARCHAR;
    } else {
        throw std::runtime_error("Unknown data type");
    }
}

std::string Database::getDataTypeString(DataType type) {
    switch (type) {
        case DataType::INTEGER:
            return "INTEGER";
        case DataType::VARCHAR:
            return "VARCHAR";
        default:
            throw std::runtime_error("Unknown data type");
    }
}

std::string Database::getName() const {
    return name;
}

std::vector<std::shared_ptr<Table>> Database::getTables() const {
    std::vector<std::shared_ptr<Table>> tables;
    
    for (auto& table : this->tables) {
        tables.emplace_back(table.second);
    }

    return tables;
}

std::shared_ptr<Table> Database::getTable(std::string name) const {
    return tables.at(name);
}

void Database::createTable(std::string name, std::vector<Column> columns) {
    if(tables.find(name) != tables.end()) {
        throw std::runtime_error("Table already exists");
    }

    tables.emplace(name, std::make_shared<Table>(name, this->name, columns, storageService));

    auto content = storageService.readObject(this->name + "/config.json");

    if(content.size == 0) {
        return;
    }

    auto data = json::parse(content.data);

    json table = {
            {"name", name},
            {"columns", json::array()}
    };

    for (auto column: columns) {
        table["columns"].push_back({
            {"name", column.name},
            {"type", getDataTypeString(column.type)},
            {"size", column.size},
            {"primary_key", column.primary_key}
        });
    }

    data["tables"].push_back(table);

    storageService.writeObject(this->name + "/config.json", Buffer{(uint8_t*)data.dump().c_str(), data.dump().length()});
}

void Database::dropTable(std::string name) {
    if(tables.find(name) == tables.end()) {
        throw std::runtime_error("Table does not exist");
    }

    tables.erase(name);

    auto content = storageService.readObject(this->name + "/config.json");

    if(content.size == 0) {
        return;
    }

    auto data = json::parse(content.data);

    for (auto it = data["tables"].begin(); it != data["tables"].end(); ++it) {
        if((*it)["name"] == name) {
            data["tables"].erase(it);
            break;
        }
    }

    storageService.writeObject(this->name + "/config.json", Buffer{(uint8_t*)data.dump().c_str(), data.dump().length()});
}