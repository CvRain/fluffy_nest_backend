//
// Created by cvrain on 25-1-30.
//

#include "object_storage_service.hpp"

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/Bucket.h>
#include <drogon/HttpAppFramework.h>
#include <ranges>
#include <stdexcept>

#include "services/logger.hpp"
#include "utils/drogon_specialization.hpp"

namespace service {
    void ObjectStorageService::init() {
        service::Logger::get_instance().get_logger()->info("Initialize object storage service");

        options = std::make_shared<Aws::SDKOptions>();
        Aws::InitAPI(*options);

        const auto object_storage = drogon::app().getCustomConfig()["object_storage"];

        Aws::S3::S3ClientConfiguration client_configuration;
        client_configuration.scheme           = Aws::Http::Scheme::HTTP;
        client_configuration.region           = object_storage["region"].as<std::string>();
        client_configuration.endpointOverride = object_storage["endpoint"].as<std::string>();

        s3_client = std::make_shared<Aws::S3::S3Client>(client_configuration);

        // 查询在象存储中，是否存在指定的bucket
        const auto buckets = list_buckets();
        if (not buckets.has_value()) {
            service::Logger::get_instance().get_logger()->error("Failed to list buckets {}", buckets.error());
            throw std::runtime_error("Failed to list buckets");
        };

        const auto bucket_name = object_storage["bucket"].as<std::string>();
        const auto result      = std::ranges::any_of(
                buckets.value(), [&bucket_name](const auto& bucket) { return bucket.GetName() == bucket_name; });

        if (not result) {
            service::Logger::get_instance().get_logger()->error("Bucket {} not found", bucket_name);
            throw std::runtime_error("Bucket not found");
        };
    }

    void ObjectStorageService::shutdown_api() const {
        Aws::ShutdownAPI(*options);
        service::Logger::get_instance().get_logger()->info("Shutdown object storage service");
    }

    auto ObjectStorageService::list_buckets() const
            -> std::expected<std::vector<Aws::S3::Model::Bucket>, std::string_view> {
        const auto outcome = s3_client->ListBuckets();
        if (not outcome.IsSuccess()) {
            service::Logger::get_instance().get_logger()->error("Failed to list buckets: {}",
                                                                outcome.GetError().GetMessage().data());
            return std::unexpected<std::string_view>(outcome.GetError().GetMessage().data());
        }

        const auto& buckets = outcome.GetResult().GetBuckets();
        service::Logger::get_instance().get_logger()->info("List buckets: {}", buckets.size());

        for (const auto& bucket: buckets) {
            service::Logger::get_instance().get_logger()->info("Bucket: {}", bucket.GetName());
        }
        return outcome.GetResult().GetBuckets();
    }

}  // namespace service
