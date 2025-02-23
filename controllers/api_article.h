#pragma once

#include <drogon/HttpController.h>

#include "filters/filter_keywords.h"
#include "services/logger.hpp"

using namespace drogon;

namespace api {
    class Article final : public drogon::HttpController<Article> {
    public:
        METHOD_LIST_BEGIN

        METHOD_ADD(Article::create_directory,
                   "/directory/create",
                   Post,
                   filter::keywords::enable_cores,
                   filter::keywords::user_auth,
                   filter::keywords::user_id_exist);

        METHOD_ADD(Article::recursive_directory,
                   "/directory/recursive",
                   Post,
                   filter::keywords::enable_cores,
                   filter::keywords::user_auth,
                   filter::keywords::user_id_exist);

        METHOD_LIST_END

    private:
        static void create_directory(const HttpRequestPtr                          &req,
                                     std::function<void(const HttpResponsePtr &)> &&callback);

        static void recursive_directory(const HttpRequestPtr                          &req,
                                        std::function<void(const HttpResponsePtr &)> &&callback);
    };
}  // namespace api
