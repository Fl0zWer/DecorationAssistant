cmake_minimum_required(VERSION 3.20)
project(DecorationAssistant)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Geode REQUIRED)

file(GLOB_RECURSE DECORATION_ASSISTANT_SOURCES
    CONFIGURE_DEPENDS
    "src/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${DECORATION_ASSISTANT_SOURCES})

# Export compile_commands.json for tooling
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    OUTPUT_NAME "DecorationAssistant"
)

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} PRIVATE geode-sdk::geode)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    DECORATION_ASSISTANT_VERSION="0.2.0"
)

geode_setup_mod(${PROJECT_NAME})
