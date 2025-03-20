//
// Created by cvrain on 25-3-3.
//

#include "user_validator_middleware.hpp"

#include "services/logger.hpp"
#include "services/user_services.hpp"
#include "types/nlohmann_json_request.hpp"
#include "types/nlohmann_json_response.hpp"
#include "types/schema.hpp"
#include "types/type.hpp"
#include "utils/exception_handler.hpp"


namespace filter {

    void UserIdExistMiddleware::invoke(const HttpRequestPtr&    req,
                                       MiddlewareNextCallback&& nextCb,
                                       MiddlewareCallback&&     mcb) {
        service::Logger::info("UserIdExistMiddleware::invoke");

        try {
            std::string user_id{};
            if (req->method() != HttpMethod::Get) {
                user_id = req->getJsonObject()->operator[](type::UserSchema::key_user_id.data()).as<std::string>();
            }
            else {
                user_id = req->getParameter(type::UserSchema::key_user_id.data());
            }

            const auto user_exist_result = service::UserServices::id_exist(user_id);
            if (not user_exist_result.has_value()) {
                auto response = type::BasicResponse{.code    = k500InternalServerError,
                                                    .message = "UserIdExistMiddleware k500InternalServerError",
                                                    .result  = "Failed to find user",
                                                    .data    = ""};
                mcb(toResponse(response.to_json()));
                return;
            }

            if (user_exist_result.value() == false) {
                auto response = type::BasicResponse{.code    = k404NotFound,
                                                    .message = "UserIdExistMiddleware k404NotFound",
                                                    .result  = "User not found",
                                                    .data    = ""};
                mcb(toResponse(response.to_json()));
                return;
            }
            nextCb([&, mcb = std::move(mcb)](const HttpResponsePtr& resp) { mcb(resp); });
        }
        catch (const std::exception& e) {
            exception::ExceptionHandler::handle(req, std::move(mcb), e);
        }
    }

    void UserEmailExistMiddleware::invoke(const HttpRequestPtr&    req,
                                          MiddlewareNextCallback&& nextCb,
                                          MiddlewareCallback&&     mcb) {
        service::Logger::info("UserEmailExistMiddleware::invoke");

        try {
            std::string email{};
            if (req->method() != HttpMethod::Get) {
                email = req->getJsonObject()->operator[](type::UserSchema::key_email.data()).as<std::string>();
            }
            else {
                email = req->getParameter(type::UserSchema::key_email.data());
            }

            if (email.empty()) {
                auto response = type::BasicResponse{.code    = k400BadRequest,
                                                    .message = "UserEmailExistMiddleware k400BadRequest",
                                                    .result  = "Email is empty",
                                                    .data    = ""};
                mcb(toResponse(response.to_json()));
                return;
            }

            std::string user_email;
            std::ranges::transform(email.begin(), email.end(), std::back_inserter(user_email), [](const auto& c) {
                return std::tolower(c);
            });
            service::Logger::info_runtime("found user: {}", user_email);

            const auto user_email_exist = service::UserServices::email_exist(user_email);
            if (not user_email_exist.has_value()) {
                auto response = type::BasicResponse{.code    = k500InternalServerError,
                                                    .message = "UserEmailExistMiddleware k500InternalServerError",
                                                    .result  = "Failed to find user",
                                                    .data    = ""};
                mcb(toResponse(response.to_json()));
                return;
            }

            if (user_email_exist.value() == false) {
                auto response = type::BasicResponse{.code    = k404NotFound,
                                                    .message = "UserEmailExistMiddleware k404NotFound",
                                                    .result  = "User not found",
                                                    .data    = ""};
                mcb(toResponse(response.to_json()));
                return;
            }

            nextCb([&, mcb = std::move(mcb)](const HttpResponsePtr& resp) { mcb(resp); });
        }
        catch (const std::exception& e) {
            exception::ExceptionHandler::handle(req, std::move(mcb), e);
        }
    }

    void UserNameExistMiddleware::invoke(const HttpRequestPtr&    req,
                                         MiddlewareNextCallback&& nextCb,
                                         MiddlewareCallback&&     mcb) {
        service::Logger::info("UserNameExistMiddleware::invoke");

        try {
            std::string user_name{};
            if (req->method() != HttpMethod::Get) {
                user_name = req->getJsonObject()->operator[](type::UserSchema::key_name.data()).as<std::string>();
            }
            else {
                user_name = req->getParameter(type::UserSchema::key_name.data());
            }

            const auto user_name_exist = service::UserServices::name_exist(user_name);

            if (not user_name_exist.has_value()) {
                auto response = type::BasicResponse{.code    = k500InternalServerError,
                                                    .message = "UserNameExistMiddleware k500InternalServerError",
                                                    .result  = "Failed to find user",
                                                    .data    = ""};
                mcb(toResponse(response.to_json()));
                return;
            }

            if (user_name_exist.value() == false) {
                auto response = type::BasicResponse{.code    = k404NotFound,
                                                    .message = "UserNameExistMiddleware k404NotFound",
                                                    .result  = "User not found",
                                                    .data    = ""};
                mcb(toResponse(response.to_json()));
                return;
            }
            nextCb([&, mcb = std::move(mcb)](const HttpResponsePtr& resp) { mcb(resp); });
        }
        catch (const std::exception& e) {
            exception::ExceptionHandler::handle(req, std::move(mcb), e);
        }
    }
}  // namespace filter
