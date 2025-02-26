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
        /**
         * 初始化对象存储服务
         */
        void init();

        /**
         * 关闭对象存储服务, 清理资源
         */
        void shutdown_api() const;

        /**
         * 列出此对象存储服务中的所有存储桶
         * @return 正常则返回所有存储桶，失败则返回 `std::unexpected'
         */
        [[nodiscard]] auto list_buckets() const -> type::result<std::vector<Aws::S3::Model::Bucket>>;

        /**
         * 上传对象
         * @param object_key 对象键，例如："test/test.txt"
         * @param buffer 对象内容
         * @return
         */
        [[nodiscard]] auto put_object(const std::string& object_key, const std::string& buffer) const
                -> type::result<std::string>;

        /**
         * 通过对象键删除对象
         * @param object_key 对象键，例如："test/test.txt"
         * @return 删除失败返回 `std::unexpected`
         */
        [[nodiscard]] auto delete_object(const std::string& object_key) const -> type::result<bool>;


        /**
         * 删除一组对象
         * @param object_key 对象键组
         * @return 删除失败返回 `std::unexpected`
         */
        [[nodiscard]] auto delete_objects(std::vector<std::string> object_key) const -> type::result<bool>;

        /**
         * 递归遍历存储桶中的所有对象
         * @param entry_path 遍历的入口路径，默认为根目录
         * @return
         */
        [[nodiscard]] auto recursive_directory(const std::string& entry_path = "") const
                -> type::result<nlohmann::json>;

        [[nodiscard]] static auto create_directory(const std::string& path) -> type::result<bool>;

    private:
        std::shared_ptr<Aws::S3::S3Client> s3_client;
        std::shared_ptr<Aws::SDKOptions>   options;
        std::string                        bucket_name;
    };

}  // namespace service


#endif  // OBJECT_STORAGE_SERVICE_HPP
