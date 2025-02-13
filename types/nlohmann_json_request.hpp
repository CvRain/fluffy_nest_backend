//
// Created by cvrain on 24-9-1.
//

#ifndef STORAGE_DELIGHT_NLOHMANN_JSON_REQUEST_HPP
#define STORAGE_DELIGHT_NLOHMANN_JSON_REQUEST_HPP

#include <nlohmann/json.hpp>
#include <drogon/HttpRequest.h>

namespace type {
    class NlohmannJsonRequest {
    public:
        explicit NlohmannJsonRequest(const drogon::HttpRequest &request)
                : m_request(request) {
        }

        const drogon::HttpRequest &operator->() const {
            return m_request;
        }

        [[nodiscard]] const drogon::HttpRequest &getRequest() const {
            return m_request;
        }

        [[nodiscard]] nlohmann::json getNlohmannJsonBody() const {
            return nlohmann::json::parse(m_request.getBody());
        }

    private:
        const drogon::HttpRequest &m_request;
    };

    using NlohmannJsonRequestPtr = std::shared_ptr<NlohmannJsonRequest>;
}
#endif //STORAGE_DELIGHT_NLOHMANN_JSON_REQUEST_HPP
