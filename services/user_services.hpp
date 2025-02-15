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
        [[nodiscard]] auto size() const -> size_t;
        [[nodiscard]] auto append(const type::UserSchema& user) const -> type::result<std::string>;
        [[nodiscard]] auto user_exist(const std::string& id, const std::string& email) const -> type::result<bool>;
        [[nodiscard]] auto id_exist(const std::string& id) const -> std::optional<bool>;
        [[nodiscard]] auto name_exist(const std::string& name) const -> std::optional<bool>;
        [[nodiscard]] auto email_exist(const std::string& email) const -> std::optional<bool>;
        [[nodiscard]] auto remove_by_id(const std::string& id) const -> std::optional<bool>;
        [[nodiscard]] auto remove_by_email(const std::string& email) const -> std::optional<bool>;
        [[nodiscard]] auto get_by_id(const std::string& id) const -> std::optional<type::UserSchema>;
        [[nodiscard]] auto get_by_email(const std::string& email) const -> std::optional<type::UserSchema>;
        static auto check_token(const std::string& token) -> type::result<bool>;


    private:
        std::shared_ptr<models::UserModel> user_model{};
    };
}  // namespace service


#endif  // USER_SERVICES_HPP
