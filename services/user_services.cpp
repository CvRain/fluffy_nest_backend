//
// Created by cvrain on 25-1-20.
//

#include "user_services.hpp"
#include "services/logger.hpp"

namespace service {
    size_t UserServices::size() const {
        const auto result = user_model->size();
        if (not result.has_value()) {
            service::Logger::get_instance().get_logger()->error("UserServices::size {}", result.error());
        }
        return result.value_or(0);
    }

}