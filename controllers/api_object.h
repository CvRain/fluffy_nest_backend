#pragma once

#include <drogon/HttpController.h>

#include "filters/filter_keywords.h"

using namespace drogon;

namespace api {
    class Object final : public drogon::HttpController<Object> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(Object::list_objects, "/list", Options, Get, filter::keywords::user_auth);
        METHOD_ADD(Object::tree_objects, "/tree", Options, Get, filter::keywords::user_auth);
        METHOD_ADD(Object::exist_object, "/exist", Get, filter::keywords::user_auth);
        METHOD_ADD(Object::append_object, "/append", Post, filter::keywords::user_auth);
        METHOD_ADD(Object::remove_object, "/remove", Post, filter::keywords::user_auth);
        METHOD_ADD(Object::object_url, "/url", Get, filter::keywords::user_auth);
        METHOD_LIST_END

        static void list_objects(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void tree_objects(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void exist_object(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void append_object(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void remove_object(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void object_url(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    };
}  // namespace api
