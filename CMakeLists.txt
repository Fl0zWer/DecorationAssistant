cmake_minimum_required(VERSION 3.21)

# ===== Proyecto =====
project(DecorationAssistant VERSION 0.2.0 LANGUAGES CXX)

# Estándar y flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

if(APPLE AND NOT IOS)
  set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)
endif()

option(DA_WARNINGS_AS_ERRORS  "Tratar warnings como errores" OFF)
option(DA_COPY_TO_MODS_DIR    "Copiar binario a la carpeta de mods post-build" OFF)
option(DA_FETCH_GEODE         "Descargar geode-sdk automáticamente si no está" OFF)
set(DA_GEODE_TAG "main" CACHE STRING "Tag/branch del geode-sdk al hacer FetchContent")
set(GEODE_SDK_DIR "" CACHE PATH "Ruta al geode-sdk si no usas ENV{GEODE_SDK}")
set(DA_REQUIRED_DOTNET_RUNTIME "8.0.7" CACHE STRING "Runtime mínimo de .NET requerido por las tareas de Geode")

if(MSVC)
  #
  # Nota sobre /Zm:
  # ----------------
  # Al compilar con MSVC algunos usuarios reportaron el error
  #   C3859/C1090: "no se pudo crear memoria virtual para PCH"
  # que indica que el compilador agotó el espacio reservado para
  # el estado interno/precompiled headers.  Aumentar el valor del
  # flag /Zm incrementa dicho límite y permite que la solución se
  # compile correctamente en máquinas con la configuración por
  # defecto de Visual Studio.
  #
  add_compile_options(/W4 /permissive- /MP /utf-8 /Zm200)
  if(DA_WARNINGS_AS_ERRORS)
    add_compile_options(/WX)
  endif()

  set(_ensure_dotnet_ps1 "${CMAKE_CURRENT_SOURCE_DIR}/scripts/ensure_dotnet_runtime.ps1")
  if(EXISTS "${_ensure_dotnet_ps1}")
    add_custom_target(DecorationAssistantEnsureDotnetRuntime ALL
      COMMAND ${CMAKE_COMMAND} -E echo "Comprobando runtime de .NET ${DA_REQUIRED_DOTNET_RUNTIME}..."
      COMMAND powershell -NoProfile -ExecutionPolicy Bypass -File "${_ensure_dotnet_ps1}" -Version ${DA_REQUIRED_DOTNET_RUNTIME}
      BYPRODUCTS "${CMAKE_BINARY_DIR}/.dotnet-bootstrap"
      COMMENT "Preparando dependencias de Geode (runtime .NET)"
      VERBATIM
    )
  endif()
else()
  add_compile_options(-Wall -Wextra -Wno-unused-parameter)
  if(DA_WARNINGS_AS_ERRORS)
    add_compile_options(-Werror)
  endif()
endif()

# ===== Detectar Geode SDK =====
set(_GEODE_SDK_PATH "")
if(GEODE_SDK_DIR)
  set(_GEODE_SDK_PATH "${GEODE_SDK_DIR}")
elseif(DEFINED ENV{GEODE_SDK})
  set(_GEODE_SDK_PATH "$ENV{GEODE_SDK}")
elseif(EXISTS "${CMAKE_SOURCE_DIR}/extern/geode-sdk/CMakeLists.txt")
  set(_GEODE_SDK_PATH "${CMAKE_SOURCE_DIR}/extern/geode-sdk")
elseif(DA_FETCH_GEODE)
  include(FetchContent)
  message(STATUS "DA_FETCH_GEODE=ON → descargando geode-sdk (${DA_GEODE_TAG})")
  FetchContent_Declare(geode-sdk
    GIT_REPOSITORY https://github.com/geode-sdk/geode.git
    GIT_TAG        ${DA_GEODE_TAG}
    GIT_SHALLOW    TRUE
  )
  FetchContent_MakeAvailable(geode-sdk)
  set(_GEODE_SDK_PATH "${geode-sdk_SOURCE_DIR}")
endif()

if(NOT _GEODE_SDK_PATH)
  message(FATAL_ERROR "[Geode] No se encontró el SDK. Usa -DGEODE_SDK_DIR=... o ENV{GEODE_SDK}, o DA_FETCH_GEODE=ON, o coloca extern/geode-sdk")
endif()
if(NOT EXISTS "${_GEODE_SDK_PATH}/CMakeLists.txt")
  message(FATAL_ERROR "[Geode] La ruta del SDK (${_GEODE_SDK_PATH}) no contiene CMakeLists.txt")
endif()
message(STATUS "Geode SDK: ${_GEODE_SDK_PATH}")

# SDK como subdirectorio (expone setup_geode_mod)
add_subdirectory("${_GEODE_SDK_PATH}" "${CMAKE_CURRENT_BINARY_DIR}/_geode")

# ===== Fuentes =====
# Incluye todo bajo src/ (core, ui, integration, utils, etc.)
file(GLOB_RECURSE DA_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${DA_SOURCES})

if(TARGET DecorationAssistantEnsureDotnetRuntime)
  add_dependencies(${PROJECT_NAME} DecorationAssistantEnsureDotnetRuntime)
  add_custom_command(TARGET DecorationAssistantEnsureDotnetRuntime POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_BINARY_DIR}/.dotnet-bootstrap"
  )
endif()

# Includes del proyecto
target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Definiciones útiles
target_compile_definitions(${PROJECT_NAME} PRIVATE
  $<$<CONFIG:Debug>:DA_DEBUG=1>
)

# ===== Salidas ordenadas =====
set(_OUT_DIR "${CMAKE_BINARY_DIR}/bin")
set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${_OUT_DIR}/$<CONFIG>"
  LIBRARY_OUTPUT_DIRECTORY "${_OUT_DIR}/$<CONFIG>"
  ARCHIVE_OUTPUT_DIRECTORY "${_OUT_DIR}/$<CONFIG>"
)

# ===== Integración con Geode (lee mod.json, resources, empaquetado) =====
# mod.json debe vivir en la raíz del proyecto junto a este CMakeLists.txt
# y resources/ debe contener los íconos UHD (Geode generará M/L)
setup_geode_mod(${PROJECT_NAME})

# ===== Copia opcional al folder de mods del usuario =====
if(DA_COPY_TO_MODS_DIR)
  if(DEFINED ENV{GEODE_MODS_DIR})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory "$ENV{GEODE_MODS_DIR}"
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:${PROJECT_NAME}>"
        "$ENV{GEODE_MODS_DIR}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
      COMMENT "Copiando a %GEODE_MODS_DIR% ..."
    )
    message(STATUS "Copy-to-mods: ACTIVADO → $ENV{GEODE_MODS_DIR}")
  else()
    message(WARNING "DA_COPY_TO_MODS_DIR=ON pero GEODE_MODS_DIR no está definido. Define la variable de entorno con la carpeta de mods.")
  endif()
endif()

message(STATUS "DecorationAssistant listo. Configura con 'cmake -S . -B build' y compila con 'cmake --build build --config Debug' o usa 'geode build'.")
