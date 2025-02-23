//
// Created by cvrain on 25-1-20.
//

#include "user_services.hpp"

#include "services/logger.hpp"
#include "utils/date.h"
#include "utils/string.hpp"

namespace service {
    size_t UserServices::size() {
        const auto result = models::UserModel::size();
        if (not result.has_value()) {
            service::Logger::get_instance().get_logger()->error("UserServices::size {}", result.error());
        }
        return result.value_or(0);
    }

    auto UserServices::append(const type::UserSchema& user) -> type::result<std::string> {
        const auto will_insert_user = type::UserSchema{.id          = drogon::utils::getUuid(),
                                                       .name        = user.name,
                                                       .password    = user.password,
                                                       .email       = user.email,
                                                       .role        = 0,
                                                       .icon        = "",
                                                       .signature   = "",
                                                       .create_time = trantor::Date::now(),
                                                       .update_time = trantor::Date::now()};
        const auto insert_result    = models::UserModel::append(will_insert_user);
        if (not insert_result.has_value()) {
            service::Logger::get_instance().get_logger()->error("UserServices::append {}", insert_result.error());
            return insert_result.error().data();
        }
        return insert_result.value() ? will_insert_user.id : "";
    }

    auto UserServices::user_exist(const std::string& id, const std::string& email) -> type::result<bool> {
        const auto result = models::UserModel::has_id(id) and models::UserModel::has_email(email);
        return result;
    }

    auto UserServices::id_exist(const std::string& id) -> std::optional<bool> {
        const auto result = models::UserModel::has_id(id);
        if (not result.has_value()) {
            service::Logger::get_instance().get_logger()->error("UserServices::id_exist {}", result.error());
            throw std::runtime_error(result.error().data());
        }
        return result.value();
    }

    auto UserServices::name_exist(const std::string& name) -> std::optional<bool> {
        service::Logger::get_instance().get_logger()->info("UserServices::name_exist {}", name);
        const auto result = models::UserModel::has_name(name);
        if (not result.has_value()) {
            service::Logger::get_instance().get_logger()->error("UserServices::name_exist {}", result.error());
            throw std::runtime_error(result.error().data());
        }
        return result.value();
    }

    auto UserServices::email_exist(const std::string& email) -> std::optional<bool> {
        const auto result = models::UserModel::has_email(email);
        if (not result.has_value()) {
            service::Logger::get_instance().get_logger()->error("UserServices::email_exist {}", result.error());
            throw std::runtime_error(result.error().data());
        }
        return result.value();
    }

    auto UserServices::remove_by_id(const std::string& id) -> std::optional<bool> {
        const auto result = models::UserModel::remove_by_id(id);

        if (not result.has_value()) {
            service::Logger::get_instance().get_logger()->error("UserServices::remove_by_id {}", result.error());
            throw std::runtime_error(result.error().data());
        }

        if (result.value() == 0) {
            service::Logger::get_instance().get_logger()->warn("UserServices::remove_by_id {}", result.error());
            return false;
        }
        return true;
    }

    auto UserServices::remove_by_email(const std::string& email) -> std::optional<bool> {
        const auto result = models::UserModel::remove_by_email(email);
        if (not result.has_value()) {
            service::Logger::get_instance().get_logger()->error("UserServices::remove_by_email {}", result.error());
            throw std::runtime_error(result.error().data());
        }
        if (result.value() == 0) {
            service::Logger::get_instance().get_logger()->warn("UserServices::remove_by_email {}", result.error());
            return false;
        }
        return true;
    }

    auto UserServices::get_by_id(const std::string& id) -> std::optional<type::UserSchema> {
        const auto result = models::UserModel::get_by_id(id);
        if (not result.has_value()) {
            service::Logger::get_instance().get_logger()->error("UserServices::get_by_id {}", result.error());
            throw std::runtime_error(result.error().data());
        }
        return result.value();
    }

    auto UserServices::get_by_email(const std::string& email) -> std::optional<type::UserSchema> {
        const auto result = models::UserModel::get_by_email(email);
        if (not result.has_value()) {
            service::Logger::get_instance().get_logger()->error("UserServices::get_by_email {}", result.error());
            throw std::runtime_error(result.error().data());
        }
        return result.value();
    }

    auto UserServices::check_token(const std::string& token) -> type::result<bool> {
        try {
            if (token.empty()) {
                return std::unexpected("token is empty");
            }

            const auto& current_time  = fluffy_utils::Date::get_current_timestamp_32();
            const auto& max_token_sec = drogon::app().getCustomConfig()["max_token_sec"].asInt();
            const auto& random_string = drogon::app().getCustomConfig()["random_string"].asString();

            const auto& parse_jwt_opt = fluffy_utils::StringEncryption::parse_jwt(token, random_string);
            if (not parse_jwt_opt.has_value()) {
                return std::unexpected<std::string>("parse_jwt_opt failed!");
            }

            const auto& parse_jwt        = parse_jwt_opt.value();
            const auto [header, payload] = parse_jwt;

            schema::JwtBody jwt_body;
            jwt_body.header.typ = header.at(type::basic_value::jwt::typ).get<std::string>();
            jwt_body.header.alg = header.at(type::basic_value::jwt::alg).get<std::string>();

            if (jwt_body.header.typ != "JWT" || jwt_body.header.alg != "HS256") {
                return std::unexpected("header.typ or header.alg is not JWT or HS256");
            }

            jwt_body.payload.iss     = payload.at(type::basic_value::jwt::iss).get<std::string>();
            jwt_body.payload.sub     = payload.at(type::basic_value::jwt::sub).get<std::string>();
            jwt_body.payload.aud     = payload.at(type::basic_value::jwt::aud).get<std::string>();
            jwt_body.payload.exp     = payload.at(type::basic_value::jwt::exp).get<int>();
            jwt_body.payload.iat     = payload.at(type::basic_value::jwt::iat).get<int32_t>();
            jwt_body.payload.user_id = payload.at(type::UserSchema::key_id).get<std::string>();

            if (current_time - jwt_body.payload.iat > max_token_sec) {
                return std::unexpected("token expired");
            }

            if (jwt_body.payload.iss != "fluffy_nest::backend" || jwt_body.payload.sub != "login") {
                return std::unexpected("iss or sub is not fluffy_nest or login");
            }

            if (jwt_body.payload.aud != "fluffy_nest::client") {
                return std::unexpected("aud is not fluffy_nest::client");
            }

            const auto& id_exist = service::UserServices::id_exist(jwt_body.payload.user_id);
            if (not id_exist.has_value()) {
                return std::unexpected("id_exist failed!");
            }

            if (not id_exist.value()) {
                return std::unexpected("id_exist is false");
            }
            return true;
        }
        catch (const std::exception& e) {
            service::Logger::get_instance().get_logger()->error("UserServices::check_token {}", e.what());
            return std::unexpected(e.what());
        }
    };
}  // namespace service
