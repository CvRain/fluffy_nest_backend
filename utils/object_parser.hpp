//
// Created by cvrain on 25-2-24.
//

#ifndef OBJECT_PARSER_HPP
#define OBJECT_PARSER_HPP
#include <string>
#include <vector>

#include "types/nlohmann_json_request.hpp"

struct ParsedObject {
    std::vector<std::string> parts;
    bool                     is_directory;
};

class ObjectParser {
public:
    using string_list = std::vector<std::string>;
    static auto split_path(const std::string& path) -> std::vector<std::string>;
    static auto get_last_part(const std::string& path) -> std::string;
    static auto process_parts(nlohmann::json& parent, const std::vector<std::string>& parts, bool is_directory) -> void;
};


#endif  // OBJECT_PARSER_HPP
