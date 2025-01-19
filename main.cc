#include <drogon/drogon.h>

int main() {
    constexpr auto setting_path = "./default_setting.json";
    drogon::app().loadConfigFile(setting_path).run();
    return 0;
}
