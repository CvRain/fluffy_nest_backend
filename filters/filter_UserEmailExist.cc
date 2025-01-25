/**
 *
 *  filter_UserEmailExist.cc
 *
 */

#include "filter_UserEmailExist.h"

#include "services/logger.hpp"
#include "services/user_services.hpp"

using namespace drogon;

namespace filter {
    void UserEmailExist::invoke(const HttpRequestPtr& req, MiddlewareNextCallback&& nextCb, MiddlewareCallback&& mcb) {
        service::Logger::get_instance().get_logger()->debug("UserEmailExist::invoke");

        try {
            const auto  request_body = fromRequest<nlohmann::json>(*req);

            if (const auto& email = request_body.at("email").get<std::string>();
                not service::UserServices::get_instance().email_exist(email).value()) {
                type::BasicResponse basic_response{.code    = k400BadRequest,
                                                   .message = "User::get_by_email k400BadRequest",
                                                   .result  = "email not exist",
                                                   .data    = {}};
                service::Logger::get_instance().get_logger()->debug("User::get_by_email k400BadRequest email not exist");
                mcb(newHttpJsonResponse(basic_response.to_json()));
                return;
            }

            nextCb([&, mcb = std::move(mcb)](const HttpResponsePtr& resp) { mcb(resp); });
        }
        catch (const std::exception& e) {
            service::Logger::get_instance().get_logger()->error("UserIdExist::invoke: {}", e.what());
            mcb(drogon::HttpResponse::newNotFoundResponse());
        }
    }
}  // namespace filter
