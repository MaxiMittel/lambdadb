//
// Created by Maximilian Mittelhammer on 30.11.22.
//

#include "StorageService.h"
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/HeadObjectRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <sstream>

StorageService::StorageService(std::string bucketName): BUCKET_NAME(std::move(bucketName)) {}

Buffer StorageService::readBytes(const std::string& key, size_t offset, size_t length) {
    Aws::S3::S3Client s3_client;

    Aws::S3::Model::GetObjectRequest object_request;
    object_request.SetBucket(BUCKET_NAME);
    object_request.SetKey(key);
    object_request.SetRange("bytes=" + std::to_string(offset) + "-" + std::to_string(offset + length - 1));

    auto get_object_outcome = s3_client.GetObject(object_request);

    if (get_object_outcome.IsSuccess()) {
        auto& retrieved_file = get_object_outcome.GetResultWithOwnership().GetBody();
        std::stringstream ss;
        ss << retrieved_file.rdbuf();
        
        auto content = ss.str();
        auto data = new uint8_t[content.size()];
        memcpy(data, content.c_str(), content.size());

        return Buffer{data, content.size()};
    } else {
        throw std::runtime_error("Could not read bytes from S3");
    }
}

Buffer StorageService::readObject(std::string key) {
    Aws::S3::S3Client s3_client;

    Aws::S3::Model::GetObjectRequest object_request;
    object_request.SetBucket(BUCKET_NAME);
    object_request.SetKey(key);

    auto get_object_outcome = s3_client.GetObject(object_request);

    if (get_object_outcome.IsSuccess()) {
        auto& retrieved_file = get_object_outcome.GetResultWithOwnership().GetBody();

        std::stringstream ss;
        ss << retrieved_file.rdbuf();

        auto content = ss.str();
        auto data = new uint8_t[content.size()];
        memcpy(data, content.c_str(), content.size());

        return Buffer{data, content.size()}; // TODO: reinterpret_cast<uint8_t*>(content.c_str())
    } else {
        throw std::runtime_error("Could not read object from S3");
    }
}

std::string StorageService::readObjectAsString(std::string key) {
    Aws::S3::S3Client s3_client;

    Aws::S3::Model::GetObjectRequest object_request;
    object_request.SetBucket(BUCKET_NAME);
    object_request.SetKey(key);

    auto get_object_outcome = s3_client.GetObject(object_request);

    if (get_object_outcome.IsSuccess()) {
        auto& retrieved_file = get_object_outcome.GetResultWithOwnership().GetBody();

        std::stringstream buffer;
        buffer << retrieved_file.rdbuf();
        return buffer.str();
    } else {
        throw std::runtime_error("Could not read object from S3");
    }
}

void StorageService::writeObject(std::string key, Buffer content) {
    Aws::S3::S3Client s3_client;

    Aws::S3::Model::PutObjectRequest object_request;
    object_request.SetBucket(BUCKET_NAME);
    object_request.SetKey(key);
    object_request.SetBody(Aws::MakeShared<Aws::StringStream>("SampleAllocationTag", std::string((char*)content.data, content.size)));

    auto put_object_outcome = s3_client.PutObject(object_request);

    if (!put_object_outcome.IsSuccess()) {
        throw std::runtime_error("Could not write object to S3");
    }
}

size_t StorageService::appendObject(std::string key, Buffer content) {
    Aws::S3::S3Client s3_client;

    Aws::S3::Model::GetObjectRequest object_request;
    object_request.SetBucket(BUCKET_NAME);
    object_request.SetKey(key);

    auto get_object_outcome = s3_client.GetObject(object_request);

    if (get_object_outcome.IsSuccess()) {
        auto& retrieved_file = get_object_outcome.GetResultWithOwnership().GetBody();
        size_t retreived_size = get_object_outcome.GetResult().GetContentLength();
        
        std::stringstream buffer;
        buffer << retrieved_file.rdbuf();

        auto retrieved_content = buffer.str();
        auto data = new uint8_t[retreived_size + content.size];
        memcpy(data, retrieved_content.c_str(), retreived_size);
        memcpy(data + retreived_size, content.data, content.size);
                    
        writeObject(key, Buffer{data, retreived_size + content.size});

        return retreived_size;
    } else {
        throw std::runtime_error("Could not read object from S3");
    }
}

void StorageService::writeBytes(std::string key, size_t offset, Buffer content) {
    Aws::S3::S3Client s3_client;

    Aws::S3::Model::GetObjectRequest object_request;
    object_request.SetBucket(BUCKET_NAME);
    object_request.SetKey(key);

    auto get_object_outcome = s3_client.GetObject(object_request);

    if (get_object_outcome.IsSuccess()) {
        auto& retrieved_file = get_object_outcome.GetResultWithOwnership().GetBody();
        std::stringstream buffer;
        buffer << retrieved_file.rdbuf();
        
        auto retrieved_content = buffer.str();
        auto data = new uint8_t[retrieved_content.size()];
        memcpy(data, retrieved_content.c_str(), retrieved_content.size());
        memcpy(data + offset, content.data, content.size);

        writeObject(key, Buffer{data, retrieved_content.size()});
    } else {
        throw std::runtime_error("Could not read object from S3");
    }
}

void StorageService::deleteBytes(std::string key, size_t offset, size_t length) {
    Aws::S3::S3Client s3_client;

    Aws::S3::Model::GetObjectRequest object_request;
    object_request.SetBucket(BUCKET_NAME);
    object_request.SetKey(key);

    auto get_object_outcome = s3_client.GetObject(object_request);

    if (get_object_outcome.IsSuccess()) {
        auto& retrieved_file = get_object_outcome.GetResultWithOwnership().GetBody();
        
        std::stringstream buffer;
        buffer << retrieved_file.rdbuf();

        auto retrieved_content = buffer.str();
        auto data = new uint8_t[retrieved_content.size() - length];
        memcpy(data, retrieved_content.c_str(), offset);
        memcpy(data + offset, retrieved_content.c_str() + offset + length, retrieved_content.size() - offset - length);

        writeObject(key, Buffer{data, retrieved_content.size() - length}); 
    } else {
        throw std::runtime_error("Could not read object from S3");
    }
}

void StorageService::deleteObject(std::string key) {
    Aws::S3::S3Client s3_client;

    Aws::S3::Model::DeleteObjectRequest object_request;
    object_request.SetBucket(BUCKET_NAME);
    object_request.SetKey(key);

    auto delete_object_outcome = s3_client.DeleteObject(object_request);

    if (!delete_object_outcome.IsSuccess()) {
        throw std::runtime_error("Could not delete object from S3");
    }
}

bool StorageService::exists(std::string key) {
    Aws::S3::S3Client s3_client;

    Aws::S3::Model::HeadObjectRequest object_request;
    object_request.SetBucket(BUCKET_NAME);
    object_request.SetKey(key);

    auto head_object_outcome = s3_client.HeadObject(object_request);

    return head_object_outcome.IsSuccess();
}