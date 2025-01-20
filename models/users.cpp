//
// Created by cvrain on 25-1-19.
//

#include "users.hpp"
#include "services/logger.hpp"

#include <fmt/format.h>

models::UserModel::UserModel() {
    service::Logger::get_instance().get_logger()->info("UserModel::UserModel");
}

auto models::UserModel::size() const -> type::result<size_t> {
    const auto db_client = drogon::app().getDbClient();
    std::promise<type::result<size_t>> promise;
    auto callback = [&](const drogon::orm::Result &result) {
        const auto size = result.at(0).at("count").as<size_t>();
        promise.set_value(size);
    };
    auto exceptionCallback = [&](const drogon::orm::DrogonDbException &e) {
        const auto error_str = fmt::format("UserModel::size: exception: {}", e.base().what());
        service::Logger::get_instance().get_logger()->error("UserModel::size exception: {}", e.base().what());
        promise.set_value(std::unexpected<std::string_view>(error_str));
    };
    db_client->execSqlAsync("select count(*) from users", callback, exceptionCallback);
    return promise.get_future().get();
}
