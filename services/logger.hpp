//
// Created by cvrain on 25-1-20.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <memory>
#include <spdlog/spdlog.h>

#include "models/singleton_prototype.hpp"

namespace service {
    class Logger : public models::Singleton<Logger> {
    public:
        auto               init(spdlog::level::level_enum level, const std::string& log_file_path = "") -> void;
        auto               get_logger() -> std::shared_ptr<spdlog::logger>&;
        [[nodiscard]] auto create_logger(const std::string& logger_name) const -> std::shared_ptr<spdlog::logger>;

    private:
        std::shared_ptr<spdlog::logger> logger;
    };

}  // namespace service


#endif  // LOGGER_HPP
