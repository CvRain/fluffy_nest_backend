//
// Created by cvrain on 24-9-3.
//

#ifndef STORAGE_DELIGHT_NLOHMANN_JSON_RESPONSE_HPP
#define STORAGE_DELIGHT_NLOHMANN_JSON_RESPONSE_HPP

#include <drogon/HttpResponse.h>
#include <nlohmann/json.hpp>

#include "basic_value.hpp"

namespace type {
    struct TestResponse {
        int code;
        std::string message;
    };

    class BasicResponse {
      public:

        int code;
        std::string_view message;
        std::string_view result;
        nlohmann::json data{};

        nlohmann::json to_json() {
            return nlohmann::json{
                {basic_value::request::code, code},
                {basic_value::request::message, message},
                {basic_value::request::result, result},
                {basic_value::request::data, data}
            };
        }
    };


    class NlohmannResponse {
    public:
        static drogon::HttpResponsePtr new_nlohmann_json_response(nlohmann::json&&json) {
            return drogon::HttpResponse::newCustomHttpResponse<nlohmann::json>(std::move(json));
        }
    };
}
#endif //STORAGE_DELIGHT_NLOHMANN_JSON_RESPONSE_HPP
