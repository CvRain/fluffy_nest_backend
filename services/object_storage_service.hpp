//
// Created by cvrain on 25-1-30.
//

#ifndef OBJECT_STORAGE_SERVICE_HPP
#define OBJECT_STORAGE_SERVICE_HPP

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <expected>

#include "models/singleton_prototype.hpp"
#include "types/base.hpp"
#include "types/nlohmann_json_request.hpp"


namespace service {
    class ObjectStorageService : public models::Singleton<ObjectStorageService> {
    public:
        void               init();
        void               shutdown_api() const;
        [[nodiscard]] auto list_buckets() const -> type::result<std::vector<Aws::S3::Model::Bucket>>;
        [[nodiscard]] auto put_object(const std::string& file_name, const std::string& buffer) const
                -> type::result<std::string>;
        [[nodiscard]] auto recursive_directory(const std::string& path = "") -> type::result<nlohmann::json>;

        [[nodiscard]] static auto create_directory(const std::string& path) -> type::result<bool>;

    private:
        std::shared_ptr<Aws::S3::S3Client> s3_client;
        std::shared_ptr<Aws::SDKOptions>   options;
        std::string                        bucket_name;
    };

}  // namespace service


#endif  // OBJECT_STORAGE_SERVICE_HPP
