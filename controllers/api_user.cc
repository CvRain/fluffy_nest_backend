#include "api_user.h"

#include "services/logger.hpp"
#include "services/object_storage_service.hpp"
#include "services/user_services.hpp"
#include "types/nlohmann_json_response.hpp"
#include "types/type.hpp"
#include "utils/date.h"
#include "utils/exception_handler.hpp"
#include "utils/string.hpp"

using namespace api;

void User::size(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    const auto user_size = service::UserServices::get_instance().size();

    const nlohmann::json data{{"size", user_size}};

    type::BasicResponse basic_response{.code = k200OK, .message = "User::size k200OK", .result = "", .data = data};
    callback(newHttpJsonResponse(basic_response.to_json()));
}

void User::append(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    try {
        if (app().getCustomConfig()["enable_add_user"] == false) {
            type::BasicResponse basic_response{.code    = k400BadRequest,
                                               .message = "User::append k400BadRequest",
                                               .result  = "add user is disabled",
                                               .data    = {}};
            service::Logger::get_instance().get_logger()->warn("User::append k400BadRequest add user is disabled");
            callback(newHttpJsonResponse(basic_response.to_json()));
            return;
        }

        const auto request_body = fromRequest<nlohmann::json>(*req);

        type::UserSchema user{};
        user.name     = request_body.at(type::UserSchema::key_name).get<std::string>();
        user.password = drogon::utils::getSha256(request_body.at(type::UserSchema::key_password).get<std::string>());
        user.email    = request_body.at(type::UserSchema::key_email).get<std::string>();

        if (service::UserServices::get_instance().email_exist(user.email).value()) {
            type::BasicResponse basic_response{.code    = k400BadRequest,
                                               .message = "User::append k400BadRequest",
                                               .result  = "email exist",
                                               .data    = {}};
            service::Logger::get_instance().get_logger()->warn("User::append k400BadRequest email exist");
            callback(newHttpJsonResponse(basic_response.to_json()));
            return;
        }

        if (service::UserServices::get_instance().name_exist(user.name).value()) {
            type::BasicResponse basic_response{.code    = k400BadRequest,
                                               .message = "User::append k400BadRequest",
                                               .result  = "name exist",
                                               .data    = {}};
            service::Logger::get_instance().get_logger()->warn("User::append k400BadRequest name exist");
            callback(newHttpJsonResponse(basic_response.to_json()));
            return;
        }

        const auto append_result = service::UserServices::get_instance().append(user);
        if (not append_result.has_value()) {
            type::BasicResponse basic_response{
                    .code = k400BadRequest, .message = "User::append k400BadRequest", .result = "", .data = {}};
            throw exception::BaseException{basic_response};
        }

        const auto &uuid  = append_result.value();
        const auto  is_ok = not uuid.empty();

        const auto data = nlohmann::json{
                {"is_ok", is_ok},
                {"uuid", uuid},
        };

        // 用户在数据库创建成功后将会同步在云存储中创建一个uuid的文件夹以及index.md
        const auto file_name  = uuid + "/index.md";
        const auto source     = "*Hello world!";
        const auto put_result = service::ObjectStorageService::get_instance().put_object(file_name, source);

        if (not put_result.has_value()) {
            exception::BaseException exception{{.code    = k500InternalServerError,
                                                .message = "User::append k500InternalServerError",
                                                .result  = put_result.error(),
                                                .data    = {}}};
            throw std::move(exception);
        }

        auto response = type::BasicResponse{
                .code = k200OK, .message = "User::append k200OK", .result = put_result.value(), .data = data};

        callback(newHttpJsonResponse(response.to_json()));
    }
    catch (const std::exception &e) {
        exception::ExceptionHandler::handle(req, std::move(callback), e);
    }
}
void User::remove_by_id(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service::Logger::get_instance().get_logger()->debug("User::remove_by_id");
    try {
        if (const auto enable_remove_config = app().getCustomConfig()["enable_remove_user"].as<std::string>();
            enable_remove_config == "false")
        {
            type::BasicResponse basic_response{.code    = k400BadRequest,
                                               .message = "User::remove_by_id k400BadRequest",
                                               .result  = "remove user is disabled",
                                               .data    = {}};
            service::Logger::get_instance().get_logger()->warn(
                    "User::remove_by_id k400BadRequest remove user is disabled");
            callback(newHttpJsonResponse(basic_response.to_json()));
            return;
        }

        const auto  request_body = fromRequest<nlohmann::json>(*req);
        const auto &id           = request_body.at(type::UserSchema::key_id).get<std::string>();

        if (not service::UserServices::get_instance().id_exist(id).value()) {
            type::BasicResponse basic_response{.code    = k400BadRequest,
                                               .message = "User::remove_by_id k400BadRequest",
                                               .result  = "id not exist",
                                               .data    = {}};
            service::Logger::get_instance().get_logger()->warn("User::remove_by_id k400BadRequest id not exist");
            callback(newHttpJsonResponse(basic_response.to_json()));
            return;
        }
        if (not service::UserServices::get_instance().remove_by_id(id).value()) {
            type::BasicResponse basic_response{.code    = k400BadRequest,
                                               .message = "User::remove_by_id k400BadRequest",
                                               .result  = "failed to remove user",
                                               .data    = {}};
            service::Logger::get_instance().get_logger()->warn("User::remove_by_id k400BadRequest");
            callback(newHttpJsonResponse(basic_response.to_json()));
            return;
        }

        type::BasicResponse basic_response{
                .code = k200OK, .message = "User::remove_by_id k200OK", .result = "", .data = {}};
        callback(newHttpJsonResponse(basic_response.to_json()));
    }
    catch (const std::exception &e) {
        exception::ExceptionHandler::handle(req, std::move(callback), e);
    }
}

