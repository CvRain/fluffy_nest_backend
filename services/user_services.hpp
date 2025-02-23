//
// Created by cvrain on 25-1-20.
//

#ifndef USER_SERVICES_HPP
#define USER_SERVICES_HPP

#include "models/singleton_prototype.hpp"
#include "models/users.hpp"

namespace service {
    class UserServices final : public models::Singleton<UserServices> {
    public:
        [[nodiscard]] static auto size() -> size_t;
        [[nodiscard]] static auto append(const type::UserSchema& user) -> type::result<std::string>;
        [[nodiscard]] static auto user_exist(const std::string& id, const std::string& email) -> type::result<bool>;
        [[nodiscard]] static auto id_exist(const std::string& id) -> std::optional<bool>;
        [[nodiscard]] static auto name_exist(const std::string& name) -> std::optional<bool>;
        [[nodiscard]] static auto email_exist(const std::string& email) -> std::optional<bool>;
        [[nodiscard]] static auto remove_by_id(const std::string& id) -> std::optional<bool>;
        [[nodiscard]] static auto remove_by_email(const std::string& email) -> std::optional<bool>;
        [[nodiscard]] static auto get_by_id(const std::string& id) -> std::optional<type::UserSchema>;
        [[nodiscard]] static auto get_by_email(const std::string& email) -> std::optional<type::UserSchema>;
        static auto check_token(const std::string& token) -> type::result<bool>;


    private:
        std::shared_ptr<models::UserModel> user_model{};
    };
}  // namespace service


#endif  // USER_SERVICES_HPP
