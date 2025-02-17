#pragma once

#include <drogon/HttpController.h>

#include "filters/filter_keywords.h"

using namespace drogon;

namespace api {
    class Check final: public drogon::HttpController<Check> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(Check::enable_register, "/enable_register", Get, filter::keywords::enable_cores);
        METHOD_LIST_END
    private:
        static void enable_register(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    };
}  // namespace api
