#pragma once

#include <drogon/HttpController.h>

#include "filters/filter_keywords.h"

using namespace drogon;

namespace api {
    class Check final : public drogon::HttpController<Check> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(Check::enable_register, "/enable_register", Get, filter::keywords::enable_cores_v2);
        METHOD_ADD(Check::name_exist, "/exist/name", Get, filter::keywords::enable_cores_v2);
        METHOD_ADD(Check::email_exist, "/exist/email", Get, filter::keywords::enable_cores_v2);
        METHOD_ADD(Check::id_exist, "/exist/id", Get, filter::keywords::enable_cores_v2);
        METHOD_LIST_END
    private:
        static void enable_register(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void name_exist(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void email_exist(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void id_exist(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    };
}  // namespace api
