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

auto models::UserModel::append(const type::UserSchema &user) -> type::result<bool> {
    const auto db_client = drogon::app().getDbClient();
    std::promise<type::result<bool>> promise;

    auto callback = [&](const drogon::orm::Result &result) {
        if (result.affectedRows() <= 0) {
            promise.set_value(false);
            return;
        }
        promise.set_value(true);
        service::Logger::get_instance().get_logger()->info("UserModel::append: user: {}-{}", user.name, user.email);
    };

    auto exceptionCallback = [&](const drogon::orm::DrogonDbException &e) {
        const auto error_str = fmt::format("UserModel::append: exception: {}", e.base().what());
        service::Logger::get_instance().get_logger()->error("UserModel::append exception: {}", e.base().what());
        promise.set_value(std::unexpected<std::string_view>(error_str));
    };

    const auto transaction = db_client->newTransaction();
    transaction->execSqlAsync(
        "INSERT INTO public.users (id, role, name, password, email, icon, signature, insert_time, update_time) VALUES ('$1', $2, '$3', '$4', '$5', '$6', '$7', $8, $9)",
        callback, exceptionCallback,
        user.id, user.role, user.name, user.password, user.email, user.icon, user.signature, user.create_time,
        user.update_time);

    return promise.get_future().get();
}
