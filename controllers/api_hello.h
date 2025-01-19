#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api {
    class hello final: public HttpController<hello> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(hello::index, "/index", Get);

        METHOD_LIST_END

        static void index(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback);
    };
}
