//
// Created by cvrain on 25-1-20.
//

#include "user_services.hpp"

#include "services/logger.hpp"
#include "utils/date.h"

namespace service {
    size_t UserServices::size() const {
        const auto result = models::UserModel::size();
        if (not result.has_value()) {
            service::Logger::get_instance().get_logger()->error("UserServices::size {}", result.error());
        }
        return result.value_or(0);
    }

    auto UserServices::append(const type::UserSchema& user) const -> type::result<std::string> {
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

    auto UserServices::user_exist(const std::string& id, const std::string& email) const -> type::result<bool> {
        const auto result = models::UserModel::has_id(id) and models::UserModel::has_email(email);
        return result;
    }

    auto UserServices::id_exist(const std::string& id) const -> std::optional<bool> {
        const auto result = models::UserModel::has_id(id);
        if (not result.has_value()) {
            service::Logger::get_instance().get_logger()->error("UserServices::id_exist {}", result.error());
            throw std::runtime_error(result.error().data());
        }
        return result.value();
    }

    auto UserServices::email_exist(const std::string& email) const -> std::optional<bool> {
        const auto result = models::UserModel::has_email(email);
        if (not result.has_value()) {
            service::Logger::get_instance().get_logger()->error("UserServices::email_exist {}", result.error());
            throw std::runtime_error(result.error().data());
        }
        return result.value();
    }

    auto UserServices::remove_by_id(const std::string& id) const -> std::optional<bool> {
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

    auto UserServices::remove_by_email(const std::string& email) const -> std::optional<bool> {
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
    };
}  // namespace service
