#include "api_article.h"

#include "services/logger.hpp"
#include "services/object_storage_service.hpp"
#include "types/nlohmann_json_response.hpp"
#include "users.hpp"
#include "utils/drogon_specialization.hpp"
#include "utils/exception_handler.hpp"

namespace api {
    void Article::create_directory(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
        service::Logger::info("Article::create_directory");

        try {
            const auto request_body = fromRequest<nlohmann::json>(*req);

            const auto& user_id   = request_body.at("user_id").get<std::string>();
            const auto& path      = request_body.at("path").get<std::string>();
            const auto& full_path = fmt::format("{}/{}", user_id, path);

            auto build_response = [](const int code, const std::string& message, const std::string& result) {
                return newHttpJsonResponse(type::BasicResponse{.code    = code,
                                                               .message = std::forward<decltype(message)>(message),
                                                               .result  = std::forward<decltype(result)>(result),
                                                               .data    = ""}
                                                   .to_json());
            };

            if (auto result = service::ObjectStorageService::create_directory(full_path)) {
                callback(build_response(k200OK, "Directory created successfully", "Success"));
            }
            else {
                service::Logger::get_instance().get_logger()->error("Create directory failed: {}",
                                                                    result.error());  // 添加错误日志
                callback(build_response(k409Conflict, "Directory creation conflict", result.error()));
            }
        }
        catch (std::exception& e) {
            exception::ExceptionHandler::handle(req, std::move(callback), e);
        }
    }

    void Article::recursive_directory(const HttpRequestPtr&                         req,
                                      std::function<void(const HttpResponsePtr&)>&& callback) {
        service::Logger::info("Article::recursive_directory");
        try {
            const auto request = service::ObjectStorageService::get_instance().tree_list_directory();
            callback(toResponse(request.value_or(nlohmann::json{})));
        }
        catch (std::exception& e) {
            exception::ExceptionHandler::handle(req, std::move(callback), e);
        }
    }

    void Article::personal_directory(const HttpRequestPtr&                         req,
                                     std::function<void(const HttpResponsePtr&)>&& callback) {
        service::Logger::info("Article::personal_directory");
        try {
            const auto  request_body = *req->getJsonObject();
            const auto& user_id      = request_body[type::UserSchema::key_id.data()].as<std::string>();

            const auto result   = service::ObjectStorageService::get_instance().tree_list_directory(user_id);
            auto       response = type::BasicResponse{.code    = k200OK,
                                                      .message = "Article::personal_directory k200OK",
                                                      .result  = "Ok",
                                                      .data    = result.value_or(nlohmann::json{})};

            callback(toResponse(response.to_json()));
        }
        catch (std::exception& e) {
            exception::ExceptionHandler::handle(req, std::move(callback), e);
        }
    }

    void Article::append_object(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {}

    void Article::remove_object(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
        service::Logger::info("Article::remove_object");
        try {
            const auto request_body = fromRequest<nlohmann::json>(*req);
            //todo 还差一个检查object是否存在
        }
        catch (std::exception& e) {
            exception::ExceptionHandler::handle(req, std::move(callback), e);
        }
    }


}  // namespace api
