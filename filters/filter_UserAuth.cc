/**
 *
 *  filter_UserAuth.cc
 *
 */

#include "filter_UserAuth.h"

#include "services/logger.hpp"
#include "services/user_services.hpp"
#include "types/nlohmann_json_response.hpp"
#include "types/type.hpp"
#include "utils/date.h"
#include "utils/drogon_specialization.hpp"
#include "utils/exception_handler.hpp"
#include "utils/string.hpp"

using namespace drogon;

namespace filter {
    void UserAuth::invoke(const HttpRequestPtr& req, MiddlewareNextCallback&& nextCb, MiddlewareCallback&& mcb) {
        service::Logger::get_instance().get_logger()->debug("UserAuth::invoke");

        try {
            const auto& token = req->getHeader("Authorization");

            if (const auto result = service::UserServices::check_token(token);
                not result.has_value() || not result.value())
            {
                throw exception::BaseException{{.code    = k401Unauthorized,
                                                .message = "k401Unauthorized",
                                                .result  = result.error(),
                                                .data    = {}}};
            }

            nextCb([&, mcb = std::move(mcb)](const HttpResponsePtr& resp) { mcb(resp); });
        }
        catch (const std::exception& e) {
            exception::ExceptionHandler::handle(req, std::move(mcb), e);
        }
    }

}  // namespace filter
