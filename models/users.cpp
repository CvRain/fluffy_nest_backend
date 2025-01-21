//
// Created by cvrain on 25-1-19.
//

#include "users.hpp"

#include <fmt/format.h>

#include "services/logger.hpp"
#include "types/type.hpp"

models::UserModel::UserModel() { service::Logger::get_instance().get_logger()->info("UserModel::UserModel"); }

auto models::UserModel::size() const -> type::result<size_t> {
    const auto                         db_client = drogon::app().getDbClient();
    std::promise<type::result<size_t>> promise;
    auto                               callback = [&](const drogon::orm::Result &result) {
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
        service::Logger::get_instance().get_logger()->info(
                "UserModel::append: user: {}-{}", user.name.data(), user.email.data());
    };

    auto exceptionCallback = [&](const drogon::orm::DrogonDbException &e) {
        const auto error_str = fmt::format("UserModel::append: exception: {}", e.base().what());
        service::Logger::get_instance().get_logger()->error("UserModel::append exception: {}", e.base().what());
        promise.set_value(std::unexpected<std::string_view>(error_str));
    };

    const auto transaction = db_client->newTransaction();
    transaction->execSqlAsync(
            "INSERT INTO public.users (id, role, name, password, email, icon, signature, create_time, update_time) "
            "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9)",
            callback,
            exceptionCallback,
            user.id,
            user.role,
            user.name,
            user.password,
            user.email,
            user.icon,
            user.signature,
            user.create_time,
            user.update_time);

    return promise.get_future().get();
}

auto models::UserModel::has_id(const std::string &id) const -> type::result<bool> {
    const auto db_client = drogon::app().getDbClient();

    std::promise<type::result<bool>> promise;

    auto callback = [&](const drogon::orm::Result &result) {
        if (result.at(0).at(type::UserSchema::key_id.data()).length() > 0) {
            promise.set_value(true);
            service::Logger::get_instance().get_logger()->info("UserModel::has_id: id: {}", id);
            return;
        }
        promise.set_value(false);
    };
    auto exceptionCallback = [&](const drogon::orm::DrogonDbException &e) {
        const auto error_str = fmt::format("UserModel::has_id: exception: {}", e.base().what());
        service::Logger::get_instance().get_logger()->error("UserModel::has_id exception: {}", e.base().what());
        promise.set_value(std::unexpected<std::string_view>(error_str));
    };

    db_client->execSqlAsync("select id from users where id = $1", callback, exceptionCallback, id);
    return promise.get_future().get();
}

auto models::UserModel::has_email(const std::string &email) const -> type::result<bool> {
    const auto db_client = drogon::app().getDbClient();

    std::promise<type::result<bool>> promise;

    auto callback = [&](const drogon::orm::Result &result) {
        if (result.at(0).at(type::UserSchema::key_email.data()).length() > 0) {
            promise.set_value(true);
            service::Logger::get_instance().get_logger()->info("UserModel::has_email: email: {}", email);
            return;
        }
        promise.set_value(false);
    };

    auto exceptionCallback = [&](const drogon::orm::DrogonDbException &e) {
        const auto error_str = fmt::format("UserModel::has_email: exception: {}", e.base().what());
        service::Logger::get_instance().get_logger()->error("UserModel::has_email exception: {}", e.base().what());
        promise.set_value(std::unexpected<std::string_view>(error_str));
    };

    db_client->execSqlAsync("select email from users where email = $1", callback, exceptionCallback, email);
    return promise.get_future().get();
}

//todo
auto models::UserModel::remove_by_id(const std::string &id) -> type::result<int> {
    return 0;
}

//todo
auto models::UserModel::remove_by_email(const std::string &email) -> type::result<int> {
    return 0;
}
