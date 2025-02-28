/**
 *
 *  filter_UserIdExist.cc
 *
 */

#include "filter_UserIdExist.h"

#include "services/logger.hpp"
#include "services/user_services.hpp"

using namespace drogon;

namespace filter {
    void UserIdExist::invoke(const HttpRequestPtr& req, MiddlewareNextCallback&& nextCb, MiddlewareCallback&& mcb) {
        service::Logger::get_instance().get_logger()->debug("UserIdExist::invoke");

        try {
            const auto request_body = fromRequest<nlohmann::json>(*req);

            if (const auto& id = request_body.at(type::UserSchema::key_user_id).get<std::string>();
                not service::UserServices::id_exist(id).value())
            {
                type::BasicResponse basic_response{.code    = k400BadRequest,
                                                   .message = "User::get_by_id k400BadRequest",
                                                   .result  = "id not exist",
                                                   .data    = {}};
                service::Logger::get_instance().get_logger()->debug("User::get_by_id k400BadRequest id not exist");
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