void User::remove_by_email(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service::Logger::get_instance().get_logger()->debug("User::remove_by_email");
    try {
        if (const auto enable_remove_config = app().getCustomConfig()["enable_remove_user"].as<std::string>();
            enable_remove_config == "false")
        {
            type::BasicResponse basic_response{.code    = k400BadRequest,
                                               .message = "User::remove_by_id k400BadRequest",
                                               .result  = "remove user is disabled",
                                               .data    = {}};
            service::Logger::get_instance().get_logger()->warn(
                    "User::remove_by_email k400BadRequest remove user is disabled");
            callback(newHttpJsonResponse(basic_response.to_json()));
            return;
        }

        const auto  request_body = fromRequest<nlohmann::json>(*req);
        const auto &email        = request_body.at(type::UserSchema::key_email).get<std::string>();

        if (not service::UserServices::email_exist(email).value()) {
            type::BasicResponse basic_response{.code    = k400BadRequest,
                                               .message = "User::remove_by_email k400BadRequest",
                                               .result  = "email not exist",
                                               .data    = {}};
            service::Logger::get_instance().get_logger()->warn("User::remove_by_email k400BadRequest email not exist");
            callback(newHttpJsonResponse(basic_response.to_json()));
            return;
        }
        if (not service::UserServices::remove_by_email(email).value()) {
            type::BasicResponse basic_response{.code    = k400BadRequest,
                                               .message = "User::remove_by_email k400BadRequest",
                                               .result  = "failed to remove user",
                                               .data    = {}};
            service::Logger::get_instance().get_logger()->warn("User::remove_by_email k400BadRequest");
            callback(newHttpJsonResponse(basic_response.to_json()));
            return;
        }
        type::BasicResponse basic_response{
                .code = k200OK, .message = "User::remove_by_email k200OK", .result = "", .data = {}};
        callback(newHttpJsonResponse(basic_response.to_json()));
    }
    catch (const std::exception &e) {
        exception::ExceptionHandler::handle(req, std::move(callback), e);
    }
}
void User::get_by_id(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service::Logger::get_instance().get_logger()->debug("User::get_by_id");
    try {
        const auto& id = req->getParameter(type::UserSchema::key_user_id.data());

        if (const auto user_exist_result = service::UserServices::id_exist(id); not user_exist_result.has_value()) {
            type::BasicResponse basic_response{.code    = k400BadRequest,
                                               .message = "User::get_by_id k400BadRequest",
                                               .result  = "id not exist",
                                               .data    = {}};
            service::Logger::warn("User::get_by_id k400BadRequest id not exist");
            callback(newHttpJsonResponse(basic_response.to_json()));
            return;
        }

        const auto get_user_result = service::UserServices::get_by_id(id);
        if (not get_user_result.has_value()) {
            type::BasicResponse basic_response{.code    = k400BadRequest,
                                               .message = "User::get_by_id k400BadRequest",
                                               .result  = "id not exist",
                                               .data    = {}};
            service::Logger::warn("User::get_by_id k400BadRequest id not exist");
            callback(newHttpJsonResponse(basic_response.to_json()));
            return;
        }
        const auto& user = get_user_result.value();
        const auto data = nlohmann::json{
                {type::UserSchema::key_id, user.id},
                {type::UserSchema::key_email, user.email},
                {type::UserSchema::key_id, user.id},
                {type::UserSchema::key_create_time, user.create_time.secondsSinceEpoch()},
                {type::UserSchema::key_update_time, user.update_time.secondsSinceEpoch()},
                {type::UserSchema::key_role, user.role},
                {type::UserSchema::key_signature, user.signature},
                {type::UserSchema::key_icon, user.icon},
        };
        type::BasicResponse basic_response{
                .code = k200OK, .message = "User::get_by_id k200OK", .result = "", .data = data};
        callback(newHttpJsonResponse(basic_response.to_json()));
    }
    catch (const std::exception &e) {
        exception::ExceptionHandler::handle(req, std::move(callback), e);
    }
}

