//
// Created by Maximilian Mittelhammer on 04.12.22.
//

#ifndef LAMBDADB_OFFSETMAP_H
#define LAMBDADB_OFFSETMAP_H

#include "../storage/StorageService.h"

class OffsetMap {
private:
    StorageService &storageService;
    std::string database;
    std::string name;

public:
    OffsetMap(StorageService &storageService, std::string database, std::string name);

    /**
     * Returns the offset for the given key
     * @param key The key
     * @return The offset
     */
    [[nodiscard]] size_t getOffset(std::string key) const;

    /**
     * Checks if the given key exists
     * @param key The key
     * @return true/false
     */
    [[nodiscard]] bool exists(std::string key) const;

    /**
     * Deletes the given key
     */
    void deleteKey(std::string key);
};

#endif // LAMBDADB_OFFSETMAP_H
