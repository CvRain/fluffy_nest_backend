#pragma once

#include <drogon/HttpController.h>

#include "filters/filter_keywords.h"
#include "utils/drogon_specialization.hpp"

using namespace drogon;

namespace api {
    class User final : public drogon::HttpController<User> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(User::size, "/size", Get);
        METHOD_ADD(User::append, "/append", Post);
        METHOD_ADD(User::remove_by_id, "/remove/id", Delete);
        METHOD_ADD(User::remove_by_email, "/remove/email", Delete);
        METHOD_ADD(User::get_by_id, "/get/id", filter::keywords::user_id_exist, Get);
        METHOD_ADD(User::get_by_email, "/get/email", filter::keywords::user_email_exist, Get);
        METHOD_ADD(User::login, "/login", Post, filter::keywords::enable_cores, filter::keywords::user_email_exist);
        METHOD_ADD(User::token_login, "/login/token", Get, filter::keywords::enable_cores);
        METHOD_LIST_END

    private:
        static void size(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void append(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void remove_by_id(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void remove_by_email(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void get_by_id(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void get_by_email(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void token_login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void enable_register(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    };
}  // namespace api
