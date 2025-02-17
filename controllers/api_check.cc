#include "api_check.h"

#include "services/logger.hpp"
#include "types/nlohmann_json_response.hpp"
#include "utils/drogon_specialization.hpp"
#include "utils/exception_handler.hpp"

using namespace api;


void Check::enable_register(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service::Logger::get_instance().get_logger()->debug("User::enable_register");
    try {
        if (app().getCustomConfig()["enable_add_user"].asBool()) {
            type::BasicResponse basic_response{.code    = k200OK,
                                               .message = "User::enable_register k200OK",
                                               .result  = "server enable add user",
                                               .data    = ""};
            callback(newHttpJsonResponse(basic_response.to_json()));
        }
        else {
            type::BasicResponse basic_response{.code    = k403Forbidden,
                                               .message = "User::enable_register k403Forbidden",
                                               .result  = "server disable add user",
                                               .data    = ""};
            callback(newHttpJsonResponse(basic_response.to_json()));
        }
    }
    catch (const std::exception &e) {
        exception::ExceptionHandler::handle(req, std::move(callback), e);
    }
}
