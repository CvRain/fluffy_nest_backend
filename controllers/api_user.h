#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api {
    class User final : public drogon::HttpController<User> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(User::size, "/size", Options, Get);
        METHOD_LIST_END

    private:
        static void size(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback);
    };
}
