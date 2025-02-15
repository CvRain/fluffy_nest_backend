//
// Created by cvrain on 25-1-19.
//

#ifndef USERS_HPP
#define USERS_HPP

#include "types/schema.hpp"
#include "types/base.hpp"

#include <drogon/drogon.h>
#include <string>

namespace models {
    class UserModel {
    public:
        explicit UserModel();

        /// 统计表中存在多少条数据
        /// @return 成功为数据大小，失败为异常
        [[nodiscard]] static auto size() -> type::result<size_t>;

        /// 插入一条数据
        /// @return 插入成功为ture,插入失败为false, 插入错误为异常
        static auto append(const type::UserSchema& user) -> type::result<bool>;

        /// 判断表中是否存在id
        /// @return 存在为true，不存在为false，判断错误为异常
        static auto has_id(const std::string& id)  -> type::result<bool>;

        /// 判断表中是否存在email
        /// @return 存在为true，不存在为false，判断错误为异常
        [[nodiscard]] static auto has_email(const std::string& email) -> type::result<bool>;

        /// 判断表中是否存在name
        /// @return 存在为true，不存在为false，判断错误为异常
        [[nodiscard]] static auto has_name(const std::string& name) -> type::result<bool>;

        /// 通过id删除一条数据
        /// @return 返回执行后影响的行数，删除错误为异常
        static auto remove_by_id(const std::string& id) -> type::result<int>;

        /// 通过email删除一条数据
        /// @return 返回执行后影响的行数，删除错误为异常
        static auto remove_by_email(const std::string& email) -> type::result<int>;

        /// 通过id获取一条数据
        /// @return 返回获取的数据，获取错误为异常
        static auto get_by_id(const std::string& id) -> type::result<type::UserSchema>;

        /// 通过email获取一条数据
        /// @return 返回获取的数据，获取错误为异常
        static auto get_by_email(const std::string& email) -> type::result<type::UserSchema>;
    };
}


#endif //USERS_HPP
