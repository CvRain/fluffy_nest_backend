#pragma once

#include <drogon/HttpController.h>
#include <drogon/IntranetIpFilter.h>

#include "filters/filter_keywords.h"
#include "services/logger.hpp"

using namespace drogon;

namespace api {
    class Article final : public drogon::HttpController<Article> {
    public:
        METHOD_LIST_BEGIN

        METHOD_ADD(Article::create_directory,
                   "/directory/create",
                   Put,
                   filter::keywords::enable_cores,
                   filter::keywords::user_auth);

        METHOD_ADD(Article::recursive_directory,
                   "/directory/recursive",
                   Get,
                   filter::keywords::allow_intranet_access,
                   filter::keywords::enable_cores);

        METHOD_ADD(Article::personal_directory,
                   "/directory/personal",
                   Get,
                   filter::keywords::enable_cores,
                   filter::keywords::user_auth);

        METHOD_LIST_END

    private:
        static void create_directory(const HttpRequestPtr                          &req,
                                     std::function<void(const HttpResponsePtr &)> &&callback);

        static void recursive_directory(const HttpRequestPtr                          &req,
                                        std::function<void(const HttpResponsePtr &)> &&callback);

        static void personal_directory(const HttpRequestPtr                          &req,
                                       std::function<void(const HttpResponsePtr &)> &&callback);

        static void append_object(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void remove_object(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    };
}  // namespace api
