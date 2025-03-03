//
// Created by cvrain on 25-3-3.
//

#include "user_validator_middleware.hpp"

namespace filter {

    void UserIdExistMiddleware::invoke(const HttpRequestPtr&    req,
                                       MiddlewareNextCallback&& nextCb,
                                       MiddlewareCallback&&     mcb) {}

    void UserEmailExistMiddleware::invoke(const HttpRequestPtr&    req,
                                          MiddlewareNextCallback&& nextCb,
                                          MiddlewareCallback&&     mcb) {
        // todo
        nextCb([&, mcb = std::move(mcb)](const HttpResponsePtr& resp) { mcb(resp); });
    }

    void UserNameExistMiddleware::invoke(const HttpRequestPtr&    req,
                                         MiddlewareNextCallback&& nextCb,
                                         MiddlewareCallback&&     mcb) {
        // todo
        nextCb([&, mcb = std::move(mcb)](const HttpResponsePtr& resp) { mcb(resp); });
    }
}  // namespace filter
