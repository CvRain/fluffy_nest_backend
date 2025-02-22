#pragma once

#include <drogon/HttpController.h>

#include "filters/filter_keywords.h"
#include "utils/drogon_specialization.hpp"

using namespace drogon;

namespace api {
    class User final : public drogon::HttpController<User> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(User::size, "/size", Get, filter::keywords::enable_cores);
        METHOD_ADD(User::append, "/append", Post, filter::keywords::enable_cores);
        METHOD_ADD(User::remove_by_id, "/remove/id", Delete, filter::keywords::enable_cores);
        METHOD_ADD(User::remove_by_email, "/remove/email", Delete, filter::keywords::enable_cores);
        METHOD_ADD(User::login, "/login", Post, filter::keywords::enable_cores, filter::keywords::user_email_exist);
        METHOD_ADD(User::token_login, "/login/token", Get, filter::keywords::enable_cores);
        METHOD_ADD(User::name_exist, "/exist/name", Get, filter::keywords::enable_cores);
        METHOD_ADD(User::get_by_id, "/one/id", Get, filter::keywords::enable_cores, filter::keywords::user_id_exist);
        METHOD_ADD(User::get_by_email,"/one/email",Get, filter::keywords::enable_cores, filter::keywords::user_email_exist);

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
        static void name_exist(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    };
}  // namespace api
