#include <drogon/drogon.h>

#include "services/logger.hpp"

void print_logo();

int main() {
    constexpr auto setting_path = "./default_setting.json";
    drogon::app().loadConfigFile(setting_path);

    service::Logger::get_instance().init();
    print_logo();
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
