#include "api_hello.h"

#include "services/logger.hpp"
#include "utils/exception_handler.hpp"

using namespace api;

// Add definition of your processing function here
void Hello::index(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service::Logger::get_instance().get_logger()->debug("Hello::index");

    try {
        const auto &currentTimeStamp = std::chrono::system_clock::now();
        const auto &timeStamp =
                std::chrono::duration_cast<std::chrono::seconds>(currentTimeStamp.time_since_epoch()).count();

        const auto data = nlohmann::json{{"timestamp", timeStamp}};
        type::BasicResponse response{.code = k200OK, .message = "k200OK", .result = "", .data = data};

        service::Logger::get_instance().get_logger()->debug("Hello::index {}", response.to_json().dump());

        callback(newHttpJsonResponse(response.to_json()));
    }catch (const std::exception &e) {
        exception::ExceptionHandler::handle(req, std::move(callback), e);
    }
}
