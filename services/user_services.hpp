//
// Created by cvrain on 25-1-20.
//

#ifndef USER_SERVICES_HPP
#define USER_SERVICES_HPP

#include "models/singleton_prototype.hpp"
#include "models/users.hpp"

namespace service {
    class UserServices final: public models::Singleton<UserServices>{
    public:
        [[nodiscard]] auto size() const -> size_t;
        auto append(const type::UserSchema& user) const -> type::result<std::string>;
        auto user_exist(const std::string& id, const std::string& email) const -> type::result<bool>;
        auto id_exist(const std::string& id) const -> std::optional<bool>;
        auto email_exist(const std::string& email) const -> std::optional<bool>;
    private:
        std::shared_ptr<models::UserModel> user_model{};
    };
}



#endif //USER_SERVICES_HPP
