#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api {
    class Hello final: public HttpController<Hello> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(Hello::index, "/index", Get);

        METHOD_LIST_END

        static void index(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback);
    };
}
