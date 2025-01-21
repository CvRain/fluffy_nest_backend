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
        [[nodiscard]] auto size() const -> type::result<size_t>;

        /// 插入一条数据
        /// @return 插入成功为ture,插入失败为false, 插入错误为异常
        auto append(const type::UserSchema& user) -> type::result<bool>;
    };
}


#endif //USERS_HPP