void User::get_by_email(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service::Logger::get_instance().get_logger()->debug("User::get_by_email");

    try {
        const auto& email = req->getParameter(type::UserSchema::key_email.data());

        const auto user = service::UserServices::get_by_email(email).value();
        const auto data = nlohmann::json{
                {type::UserSchema::key_id, user.id},
                {type::UserSchema::key_email, user.email},
                {type::UserSchema::key_id, user.id},
                {type::UserSchema::key_create_time, user.create_time.secondsSinceEpoch()},
                {type::UserSchema::key_update_time, user.update_time.secondsSinceEpoch()},
                {type::UserSchema::key_role, user.role},
                {type::UserSchema::key_signature, user.signature},
                {type::UserSchema::key_icon, user.icon},
        };
        type::BasicResponse basic_response{
                .code = k200OK, .message = "User::get_by_email k200OK", .result = "", .data = data};
        callback(newHttpJsonResponse(basic_response.to_json()));
    }
    catch (const std::exception &e) {
        exception::ExceptionHandler::handle(req, std::move(callback), e);
    }
}
void User::login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service::Logger::get_instance().get_logger()->debug("User::login");
    try {
        const auto request_ip = req->getPeerAddr().toIpPort();
        service::Logger::get_instance().get_logger()->info("User::login request_ip: {}", request_ip);

        const auto  request_body = fromRequest<nlohmann::json>(*req);
        const auto &email        = request_body.at(type::UserSchema::key_email).get<std::string>();
        const auto &password =
                drogon::utils::getSha256(request_body.at(type::UserSchema::key_password).get<std::string>());
        const auto &user = service::UserServices::get_instance().get_by_email(email).value_or(type::UserSchema{});

        if (const auto saved_password = user.password; saved_password != password or saved_password.empty()) {
            type::BasicResponse basic_response{.code    = k400BadRequest,
                                               .message = "User::login k400BadRequest",
                                               .result  = "password is wrong",
                                               .data    = {}};
            service::Logger::get_instance().get_logger()->warn("User::login k400BadRequest password is wrong");
            service::Logger::get_instance().get_logger()->debug("User::login request password: {}", password);
            service::Logger::get_instance().get_logger()->debug("User::login saved password: {}", saved_password);
            callback(newHttpJsonResponse(basic_response.to_json()));
            return;
        }

        const auto header =
                nlohmann::json{{type::basic_value::jwt::alg, "HS256"}, {type::basic_value::jwt::typ, "JWT"}};

        const auto &current_time  = fluffy_utils::Date::get_current_timestamp_32();
        const auto &max_token_sec = drogon::app().getCustomConfig()["max_token_sec"].asInt();
        const auto &random_string = drogon::app().getCustomConfig()["random_string"].asString();

        const nlohmann::json payload{{type::basic_value::jwt::iss, "fluffy_nest::backend"},
                                     {type::basic_value::jwt::exp, current_time + max_token_sec},
                                     {type::basic_value::jwt::sub, "login"},
                                     {type::basic_value::jwt::iat, current_time},
                                     {type::basic_value::jwt::aud, "fluffy_nest::client"},
                                     {type::UserSchema::key_id, user.id}};

        const auto jwt = fluffy_utils::StringEncryption::generate_jwt(header.dump(), payload.dump(), random_string);

        service::Logger::get_instance().get_logger()->debug("User::login jwt: {}", jwt);

        const nlohmann::json response_data{
                {type::UserSchema::key_id, user.id}, {type::UserSchema::key_email, user.email}, {"token", jwt}};
        type::BasicResponse basic_response{
                .code = k200OK, .message = "User::login k200OK", .result = "", .data = response_data};
        callback(newHttpJsonResponse(basic_response.to_json()));
    }
    catch (const std::exception &e) {
        exception::ExceptionHandler::handle(req, std::move(callback), e);
    }
}

