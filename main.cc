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
    framework_init();
    service_init();

    drogon::app().run();
    cleanup();

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
    service::Logger::info(text_logo);
    service::Logger::info("Hello fluffy nest");
}

void framework_init() {
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
}

void service_init() {
    service::ObjectStorageService::get_instance().init();
    service::Logger::info("Service init success");

    service_test();
}

void service_test() {
    service::Logger::info("Service test start");
    try {
        const auto result = service::ObjectStorageService::get_instance().recursive_directory();
        if (not result.has_value()) {
            service::Logger::error_runtime("ObjectStorageService::recursive_directory", result.error());
        }
        service::Logger::info_runtime("ObjectStorageService::recursive_directory", result.value().dump());
    }
    catch (const std::exception &e) {
        service::Logger::error_runtime("service_test", e.what());
    }
    service::Logger::info("Service test end");
}


void cleanup() { service::ObjectStorageService::get_instance().shutdown_api(); }
