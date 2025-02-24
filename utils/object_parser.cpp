//
// Created by cvrain on 25-2-24.
//

#include "object_parser.hpp"
auto ObjectParser::split_path(const std::string& path) -> std::vector<std::string> {
    string_list        parts;
    std::istringstream iss(path);
    std::string        part;

    while (std::getline(iss, part, '/')) {
        if (!part.empty()) {
            parts.push_back(part);
        }
    }
    return parts;
}

auto ObjectParser::get_last_part(const std::string& path) -> std::string {
    auto parts = ObjectParser::split_path(path);
    return parts.empty() ? "" : parts.back();
}

auto ObjectParser::process_parts(nlohmann::json& parent, const std::vector<std::string>& parts, const bool is_directory)
        -> void {
    nlohmann::json* current = &parent;
    for (size_t i = 0; i < parts.size(); ++i) {
        const std::string& part = parts[i];

        const bool is_last = (i == parts.size() - 1);
        const bool is_file = is_last ? !is_directory : false;

        nlohmann::json* child = nullptr;
        for (auto& item: (*current)["children"]) {
            if (item["name"] == part) {
                child = &item;
                break;
            }
        }

        if (!child) {
            nlohmann::json new_node;
            new_node["name"] = part;
            new_node["type"] = is_file ? "file" : "directory";
            if (!is_file) {
                new_node["children"] = nlohmann::json::array();
            }
            (*current)["children"].push_back(new_node);
            child = &(*current)["children"].back();
        }

        if (child->at("type") == "directory") {
            current = child;
        }
        else {
            break;
        }
    }
}