void User::token_login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service::Logger::get_instance().get_logger()->debug("User::token_login");
    try {
        const auto request_ip = req->getPeerAddr().toIpPort();
        service::Logger::get_instance().get_logger()->info("User::token_login request_ip: {}", request_ip);

        const auto token = req->getHeader("Authorization");
        service::Logger::get_instance().get_logger()->debug("User::token_login token: {}", token);

        const auto check_result = service::UserServices::check_token(token);
        if (not check_result.has_value()) {
            type::BasicResponse basic_response{.code    = k400BadRequest,
                                               .message = "User::token_login k400BadRequest",
                                               .result  = check_result.error(),
                                               .data    = {}};
            callback(newHttpJsonResponse(basic_response.to_json()));
            return;
        }

        if (not check_result.value()) {
            type::BasicResponse basic_response{.code    = k500InternalServerError,
                                               .message = "User::token_login k500InternalServerError",
                                               .result  = "unknown exception",
                                               .data    = {}};
            callback(newHttpJsonResponse(basic_response.to_json()));
            return;
        }
        type::BasicResponse basic_response{
                .code = k200OK, .message = "User::token_login k200OK", .result = "", .data = {}};
        callback(newHttpJsonResponse(basic_response.to_json()));
    }
    catch (const std::exception &e) {
        exception::ExceptionHandler::handle(req, std::move(callback), e);
    }
}


void User::name_exist(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service::Logger::get_instance().get_logger()->debug("User::name_exist");
    try {
        const auto  body = fromRequest<nlohmann::json>(*req);
        const auto &name = body.at(type::UserSchema::key_name).get<std::string>();

        if (const auto name_exist_result = service::UserServices::name_exist(name).value_or(false);
            not name_exist_result)
        {
            type::BasicResponse basic_response{
                    .code = k200OK, .message = "User::name_exist k200OK", .result = "name not exist", .data = {}};
            callback(newHttpJsonResponse(basic_response.to_json()));
            return;
        }
        type::BasicResponse basic_response{
                .code = k200OK, .message = "User::name_exist k200OK", .result = "name exist", .data = {}};
        callback(newHttpJsonResponse(basic_response.to_json()));
    }
    catch (const std::exception &e) {
        exception::ExceptionHandler::handle(req, std::move(callback), e);
    }
}
