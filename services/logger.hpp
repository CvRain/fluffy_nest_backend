//
// Created by cvrain on 25-1-20.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fmt/core.h>
#include <memory>
#include <spdlog/spdlog.h>

#include "models/singleton_prototype.hpp"

namespace service {
    template<typename T>
    concept Formattable = requires(T t) {
        {
            fmt::format("{}", t)
        } -> std::convertible_to<std::string>;
    };


    template<typename... Args>
    concept input_args = (Formattable<Args> && ...);

    class Logger : public models::Singleton<Logger> {
    public:
        auto init(spdlog::level::level_enum level, const std::string& log_file_path = "") -> void;
        auto get_logger() -> std::shared_ptr<spdlog::logger>&;

        [[nodiscard]] auto create_logger(const std::string& logger_name) const -> std::shared_ptr<spdlog::logger>;

        /**
         * 启用编译期检查，在debug模式下的日志输出函数
         * @tparam Args 可被转换为字符串的常量类型
         * @param fmt 格式化字符串
         * @param args 传入参数
         */
        template<input_args... Args>
        static auto debug(fmt::format_string<Args...> fmt, Args&&... args) -> void {
            get_instance().get_logger()->debug(fmt, std::forward<Args>(args)...);
        }

        /**
         * 运行时格式字符串版本，在debug模式下的日志输出函数
         * @tparam Args 可被转换为字符串的常量类型
         * @param fmt 格式化字符串
         * @param args 传入参数
         */
        template<input_args... Args>
        static void debug_runtime(const std::string_view fmt, Args&&... args) {
            get_instance().get_logger()->debug(fmt::runtime(fmt), std::forward<Args>(args)...);
        }

        /**
         * 启用编译期检查，在info模式下的日志输出函数
         * @tparam Args 可被转换为字符串的常量类型
         * @param fmt 格式化字符串
         * @param args 传入参数
         */
        template<input_args... Args>
        static auto info(fmt::format_string<Args...> fmt, Args&&... args) -> void {
            get_instance().get_logger()->info(fmt, std::forward<Args>(args)...);
        }

        /**
         * 运行时格式字符串版本，在info模式下的日志输出函数
         * @tparam Args 可被转换为字符串的常量类型
         * @param fmt 格式化字符串
         * @param args 传入参数
         */
        template<input_args... Args>
        static auto info_runtime(const std::string_view fmt, Args&&... args) -> void {
            get_instance().get_logger()->info(fmt::runtime(fmt), std::forward<Args>(args)...);
        }

        /**
         * 启用编译期检查，在warn模式下的日志输出函数
         * @tparam Args 可被转换为字符串的常量类型
         * @param fmt 格式化字符串
         * @param args 传入参数
         */
        template<input_args... Args>
        static auto warn(fmt::format_string<Args...> fmt, Args&&... args) -> void {
            get_instance().get_logger()->warn(fmt, std::forward<Args>(args)...);
        }

        /**
         * 运行时格式字符串版本，在warn模式下的日志输出函数
         * @tparam Args 可被转换为字符串的常量类型
         * @param fmt 格式化字符串
         * @param args 传入参数
         */
        template<input_args... Args>
        static auto warn_runtime(const std::string_view fmt, Args&&... args) -> void {
            get_instance().get_logger()->warn(fmt::runtime(fmt), std::forward<Args>(args)...);
        }

        /**
         * 启用编译期检查，在error模式下的日志输出函数
         * @tparam Args 可被转换为字符串的常量类型
         * @param fmt 格式化字符串
         * @param args 传入参数
         */
        template<input_args... Args>
        static auto error(fmt::format_string<Args...> fmt, Args&&... args) -> void {
            get_instance().get_logger()->error(fmt, std::forward<Args>(args)...);
        }

        /**
         * 运行时格式字符串版本，在error模式下的日志输出函数
         * @tparam Args 可被转换为字符串的常量类型
         * @param fmt 格式化字符串
         * @param args 传入参数
         */
        template<input_args... Args>
        static auto error_runtime(const std::string_view fmt, Args&&... args) -> void {
            get_instance().get_logger()->error(fmt::runtime(fmt), std::forward<Args>(args)...);
        }

        /**
         * 启用编译期检查，在critical模式下的日志输出函数
         * @tparam Args 可被转换为字符串的常量类型
         * @param fmt 格式化字符串
         * @param args 传入参数
         */
        template<input_args... Args>
        static auto critical(fmt::format_string<Args...> fmt, Args&&... args) -> void {
            get_instance().get_logger()->critical(fmt, std::forward<Args>(args)...);
        }

        /**
         * 运行时格式字符串版本，在critical模式下的日志输出函数
         * @tparam Args 可被转换为字符串的常量类型
         * @param fmt 格式化字符串
         * @param args 传入参数
         */
        template<input_args... Args>
        static auto critical_runtime(const std::string_view fmt, Args&&... args) -> void {
            get_instance().get_logger()->critical(fmt::runtime(fmt), std::forward<Args>(args)...);
        }

        /**
         *启用编译期检查，在trace模式下的日志输出函数
         * @tparam Args 可被转换为字符串的常量类型
         * @param fmt 格式化字符串
         * @param args 传入参数
         */
        template<input_args... Args>
        static auto trace(fmt::format_string<Args...> fmt, Args&&... args) -> void {
            get_instance().get_logger()->trace(fmt, std::forward<Args>(args)...);
        }

        /**
         * 运行时格式字符串版本，在trace模式下的日志输出函数
         * @tparam Args 可被转换为字符串的常量类型
         * @param fmt 格式化字符串
         * @param args 传入参数
         */
        template<input_args... Args>
        static auto trace_runtime(const std::string_view fmt, Args&&... args) -> void {
            get_instance().get_logger()->trace(fmt::runtime(fmt), std::forward<Args>(args)...);
        }

    private:
        std::shared_ptr<spdlog::logger> logger;
    };

}  // namespace service


#endif  // LOGGER_HPP
