#include <drogon/drogon.h>

#include "services/logger.hpp"
#include "services/object_storage_service.hpp"

void print_logo();
void framework_init();
void service_init();
void service_test();
void cleanup();

int main() {
    constexpr auto setting_path = "./default_setting.json";
    drogon::app().loadConfigFile(setting_path);

    service::Logger::get_instance().init(spdlog::level::trace);

    print_logo();
    service_init();
    framework_init();

    cleanup();
    return 0;
}

void framework_init() {
    drogon::app()
            .setExceptionHandler([](const std::exception                            &e,
                                    const drogon::HttpRequestPtr                    &req,
                                    std::function<void(drogon::HttpResponsePtr &)> &&callback) {
                // LOG_DEBUG << e.what();
                Json::Value json;
                json["code"]  = drogon::k500InternalServerError;
                json["error"] = e.what();
                json["data"]  = "";
                auto resp     = drogon::HttpResponse::newHttpJsonResponse(json);
                callback(resp);
            })
            .registerPreRoutingAdvice([](const drogon::HttpRequestPtr &req,
                                         drogon::FilterCallback      &&stop,
                                         drogon::FilterChainCallback &&pass) {
                // 处理 OPTIONS 预检请求
                if (req->method() == drogon::HttpMethod::Options) {
                    auto        resp   = drogon::HttpResponse::newHttpResponse();
                    const auto &origin = req->getHeader("Origin");
                    // 动态设置 Allow-Origin
                    if (!origin.empty()) {
                        resp->addHeader("Access-Control-Allow-Origin", origin);
                    }
                    else {
                        resp->addHeader("Access-Control-Allow-Origin", "*");
                    }
                    // 允许的 Methods 和 Headers
                    resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
                    const auto &reqHeaders = req->getHeader("Access-Control-Request-Headers");
                    if (!reqHeaders.empty()) {
                        resp->addHeader("Access-Control-Allow-Headers", reqHeaders);
                    }
                    // 允许携带凭证（如 Cookie）
                    resp->addHeader("Access-Control-Allow-Credentials", "true");
                    // 设置状态码为 204（无内容）
                    resp->setStatusCode(drogon::k204NoContent);
                    stop(resp);
                    return;
                }
                pass();
            })
            .registerPostHandlingAdvice([](const drogon::HttpRequestPtr &req, const drogon::HttpResponsePtr &resp) {
                // 为所有响应添加 CORS 头
                const auto &origin = req->getHeader("Origin");
                if (!origin.empty()) {
                    resp->addHeader("Access-Control-Allow-Origin", origin);
                }
                else {
                    resp->addHeader("Access-Control-Allow-Origin", "*");
                }
                resp->addHeader("Access-Control-Allow-Credentials", "true");
            })
            .run();
}

void service_init() {
    service::ObjectStorageService::get_instance().init();
    service::Logger::info("Service init success");

    service_test();
}

void service_test() {
    try {
        service::Logger::info("Service test start");
        //service::ObjectStorageService::get_instance().list_directory();
    }
    catch (const std::exception &e) {
        service::Logger::error_runtime("service_test", e.what());
    }
    service::Logger::info("Service test end");
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
    service::Logger::info(text_logo);
    service::Logger::info("Hello fluffy nest");
}

void cleanup() { service::ObjectStorageService::get_instance().shutdown_api(); }
