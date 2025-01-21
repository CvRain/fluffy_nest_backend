#include "api_user.h"

#include "services/user_services.hpp"
#include "types/nlohmann_json_response.hpp"

using namespace api;

void User::size(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    const auto user_size = service::UserServices::get_instance().size();

    const nlohmann::json data{{"size", user_size}};

    type::BasicResponse basic_response{.code = k200OK, .message = "User::size k200OK", .result = "", .data = data};
    callback(newHttpJsonResponse(basic_response.to_json()));
}

void User::append(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    const auto &request_body = req->getJsonObject();
}
