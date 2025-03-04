//
// Created by cvrain on 25-1-19.
//

#ifndef BASE_HPP
#define BASE_HPP

#include <expected>
#include <ranges>

namespace type {
    template<typename T>
    using result = std::expected<T, std::string>;

    template<typename Func>
    struct PipeAdapter {
        Func func;
        explicit PipeAdapter(Func f) : func(f) {}

        template<typename T>
        auto operator()(T&& value) const {
            return func(std::forward<T>(value));
        }
    };

    // 辅助函数创建适配器
    template<typename Func>
    PipeAdapter<Func> MakePipe(Func f) {
        return PipeAdapter<Func>(f);
    }

    // 重载operator|，支持链式调用
    template<typename T, typename Func>
    auto operator|(T&& value, const PipeAdapter<Func>& adapter) {
        return adapter(std::forward<T>(value));
    }
}  // namespace type

#endif  // BASE_HPP
