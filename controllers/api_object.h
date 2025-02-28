#pragma once

#include <drogon/HttpController.h>

#include "filters/filter_keywords.h"

using namespace drogon;

namespace api {
    class Object final : public drogon::HttpController<Object> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(Object::list_objects,
                   "/list",
                   Get,
                   filter::keywords::enable_cores,
                   filter::keywords::user_auth,
                   filter::keywords::user_id_exist);

        METHOD_ADD(Object::tree_objects,
                   "/tree",
                   Get,
                   filter::keywords::enable_cores,
                   filter::keywords::user_auth,
                   filter::keywords::user_id_exist);

        METHOD_LIST_END
        static void list_objects(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void tree_objects(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    };
}  // namespace api
