//
// Created by cvrain on 24-11-28.
//

#include "exception_handler.hpp"

#include "services/logger.hpp"
#include "types/nlohmann_json_request.hpp"
#include "types/type.hpp"
#include "utils/drogon_specialization.hpp"

namespace exception {
    void ExceptionHandler::handle(const drogon::HttpRequestPtr&                 request,
                                  std::function<void(const HttpResponsePtr&)>&& callback,
                                  const std::exception&                         exception) {
        if (const auto* nlohmann_exception = dynamic_cast<const nlohmann::detail::exception*>(&exception)) {
            ExceptionHandler::nlohmann_exception(request, std::move(callback), exception);
            return;
        }
        if (const auto* base_exception = dynamic_cast<const BaseException*>(&exception)) {
            ExceptionHandler::base_exception(request, std::move(callback), exception);
            return;
        }
        ExceptionHandler::standard_exception(request, std::move(callback), exception);
    }

    void ExceptionHandler::nlohmann_exception(const drogon::HttpRequestPtr&                 request,
                                              std::function<void(const HttpResponsePtr&)>&& callback,
                                              const std::exception&                         exception) {
        type::BasicResponse response{
                .code = drogon::k400BadRequest, .message = "k400BadRequest", .result = exception.what(), .data = {}};

        service::Logger::get_instance().get_logger()->error("ExceptionHandler::nlohmann_exception {}",
                                                            exception.what());

        callback(newHttpJsonResponse(response.to_json()));
    }

    void ExceptionHandler::base_exception(const drogon::HttpRequestPtr&                 request,
                                          std::function<void(const HttpResponsePtr&)>&& callback,
                                          const std::exception&                         exception) {
        const auto* base_exception = dynamic_cast<const BaseException*>(&exception);

        service::Logger::get_instance().get_logger()->error("ExceptionHandler::base_exception {}",
            exception.what());

        callback(newHttpJsonResponse(base_exception->response().to_json()));
    }

    void ExceptionHandler::standard_exception(const HttpRequestPtr&                         request,
                                              std::function<void(const HttpResponsePtr&)>&& callback,
                                              const std::exception&                         exception) {
        service::Logger::get_instance().get_logger()->error("ExceptionHandler::standard_exception {}",
                                                            exception.what());

        callback(newHttpJsonResponse(type::BasicResponse{
                .code    = drogon::k500InternalServerError,
                .message = "k500InternalServerError",
                .result  = exception.what(),
                .data    = {}}.to_json()));
    }

}  // namespace exception
