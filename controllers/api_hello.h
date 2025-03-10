#pragma once

#include <drogon/HttpController.h>

#include "filters/filter_keywords.h"
#include "utils/drogon_specialization.hpp"

using namespace drogon;

namespace api {
    class Hello final : public HttpController<Hello> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(Hello::index, "/index", Options, Get, filter::keywords::enable_cores_v2);
        METHOD_ADD(Hello::print_arg, "/print_arg", Options, Get, filter::keywords::enable_cores_v2);

        METHOD_LIST_END

        static void index(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void print_arg(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    };
}  // namespace api
