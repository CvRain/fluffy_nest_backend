//
// Created by cvrain on 25-1-20.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <memory>
#include <spdlog/spdlog.h>


#include "models/singleton_prototype.hpp"

namespace service {
    class Logger: public models::Singleton<Logger>{
    public:
        void init(const std::string& log_level = "info", const std::string& log_file_path = "");
        std::shared_ptr<spdlog::logger>& get_logger();
        std::shared_ptr<spdlog::logger> create_logger(const std::string& logger_name) const;
    private:
        std::shared_ptr<spdlog::logger> logger;
    };

}


#endif //LOGGER_HPP
