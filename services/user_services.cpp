//
// Created by cvrain on 25-1-20.
//

#include "user_services.hpp"

#include "services/logger.hpp"
#include "utils/date.h"

namespace service {
    size_t UserServices::size() const {
        const auto result = user_model->size();
        if (not result.has_value()) {
            service::Logger::get_instance().get_logger()->error("UserServices::size {}", result.error());
        }
        return result.value_or(0);
    }

    auto UserServices::append(const type::UserSchema& user) const -> type::result<std::string> {
        const auto will_insert_user = type::UserSchema{.id          = drogon::utils::getUuid().data(),
                                                       .name        = user.name,
                                                       .password    = user.password,
                                                       .email       = user.email,
                                                       .role        = 0,
                                                       .icon        = "",
                                                       .signature   = "",
                                                       .create_time = trantor::Date::now(),
                                                       .update_time = trantor::Date::now()};
        const auto insert_result    = user_model->append(will_insert_user);
        if (not insert_result.has_value()) {
            service::Logger::get_instance().get_logger()->error("UserServices::append {}", insert_result.error());
            return insert_result.error().data();
        }
        return insert_result.value() ? will_insert_user.id : "";
    }

    auto UserServices::user_exist(const std::string& id, const std::string& email) const -> type::result<bool> {
        const auto result = user_model->has_id(id) and user_model->has_email(email);
        return result;
    }

    auto UserServices::id_exist(const std::string& id) const -> std::optional<bool> {
        const auto result =  user_model->has_id(id);
        if (not result.has_value()) {
            service::Logger::get_instance().get_logger()->error("UserServices::id_exist {}", result.error());
            throw std::runtime_error(result.error().data());
        }
        return result.value();
    }

    auto UserServices::email_exist(const std::string& email) const -> std::optional<bool> {
        const auto result =  user_model->has_email(email);
        if (not result.has_value()) {
            service::Logger::get_instance().get_logger()->error("UserServices::email_exist {}", result.error());
            throw std::runtime_error(result.error().data());
        }
        return result.value();
    };
}  // namespace service
