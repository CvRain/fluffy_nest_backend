/**
 *
 *  filter_UserAuth.cc
 *
 */

#include "filter_UserAuth.h"

#include "services/logger.hpp"
#include "services/user_services.hpp"
#include "types/nlohmann_json_response.hpp"
#include "types/type.hpp"
#include "utils/date.h"
#include "utils/drogon_specialization.hpp"
#include "utils/exception_handler.hpp"
#include "utils/string.hpp"

using namespace drogon;

namespace filter {
    void UserAuth::invoke(const HttpRequestPtr& req, MiddlewareNextCallback&& nextCb, MiddlewareCallback&& mcb) {
        service::Logger::get_instance().get_logger()->debug("UserAuth::invoke");

        try {
            const auto& token = req->getHeader("Authorization");

            if (token.empty()) {
                service::Logger::get_instance().get_logger()->debug("UserAuth::invoke: Authorization header is empty");
                type::BasicResponse response{.code    = k401Unauthorized,
                                             .message = "k401Unauthorized",
                                             .result  = "Authorization header is empty"};
                mcb(newHttpJsonResponse(response.to_json()));
                return;
            }

            const auto& current_time  = fluffy_utils::Date::get_current_timestamp_32();
            const auto& max_token_sec = drogon::app().getCustomConfig()["max_token_sec"].asInt();
            const auto& random_string = drogon::app().getCustomConfig()["random_string"].asString();

            const auto parse_jwt = [&]() {
                auto temp = fluffy_utils::StringEncryption::parse_jwt(token, random_string);
                if (not temp.has_value()) {
                    throw exception::BaseException{{.code    = k401Unauthorized,
                                                    .message = "k401Unauthorized",
                                                    .result  = "Failed to parse jwt string"}};
                }
                return std::move(temp.value());
            };

            const auto [header, payload] = parse_jwt();

            schema::JwtBody jwt_body;
            jwt_body.header.typ = header.at(type::basic_value::jwt::typ).get<std::string>();
            jwt_body.header.alg = header.at(type::basic_value::jwt::alg).get<std::string>();

            if (jwt_body.header.typ != "JWT" || jwt_body.header.alg != "HS256") {
                type::BasicResponse response{
                        .code = k401Unauthorized, .message = "k401Unauthorized", .result = "Invalid jwt header"};
                mcb(newHttpJsonResponse(response.to_json()));
                return;
            }

            jwt_body.payload.iss     = payload.at(type::basic_value::jwt::iss).get<std::string>();
            jwt_body.payload.sub     = payload.at(type::basic_value::jwt::sub).get<std::string>();
            jwt_body.payload.aud     = payload.at(type::basic_value::jwt::aud).get<int>();
            jwt_body.payload.exp     = payload.at(type::basic_value::jwt::exp).get<int>();
            jwt_body.payload.iat     = payload.at(type::basic_value::jwt::iat).get<int>();
            jwt_body.payload.user_id = payload.at(type::UserSchema::key_id).get<std::string>();

            if (current_time - jwt_body.payload.iat > max_token_sec) {
                type::BasicResponse response{
                        .code = k401Unauthorized, .message = "k401Unauthorized", .result = "Token is expired"};
                mcb(newHttpJsonResponse(response.to_json()));
                return;
            }

            if (jwt_body.payload.iss != "fluffy_nest" || jwt_body.payload.sub != "login") {
                type::BasicResponse response{.code    = k401Unauthorized,
                                             .message = "k401Unauthorized",
                                             .result  = "Invalid jwt issuer or subject"};
                return;
            }

            const auto& id_exist = service::UserServices::get_instance().id_exist(jwt_body.payload.user_id);
            if (not id_exist.has_value()) {
                throw exception::BaseException{
                        {.code    = k401Unauthorized,
                         .message = "k401Unauthorized",
                         .result = "service::UserServices::get_instance().id_exist(jwt_body.payload.user_id) nullopt"}};
            }
            if (not id_exist.value()) {
                type::BasicResponse response{
                        .code = k401Unauthorized, .message = "k401Unauthorized", .result = "User id not found"};
                mcb(newHttpJsonResponse(response.to_json()));
                return;
            }

            req->attributes()->insert("user_id", jwt_body.payload.user_id);
            nextCb([&, mcb = std::move(mcb)](const HttpResponsePtr& resp) {
                mcb(resp);
            });
        }
        catch (const std::exception& e) {
            exception::ExceptionHandler::handle(req, std::move(mcb), e);
        }
    }

}  // namespace filter
