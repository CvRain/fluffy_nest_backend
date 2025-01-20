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
    };
}


#endif //USERS_HPP
