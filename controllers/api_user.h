#pragma once

#include <drogon/HttpController.h>

#include "utils/drogon_specialization.hpp"

using namespace drogon;

namespace api {
    class User final : public drogon::HttpController<User> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(User::size, "/size", Options, Get);
        METHOD_ADD(User::append, "/append", Options, Post);
        METHOD_ADD(User::remove_by_id, "/remove/id", Options, Delete);
        METHOD_ADD(User::remove_by_email, "/remove/email", Options, Delete);
        METHOD_LIST_END

    private:
        static void size(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void append(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void remove_by_id(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void remove_by_email(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    };
}  // namespace api
