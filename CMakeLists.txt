cmake_minimum_required(VERSION 3.20)
project(DecorationAssistant)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Try to auto-detect the Geode SDK if the user didn't set Geode_DIR manually.
set(_GEODE_CANDIDATE_BASES)

# Respect common environment variables that point to the SDK.
foreach(_var IN ITEMS GEODE_SDK GEODE_SDK_PATH GEODE_SDK_ROOT GEODE_DIR)
    if(DEFINED ENV{${_var}})
        list(APPEND _GEODE_CANDIDATE_BASES "$ENV{${_var}}")
    endif()
endforeach()

# Add a few conventional install locations as fallbacks.
list(APPEND _GEODE_CANDIDATE_BASES "${CMAKE_SOURCE_DIR}/../geode-sdk")

if(DEFINED ENV{HOME})
    list(APPEND _GEODE_CANDIDATE_BASES "$ENV{HOME}/geode-sdk")
endif()

if(DEFINED ENV{LOCALAPPDATA})
    list(APPEND _GEODE_CANDIDATE_BASES "$ENV{LOCALAPPDATA}/geode-sdk")
endif()

if(DEFINED ENV{USERPROFILE})
    list(APPEND _GEODE_CANDIDATE_BASES "$ENV{USERPROFILE}/geode-sdk")
endif()

set(_GEODE_CONFIG_HINTS)
foreach(_base IN LISTS _GEODE_CANDIDATE_BASES)
    if(NOT _base)
        continue()
    endif()

    # Normalize separators so CMake can reason about the path in a cross-platform way.
    file(TO_CMAKE_PATH "${_base}" _normalized_base)

    foreach(_suffix IN ITEMS "" "share/cmake/Geode" "cmake/Geode" "Geode")
        set(_candidate "${_normalized_base}/${_suffix}")
        if(EXISTS "${_candidate}/GeodeConfig.cmake")
            list(APPEND _GEODE_CONFIG_HINTS "${_candidate}")
        endif()
    endforeach()
endforeach()

# Ensure Geode_DIR (if provided manually) takes precedence over auto-detected hints.
if(DEFINED Geode_DIR)
    list(PREPEND _GEODE_CONFIG_HINTS "${Geode_DIR}")
endif()

list(REMOVE_DUPLICATES _GEODE_CONFIG_HINTS)

set(_GEODE_FIND_PACKAGE_ARGS CONFIG)
if(_GEODE_CONFIG_HINTS)
    list(APPEND _GEODE_FIND_PACKAGE_ARGS HINTS ${_GEODE_CONFIG_HINTS})
endif()

find_package(Geode QUIET ${_GEODE_FIND_PACKAGE_ARGS})

if(NOT Geode_FOUND)
    if(_GEODE_CONFIG_HINTS)
        string(JOIN "\n  - " _GEODE_HINTS_MESSAGE ${_GEODE_CONFIG_HINTS})
        message(FATAL_ERROR "No se pudo localizar el Geode SDK automáticamente. Se revisaron las rutas:\n  - ${_GEODE_HINTS_MESSAGE}\nDefine Geode_DIR manualmente o instala el SDK siguiendo https://docs.geode-sdk.org/")
    else()
        message(FATAL_ERROR "No se pudo localizar el Geode SDK. Define Geode_DIR o agrega el SDK a CMAKE_PREFIX_PATH. Consulta https://docs.geode-sdk.org/")
    endif()
endif()

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
