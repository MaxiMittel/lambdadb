//
// Created by Maximilian Mittelhammer on 30.11.22.
//

#include "Table.h"

Table::Table(std::string name, std::string database, std::vector<Column> columns, StorageService& storageService): name(std::move(name)), database(std::move(database)), columns(std::move(columns)), storageService(storageService) {
    // Calculate the length of the items
    for (const auto& column : this->columns) {
        item_length += column.size;
    }

    // Check if primary key exists
    bool hasPrimaryKey = false;
    int primaryKeyCount = 0;
    for (const auto& column : this->columns) {
        if (column.primary_key) {
            hasPrimaryKey = true;
            primaryKeyCount++;
        }
    }

    if (!hasPrimaryKey) {
        throw std::runtime_error("Table must have a primary key");
    }

    if (primaryKeyCount > 1) {
        throw std::runtime_error("Table can only have one primary key");
    }

    OFFSET_MAP_KEY = this->database + "/" + this->name + ".offset_map";
    DATA_KEY = this->database + "/" + this->name + ".data";

    if(!storageService.exists(OFFSET_MAP_KEY)) {
        storageService.writeObject(OFFSET_MAP_KEY, Buffer{nullptr, 0});
    }

    if(!storageService.exists(DATA_KEY)) {
        storageService.writeObject(DATA_KEY, Buffer{nullptr, 0});
    }
}

std::string Table::getName() const {
    return name;
}

std::vector<Column> Table::getColumns() const {
    return columns;
}

std::vector<std::vector<std::shared_ptr<DataEntryBase>>> Table::getItems() const {
    std::vector<std::vector<std::shared_ptr<DataEntryBase>>> items;

    // Fill the items vector with columns
    for (size_t i = 0; i < columns.size(); i++) {
        items.emplace_back(std::vector<std::shared_ptr<DataEntryBase>>());
    }

    // Load the items from the storage
    auto content = storageService.readObject(DATA_KEY);

    for (size_t i = 0; i < content.size; i += item_length) {
        size_t offset = i;
        for (size_t column_index = 0; column_index < columns.size(); column_index++) {
            switch (columns[column_index].type) {
                case DataType::INTEGER:
                    items[column_index].emplace_back(std::make_shared<DataEntry<int32_t>>(*(int32_t*)(content.data + offset), DataType::INTEGER));
                    break;
                case DataType::VARCHAR:
                    items[column_index].emplace_back(std::make_shared<DataEntry<std::string>>(std::string(reinterpret_cast<char*>(content.data + offset), columns[column_index].size), DataType::VARCHAR));
                    break;
            }
            offset += columns[column_index].size;
        }
    }

    // Print the items
    for (size_t i = 0; i < items[0].size(); i++) {
        for (size_t j = 0; j < items.size(); j++) {
            switch (columns[j].type) {
                case DataType::INTEGER:
                    std::cout << static_cast<DataEntry<int32_t>*>(items[j][i].get())->getValue() << " ";
                    break;
                case DataType::VARCHAR:
                    std::cout << static_cast<DataEntry<std::string>*>(items[j][i].get())->getValue() << " ";
                    break;
            }
        }
        std::cout << std::endl;
    }

    return items;
}

void Table::insertItem(std::vector<std::shared_ptr<DataEntryBase>> item) {
    // Check if the item has the correct length
    if (item.size() != columns.size()) {
        throw std::runtime_error("Item has the wrong length");
    }

    // Check if the item has the correct types
    for (size_t i = 0; i < item.size(); i++) {
        if (item[i]->getType() != columns[i].type) {
            throw std::runtime_error("Item has the wrong type");
        }
    }

    // Write the item to the storage
    uint8_t* data = new uint8_t[item_length]{};

    size_t offset = 0;
    for (size_t i = 0; i < item.size(); i++) {
        switch (item[i]->getType()) {
            case DataType::INTEGER:
                *(int32_t*)(data + offset) = static_cast<DataEntry<int32_t>*>(item[i].get())->getValue();
                break;
            case DataType::VARCHAR:
                std::string string_value = static_cast<DataEntry<std::string>*>(item[i].get())->getValue();
                memcpy(data + offset, string_value.c_str(), string_value.length());
                break;
        }
        offset += columns[i].size;
    }

    size_t inserted_offset = storageService.appendObject(DATA_KEY, Buffer{data, item_length});

    // Get primary key
    std::string primary_key = "";
    for (size_t i = 0; i < columns.size(); i++) {
        if (columns[i].primary_key) {
            switch (columns[i].type) {
                case DataType::INTEGER:
                    primary_key = std::to_string(static_cast<DataEntry<int32_t>*>(item[i].get())->getValue());
                    break;
                case DataType::VARCHAR:
                    primary_key = static_cast<DataEntry<std::string>*>(item[i].get())->getValue();
                    break;
            };
            break;
        }
    }

    std::string offset_key = primary_key + ":" + std::to_string(inserted_offset) + ",";
    storageService.appendObject(OFFSET_MAP_KEY, Buffer{(uint8_t*)offset_key.c_str(), offset_key.size()});

    delete[] data;
}

void Table::deleteItem(std::string key) {
    // TODO: Implement
    std::cout << "Not implemented: DELETE " << key << std::endl;
    //size_t offset = getOffsetMap().at(key);
    //storageService.deleteBytes(DATA_KEY, offset, item_length);
}

void Table::updateItem(std::string key, std::vector<std::shared_ptr<DataEntryBase>> item) {
    // TODO: Implement
    std::cout << "Not implemented: UPDATE " << key << std::endl;
    std::ignore = key;
    std::ignore = item;
}

std::unordered_map<std::string, size_t> Table::getOffsetMap() {
    if (offsetMapInitialized) {
        return offsetMap;
    }

    // Read offset map from storage
    auto content = storageService.readObject(DATA_KEY + ".offsetmap");
    std::string data = std::string((char*)content.data);

    // Parse offset map (format: "key1:offset1,key2:offset2,...")
    size_t offset = 0;
    while (offset < data.size()) {
        size_t columnEnd = data.find(':', offset);
        size_t offsetEnd = data.find(',', offset);
        if (offsetEnd == std::string::npos) {
            offsetEnd = data.size();
        }
        offsetMap[data.substr(offset, columnEnd - offset)] = std::stoi(data.substr(columnEnd + 1, offsetEnd - columnEnd - 1));
        offset = offsetEnd + 1;
    }

    offsetMapInitialized = true;
    return offsetMap;
}

/*Table Table::join(Table other, JoinType type, std::string leftColumn, std::string rightColumn) const {
    return Table(__1::basic_string(), __1::vector());
}*/
