//
// Created by Maximilian Mittelhammer on 04.12.22.
//

#include "OffsetMap.h"

OffsetMap::OffsetMap(StorageService &storageService, std::string database, std::string name) : storageService(storageService), database(std::move(database)), name(std::move(name)) {}

size_t OffsetMap::getOffset(std::string key) const {
    auto content = storageService.readObject(database + "/" + name + "_offsets/" + key);

    if (content.size == 0) {
        throw std::runtime_error("Key does not exist");
    }

    return std::stoull((char*)content.data);
}

bool OffsetMap::exists(std::string key) const {
    return storageService.exists(database + "/" + name + "_offsets/" + key);
}

void OffsetMap::deleteKey(std::string key) {
    storageService.deleteObject(database + "/" + name + "_offsets/" + key);
}