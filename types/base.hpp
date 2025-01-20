//
// Created by cvrain on 25-1-19.
//

#ifndef BASE_HPP
#define BASE_HPP

#include <exception>
#include <expected>

namespace type {
    template <typename T>
    using result = std::expected<T, std::string_view>;
}

#endif //BASE_HPP
