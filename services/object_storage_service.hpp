//
// Created by cvrain on 25-1-30.
//

#ifndef OBJECT_STORAGE_SERVICE_HPP
#define OBJECT_STORAGE_SERVICE_HPP

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <expected>

#include "models/singleton_prototype.hpp"


namespace service {
    class ObjectStorageService : public models::Singleton<ObjectStorageService> {
    public:
        void init();
        void shutdown_api() const;
        auto list_buckets() const -> std::expected<std::vector<Aws::S3::Model::Bucket>, std::string_view>;
    private:
        std::shared_ptr<Aws::S3::S3Client> s3_client;
        std::shared_ptr<Aws::SDKOptions> options;
    };

}  // namespace service


#endif  // OBJECT_STORAGE_SERVICE_HPP
