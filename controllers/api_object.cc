#include "api_object.h"

#include <expected>
#include <ranges>

#include "services/logger.hpp"
#include "services/object_storage_service.hpp"
#include "types/nlohmann_json_request.hpp"
#include "types/nlohmann_json_response.hpp"
#include "types/schema.hpp"
#include "utils/exception_handler.hpp"

namespace api {
    void Object::list_objects(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
        service::Logger::info("Object::list_objects");
        try {
            const auto& user_id     = req->getParameter(type::UserSchema::key_user_id.data());
            const auto  list_result = service::ObjectStorageService::get_instance().list_directory(user_id);

            if (not list_result.has_value()) {
                auto response = type::BasicResponse{.code    = k500InternalServerError,
                                                    .message = "Object::list_objects::k500InternalServerError",
                                                    .result  = list_result.error(),
                                                    .data    = ""};
                callback(toResponse(response.to_json()));
                return;
            }

            nlohmann::json objects_group;
            for (const auto& it: list_result.value()) {
                objects_group.push_back(it);
            }

            auto response = type::BasicResponse{
                    .code = k200OK, .message = "Object::list_objects::k200OK", .result = "", .data = objects_group};
            callback(toResponse(response.to_json()));
        }
        catch (const std::exception& e) {
            exception::ExceptionHandler::handle(req, std::move(callback), e);
        }
    }

    void Object::tree_objects(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
        service::Logger::info("Object::tree_objects");
        try {
            const auto& user_id     = req->getParameter(type::UserSchema::key_user_id.data());
            const auto  tree_result = service::ObjectStorageService::get_instance().tree_list_directory(user_id);

            if (not tree_result.has_value()) {
                auto response = type::BasicResponse{.code    = k500InternalServerError,
                                                    .message = "Object::tree_objects::k500InternalServerError",
                                                    .result  = tree_result.error(),
                                                    .data    = ""};
                callback(toResponse(response.to_json()));
                return;
            }

            auto response = type::BasicResponse{.code    = k200OK,
                                                .message = "Object::tree_objects::k200OK",
                                                .result  = "",
                                                .data    = tree_result.value()};
            callback(toResponse(response.to_json()));
        }
        catch (const std::exception& e) {
            exception::ExceptionHandler::handle(req, std::move(callback), e);
        }
    }

    void Object::exist_object(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
        service::Logger::info("Object::exist_object");
        try {
        }
        catch (const std::exception& e) {
            exception::ExceptionHandler::handle(req, std::move(callback), e);
        }
    }


    void Object::append_object(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
        service::Logger::info("Object::append_object");
        try {
        }
        catch (const std::exception& e) {
            exception::ExceptionHandler::handle(req, std::move(callback), e);
        }
    }
    void Object::remove_object(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
        service::Logger::info("Object::remove_object");
        try {
        }
        catch (const std::exception& e) {
            exception::ExceptionHandler::handle(req, std::move(callback), e);
        }
    }

    void Object::object_url(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
        service::Logger::info("Object::object_url");
        try {
            const auto request_body = fromRequest<nlohmann::json>(*req);
            const auto object_key   = request_body.at("object_key").get<std::string>();

            const auto result = service::ObjectStorageService::get_instance().get_object(object_key);
            if (not result.has_value()) {
                auto response = type::BasicResponse{.code    = k500InternalServerError,
                                                    .message = "Object::object_url::k500InternalServerError",
                                                    .result  = result.error(),
                                                    .data    = ""};
                callback(toResponse(response.to_json()));
                return;
            }
            auto response = type::BasicResponse{
                    .code = k200OK, .message = "Object::object_url::k200OK", .result = "", .data = result.value()};
            callback(toResponse(response.to_json()));
        }
        catch (const std::exception& e) {
            exception::ExceptionHandler::handle(req, std::move(callback), e);
        }
    }


}  // namespace api
