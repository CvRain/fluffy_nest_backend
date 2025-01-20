//
// Created by cvrain on 25-1-20.
//

#include "logger.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h> 

namespace service {
    void Logger::init(const std::string& log_level, const std::string& log_file_path) {
        // 设置日志格式
        const std::string pattern = "[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v";

        // 创建一个支持颜色的控制台日志记录器
        logger = spdlog::stdout_color_mt("fluffy_nest_logger");
        logger->set_pattern(pattern);

        // 设置日志级别
        if (log_level == "debug") {
            logger->set_level(spdlog::level::debug);
        } else if (log_level == "warn") {
            logger->set_level(spdlog::level::warn);
        } else if (log_level == "error") {
            logger->set_level(spdlog::level::err);
        } else if (log_level == "critical") {
            logger->set_level(spdlog::level::critical);
        } else {
            logger->set_level(spdlog::level::info);
        }

        // 如果指定了日志文件路径，则创建一个文件日志记录器
        if (!log_file_path.empty()) {
            const auto file_logger = spdlog::basic_logger_mt("fluffy_nest_file_logger", log_file_path);
            file_logger->set_pattern(pattern);
            file_logger->set_level(logger->level());
            spdlog::register_logger(file_logger);
        }
    }

    std::shared_ptr<spdlog::logger> Logger::create_logger(const std::string& logger_name) const {
        auto new_logger = spdlog::stdout_color_mt(logger_name);
        new_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v");
        new_logger->set_level(logger->level());
        return new_logger;
    }

    std::shared_ptr<spdlog::logger> & Logger::get_logger() {
        return logger;
    }

}