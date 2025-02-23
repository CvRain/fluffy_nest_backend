#include "api_check.h"

#include "fmt/format.h"
#include "services/logger.hpp"
#include "services/user_services.hpp"
#include "types/nlohmann_json_response.hpp"
#include "utils/drogon_specialization.hpp"
#include "utils/exception_handler.hpp"

using namespace api;


void Check::enable_register(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service::Logger::get_instance().get_logger()->debug("User::enable_register");
    try {
        if (app().getCustomConfig()["enable_add_user"].asBool()) {
            type::BasicResponse basic_response{.code    = k200OK,
                                               .message = "User::enable_register k200OK",
                                               .result  = "server enable add user",
                                               .data    = ""};
            callback(newHttpJsonResponse(basic_response.to_json()));
        }
        else {
            type::BasicResponse basic_response{.code    = k403Forbidden,
                                               .message = "User::enable_register k403Forbidden",
                                               .result  = "server disable add user",
                                               .data    = ""};
            callback(newHttpJsonResponse(basic_response.to_json()));
        }
    }
    catch (const std::exception &e) {
        exception::ExceptionHandler::handle(req, std::move(callback), e);
    }
}

void Check::name_exist(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service::Logger::get_instance().get_logger()->debug("User::name_exist");
    try {
        const auto  request_body = fromRequest<nlohmann::json>(*req);
        const auto &user_name    = request_body.at("name").get<std::string>();

        const auto result = service::UserServices::name_exist(user_name);

        auto build_response = [&](const bool exist) {
            const auto status = exist ? k200OK : k404NotFound;
            return type::BasicResponse{.code    = status,
                                       .message = fmt::format("User::name_exist {}", exist ? "k200OK" : "k404NotFound"),
                                       .result  = fmt::format("user name {}", exist ? "exist" : "not exist"),
                                       .data    = ""}
                    .to_json();
        };

        auto response = result.and_then([&](const bool exist) {
                                  return std::make_optional(build_response(exist));
                              }).value_or(nlohmann::json{});
        callback(newHttpJsonResponse(std::move(response)));
    }
    catch (const std::exception &e) {
        exception::ExceptionHandler::handle(req, std::move(callback), e);
    }
}

void Check::email_exist(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service::Logger::get_instance().get_logger()->debug("User::email_exist");
    try {
        const auto  request_body = fromRequest<nlohmann::json>(*req);
        const auto &user_email   = request_body.at("email").get<std::string>();

        const auto result = service::UserServices::email_exist(user_email);

        auto build_response = [&](const bool exist) {
            const auto status = exist ? k200OK : k404NotFound;
            return type::BasicResponse{
                    .code    = status,
                    .message = fmt::format("User::email_exist {}", exist ? "k200OK" : "k404NotFound"),
                    .result  = fmt::format("user email {}", exist ? "exist" : "not exist"),
                    .data    = ""}
                    .to_json();
        };

        auto response = result.and_then([&](const bool exist) {
                                  return std::make_optional(build_response(exist));
                              }).value_or(nlohmann::json{});

        callback(newHttpJsonResponse(std::move(response)));
    }
    catch (const std::exception &e) {
        exception::ExceptionHandler::handle(req, std::move(callback), e);
    }
}

void Check::id_exist(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service::Logger::get_instance().get_logger()->debug("User::id_exist");

    try {
        const auto  request_body = fromRequest<nlohmann::json>(*req);
        const auto &user_id      = request_body.at("id").get<std::string>();

        const auto result = service::UserServices::id_exist(user_id);

        auto build_response = [&](const bool exist) {
            const auto status = exist ? k200OK : k404NotFound;
            return type::BasicResponse{.code    = status,
                                       .message = fmt::format("User::id_exist {}", exist ? "k200OK" : "k404NotFound"),
                                       .result  = fmt::format("user id {}", exist ? "exist" : "not exist"),
                                       .data    = ""}
                    .to_json();
        };

        auto response = result.and_then([&](const bool exist) {
                                  return std::make_optional(build_response(exist));
                              }).value_or(nlohmann::json{});

        callback(newHttpJsonResponse(std::move(response)));
    }
    catch (const std::exception &e) {
        exception::ExceptionHandler::handle(req, std::move(callback), e);
    }
}
