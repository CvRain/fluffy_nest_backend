//
// Created by cvrain on 25-1-19.
//

#ifndef SCHEMA_HPP
#define SCHEMA_HPP

#include <string>
#include <string_view>
#include <trantor/utils/Date.h>

namespace type{
    class UserSchema {
    public:
        std::string id;
        std::string name;
        std::string password;
        std::string email;
        int role;
        std::string icon;
        std::string signature;
        trantor::Date create_time;
        trantor::Date update_time;

        static constexpr std::string_view key_id = "id";
        static constexpr std::string_view key_name = "name";
        static constexpr std::string_view key_user_id = "user_id";
        static constexpr std::string_view key_user_name = "user_name";
        static constexpr std::string_view key_password = "password";
        static constexpr std::string_view key_email = "email";
        static constexpr std::string_view key_role = "role";
        static constexpr std::string_view key_icon = "icon";
        static constexpr std::string_view key_signature = "signature";
        static constexpr std::string_view key_create_time = "create_time";
        static constexpr std::string_view key_update_time = "update_time";
    };

    class ObjectSchema {
    public:

    };

}

#endif //SCHEMA_HPP
