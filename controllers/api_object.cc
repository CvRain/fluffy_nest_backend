#include "api_object.h"

#include "services/object_storage_service.hpp"
#include "types/nlohmann_json_request.hpp"
#include "types/nlohmann_json_response.hpp"
#include "types/schema.hpp"
#include "utils/exception_handler.hpp"

#include <expected>
#include <ranges>

namespace api {
    void Object::list_objects(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
        try {
            const auto request_body = fromRequest<nlohmann::json>(*req);
            const auto user_id      = request_body.at(type::UserSchema::key_id).get<std::string>();

            const auto list_result = service::ObjectStorageService::get_instance().list_directory(user_id);

            if (not list_result.has_value()) {
                auto response = type::BasicResponse{.code    = k500InternalServerError,
                                                    .message = "Object::list_objects::k500InternalServerError",
                                                    .result  = list_result.error(),
                                                    .data    = ""};
                callback(toResponse(response.to_json()));
                return;
            }

            nlohmann::json objects_group;
            for (auto it = list_result.value().begin(); it != list_result.value().end(); ++it) {
                objects_group.push_back(*it);
            }

            auto response = type::BasicResponse{.code    = k200OK,
                                                .message = "Object::list_objects::k200OK",
                                                .result  = "",
                                                .data    = objects_group};
            callback(toResponse(response.to_json()));
        }
        catch (const std::exception& e) {
            exception::ExceptionHandler::handle(req, std::move(callback), e);
        }
    }

    void Object::tree_objects(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
        try {
            const auto request_body = fromRequest<nlohmann::json>(*req);
            const auto user_id      = request_body.at(type::UserSchema::key_id).get<std::string>();

            const auto tree_result = service::ObjectStorageService::get_instance().tree_list_directory(user_id);

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
        }catch (const std::exception& e) {
            exception::ExceptionHandler::handle(req, std::move(callback), e);
        }
    }
}  // namespace api
