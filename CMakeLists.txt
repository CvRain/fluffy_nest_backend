cmake_minimum_required(VERSION 3.20)
project(fluffy_nest CXX)

include(CheckIncludeFileCXX)

check_include_file_cxx(any HAS_ANY)
check_include_file_cxx(string_view HAS_STRING_VIEW)
check_include_file_cxx(coroutine HAS_COROUTINE)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)

add_executable(${PROJECT_NAME} main.cc)

find_package(Drogon CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# ##############################################################################

#aux_source_directory(controllers CTL_SRC)
#aux_source_directory(filters FILTER_SRC)
#aux_source_directory(plugins PLUGIN_SRC)
#aux_source_directory(models MODEL_SRC)
#aux_source_directory(types TYPES_SRC)
#aux_source_directory(services SERVICES_SRC)

set(CTL_SRC
        controllers/api_hello.h
        controllers/api_hello.cc
        controllers/api_user.h
        controllers/api_user.cc
)

set(FILTER_SRC
        filters/filter_UserAuth.cc
        filters/filter_UserAuth.h
        filters/filter_UserEmailExist.cc
        filters/filter_UserEmailExist.h
        filters/filter_UserIdExist.cc
        filters/filter_UserIdExist.h)

set(PLUGIN_SRC)

set(MODEL_SRC
        models/singleton_prototype.hpp
        models/users.hpp
        models/users.cpp
)

set(TYPES_SRC
        types/base.hpp
        types/basic_value.hpp
        types/nlohmann_json_request.hpp
        types/nlohmann_json_response.hpp
        types/schema.hpp
        types/type.hpp
)

set(SERVICES_SRC
        services/logger.hpp
        services/logger.cpp
        services/user_services.hpp
        services/user_services.cpp
)

set(UTILS_SRC
        utils/date.h
        utils/drogon_specialization.hpp
        utils/exception_handler.cpp
        utils/exception_handler.hpp
)

drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
        ${CMAKE_CURRENT_BINARY_DIR})
# use the following line to create views with namespaces.
# drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR} TRUE)
# use the following line to create views with namespace CHANGE_ME prefixed
# and path namespaces.
# drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR} TRUE CHANGE_ME)

target_include_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/models)

target_sources(${PROJECT_NAME}
        PRIVATE
        ${SRC_DIR}
        ${CTL_SRC}
        ${FILTER_SRC}
        ${PLUGIN_SRC}
        ${MODEL_SRC}
        ${TYPES_SRC}
        ${SERVICES_SRC}
        ${UTILS_SRC}
)
# ##############################################################################
# uncomment the following line for dynamically loading views 
# set_property(TARGET ${PROJECT_NAME} PROPERTY ENABLE_EXPORTS ON)

# ##############################################################################

add_subdirectory(test)
