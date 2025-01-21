//
// Created by cvrain on 25-1-20.
//

#include "user_services.hpp"
#include "utils/date.h"
#include "services/logger.hpp"

namespace service {
    size_t UserServices::size() const {
        const auto result = user_model->size();
        if (not result.has_value()) {
            service::Logger::get_instance().get_logger()->error("UserServices::size {}", result.error());
        }
        return result.value_or(0);
    }

    auto UserServices::append(const type::UserSchema &user) const -> type::result<std::string_view> {
        const auto will_insert_user = type::UserSchema{
            .id = drogon::utils::getUuid(),
            .name = user.name,
            .password = user.password,
            .email = user.email,
            .role = 0,
            .icon = "",
            .signature = "",
            .create_time = utils::Date::get_current_timestamp_32(),
            .update_time = utils::Date::get_current_timestamp_32(),
        };
        const auto insert_result = user_model->append(will_insert_user);
        if (not insert_result.has_value()) {
            service::Logger::get_instance().get_logger()->error("UserServices::append {}", insert_result.error());
            return insert_result.error();
        }
        return insert_result.value() ? std::string_view{will_insert_user.id} : std::string_view{};
    };
}

