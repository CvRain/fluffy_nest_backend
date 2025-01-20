#include "api_user.h"

#include <services/user_services.hpp>

using namespace api;

void User::size(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    const auto user_size = service::UserServices::get_instance().size();
    Json::Value response;
    response["size"] = user_size;
    callback(HttpResponse::newHttpJsonResponse(response));
}
