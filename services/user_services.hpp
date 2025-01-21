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
        auto append(const type::UserSchema& user) const -> type::result<std::string_view>;
    private:
        std::shared_ptr<models::UserModel> user_model{};
    };
}



#endif //USER_SERVICES_HPP