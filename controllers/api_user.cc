#include "api_user.h"

#include "services/logger.hpp"
#include "services/user_services.hpp"
#include "types/nlohmann_json_response.hpp"
#include "types/type.hpp"
#include "utils/exception_handler.hpp"

using namespace api;

void User::size(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    const auto user_size = service::UserServices::get_instance().size();

    const nlohmann::json data{{"size", user_size}};

    type::BasicResponse basic_response{.code = k200OK, .message = "User::size k200OK", .result = "", .data = data};
    callback(newHttpJsonResponse(basic_response.to_json()));
}

void User::append(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    const auto &request_body = fromRequest<nlohmann::json>(*req);
    try {
        type::UserSchema user{};
        user.name     = request_body.at(type::UserSchema::key_name).get<std::string>();
        user.password = request_body.at(type::UserSchema::key_password).get<std::string>();
        user.email    = request_body.at(type::UserSchema::key_email).get<std::string>();

        const auto append_result = service::UserServices::get_instance().append(user);
        if (not append_result.has_value()) {
            type::BasicResponse basic_response{
                    .code = k400BadRequest, .message = "User::append k400BadRequest", .result = "", .data = {}};
            throw exception::BaseException{std::move(basic_response)};
        }

        if (service::UserServices::get_instance().email_exist(user.email).value() ) {
            type::BasicResponse basic_response{.code    = k400BadRequest,
                                               .message = "User::append k400BadRequest",
                                               .result  = "email exist",
                                               .data    = {}};
            service::Logger::get_instance().get_logger()->warn("User::append k400BadRequest email exist");
            callback(newHttpJsonResponse(basic_response.to_json()));
            return;
        }


        const auto uuid  = append_result.value();
        const auto is_ok = not uuid.empty();

        const auto data = nlohmann::json{
                {"is_ok", is_ok},
                {"uuid", uuid},
        };

        auto response =
                type::BasicResponse{.code = k200OK, .message = "User::append k200OK", .result = "", .data = data};

        callback(newHttpJsonResponse(response.to_json()));
    }
    catch (const std::exception &e) {
        exception::ExceptionHandler::handle(req, std::move(callback), e);
    }
}
