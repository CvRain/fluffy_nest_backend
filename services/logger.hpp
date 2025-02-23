//
// Created by cvrain on 25-1-20.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <memory>
#include <spdlog/spdlog.h>

#include "models/singleton_prototype.hpp"

namespace service {
    // 限制args需要能够转换为std::string
    template <typename T>
    concept StringConvertible = requires(T t) {
        {std::to_string(t)} -> std::same_as<std::string>;
    } || std::convertible_to<T, std::string>;

    template <typename... Args>
    concept input_args = (StringConvertible<Args> && ...);


    class Logger : public models::Singleton<Logger> {
    public:
        auto init(spdlog::level::level_enum level, const std::string& log_file_path = "") -> void;
        auto get_logger() -> std::shared_ptr<spdlog::logger>&;

        [[nodiscard]] auto create_logger(const std::string& logger_name) const -> std::shared_ptr<spdlog::logger>;

        template<input_args... Args>
        static auto debug(Args&&... args) -> void {
            get_instance().get_logger()->debug(std::forward<Args>(args)...);
        }

        template<input_args... Args>
        static auto info(Args&&... args) -> void {
            get_instance().get_logger()->info(std::forward<Args>(args)...);
        }

        template<input_args... Args>
        static auto warn(Args&&... args) -> void {
            get_instance().get_logger()->warn(std::forward<Args>(args)...);
        }

        template<input_args... Args>
        static auto error(Args&&... args) -> void {
            get_instance().get_logger()->error(std::forward<Args>(args)...);
        }

        template<input_args... Args>
        static auto critical(Args&&... args) -> void {
            get_instance().get_logger()->critical(std::forward<Args>(args)...);
        }

    private:
        std::shared_ptr<spdlog::logger> logger;
    };

}  // namespace service


#endif  // LOGGER_HPP
