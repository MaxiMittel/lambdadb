//
// Created by Maximilian Mittelhammer on 30.11.22.
//

#ifndef LAMBDADB_STORAGESERVICE_H
#define LAMBDADB_STORAGESERVICE_H

#include <string>
#include <optional>
#include <aws/core/Aws.h>

struct Buffer {
    uint8_t* data;
    size_t size;
};

class StorageService {
private:
    std::string BUCKET_NAME;

public:
    StorageService(std::string bucketName);

    /**
     * Reads a byterange from the given file
     * @param key The key of the file
     * @param offset The offset to start reading from (in bytes)
     * @param length The length to read (in bytes)
     * @return The read bytes
     */
    Buffer readBytes(const std::string& key, size_t offset, size_t length);

    /**
     * Downloads a file from the storage
     * @param key The name of the file
     * @return The content of the file
     */
    Buffer readObject(std::string key);

    /**
     * Uploads a file to the storage
     * @param key The name of the file
     * @param content The content of the file
     */
    void writeObject(std::string key, Buffer content);

    /**
     * Appends to the end of a file
     * @param key The name of the file
     * @param content The content to append
     * @return The offset of the new entry
     */
    size_t appendObject(std::string key, Buffer content);

    /**
     * Writes bytes to a file
     * @param key The name of the file
     * @param offset The offset to start writing to (in bytes)
     * @param content The content to write
     */
    void writeBytes(std::string key, size_t offset, Buffer content);

    /**
     * Delete bytes from a file
     * @param key The name of the file
     * @param offset The offset to start deleting from (in bytes)
     * @param length The length to delete (in bytes)
     */
    void deleteBytes(std::string key, size_t offset, size_t length);

    /**
     * Deletes a file from the storage
     * @param key The name of the file
     */
    void deleteObject(std::string key);
    
    /**
     * Object exists
     * @param key The name of the file
     * @return True if the object exists
     */
    bool exists(std::string key);
};


#endif //LAMBDADB_STORAGESERVICE_H
