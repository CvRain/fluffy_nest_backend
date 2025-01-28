#include <drogon/drogon.h>

#include "services/logger.hpp"

void print_logo();

int main() {
    constexpr auto setting_path = "./default_setting.json";
    drogon::app().loadConfigFile(setting_path);

    service::Logger::get_instance().init(spdlog::level::trace);
    print_logo();

    // 全局异常处理
    drogon::app().setExceptionHandler([](const std::exception                            &e,
                                         const drogon::HttpRequestPtr                    &req,
                                         std::function<void(drogon::HttpResponsePtr &)> &&callback) {
        // LOG_DEBUG << e.what();
        Json::Value json;
        json["code"]  = drogon::k500InternalServerError;
        json["error"] = e.what();
        json["data"]  = "";
        auto resp     = drogon::HttpResponse::newHttpJsonResponse(json);
        callback(resp);
    });

    // 全局异常处理
    drogon::app().setExceptionHandler([](const std::exception                            &e,
                                         const drogon::HttpRequestPtr                    &req,
                                         std::function<void(drogon::HttpResponsePtr &)> &&callback) {
        // LOG_DEBUG << e.what();
        Json::Value json;
        json["code"]  = drogon::k500InternalServerError;
        json["error"] = e.what();
        json["data"]  = "";
        auto resp     = drogon::HttpResponse::newHttpJsonResponse(json);
        callback(resp);
    });

    // 跨域配置
    drogon::app().registerSyncAdvice([](const drogon::HttpRequestPtr &req) -> drogon::HttpResponsePtr {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, PATCH, OPTIONS");
        resp->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
        resp->addHeader("Access-Control-Allow-Credentials", "true");

        if (req->method() == drogon::HttpMethod::Options) {
            resp->setStatusCode(drogon::k204NoContent);
            return resp;
        }

        return nullptr;
    });

    drogon::app().registerPostHandlingAdvice(
            [](const drogon::HttpRequestPtr &req, const drogon::HttpResponsePtr &resp) -> void {
                if (const auto &val = req->getHeader("Origin"); !val.empty())
                    resp->addHeader("Access-Control-Allow-Origin", val);

                if (const auto &val = req->getHeader("Access-Control-Request-Method"); !val.empty())
                    resp->addHeader("Access-Control-Allow-Methods", val);

                resp->addHeader("Access-Control-Allow-Credentials", "true");

                if (const auto &val = req->getHeader("Access-Control-Request-Headers"); !val.empty())
                    resp->addHeader("Access-Control-Allow-Headers", val);
            });

    drogon::app().run();

    return 0;
}

void print_logo() {
    constexpr auto text_logo = R"(
  ______ _     _    _ ______ ________     __     _   _ ______  _____ _______
 |  ____| |   | |  | |  ____|  ____\ \   / /    | \ | |  ____|/ ____|__   __|
 | |__  | |   | |  | | |__  | |__   \ \_/ /     |  \| | |__  | (___    | |
 |  __| | |   | |  | |  __| |  __|   \   /      | . ` |  __|  \___ \   | |
 | |    | |___| |__| | |    | |       | |       | |\  | |____ ____) |  | |
 |_|    |______\____/|_|    |_|       |_|       |_| \_|______|_____/   |_|
)";
    service::Logger::get_instance().get_logger()->info(text_logo);
    service::Logger::get_instance().get_logger()->info("Hello fluffy nest");
}
