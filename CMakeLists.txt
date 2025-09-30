cmake_minimum_required(VERSION 3.21)

project(DecorationAssistant
  VERSION 0.1.0
  LANGUAGES CXX
)

# ===== Opciones del desarrollador =====
option(DA_UNITY_BUILD         "Compilar con Unity Build para acelerar iteraciones" OFF)
option(DA_WARNINGS_AS_ERRORS  "Tratar warnings como errores"                      OFF)
option(DA_COPY_TO_MODS_DIR    "Copiar el binario al folder de mods tras compilar" OFF)
option(DA_FETCH_GEODE         "Clonar el Geode SDK si no se encuentra localmente" OFF)

# Permitir indicar el SDK por -DGEODE_SDK_DIR=... (tiene prioridad)
set(GEODE_SDK_DIR "" CACHE PATH "Ruta al Geode SDK (si no se usa ENV{GEODE_SDK})")
set(DA_GEODE_TAG  "main" CACHE STRING "Tag/branch del Geode SDK al hacer FetchContent")

# ===== Estándar y flags =====
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

if(MSVC)
  add_compile_options(/W4 /permissive- /MP)
  if(DA_WARNINGS_AS_ERRORS)
    add_compile_options(/WX)
  endif()
  # Soporte UTF-8 en Windows
  add_compile_options(/utf-8)
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
elif(DEFINED ENV{GEODE_SDK})
  set(_GEODE_SDK_PATH "$ENV{GEODE_SDK}")
elif(EXISTS "${CMAKE_SOURCE_DIR}/extern/geode-sdk/CMakeLists.txt")
  set(_GEODE_SDK_PATH "${CMAKE_SOURCE_DIR}/extern/geode-sdk")
elif(DA_FETCH_GEODE)
  include(FetchContent)
  message(STATUS "DA_FETCH_GEODE=ON → descargando Geode SDK (tag=${DA_GEODE_TAG})")
  FetchContent_Declare(geode-sdk
    GIT_REPOSITORY https://github.com/geode-sdk/geode.git
    GIT_TAG        ${DA_GEODE_TAG}
    GIT_SHALLOW    TRUE
  )
  FetchContent_GetProperties(geode-sdk)
  if(NOT geode-sdk_POPULATED)
    FetchContent_Populate(geode-sdk)
  endif()
  set(_GEODE_SDK_PATH "${geode-sdk_SOURCE_DIR}")
endif()

if(NOT _GEODE_SDK_PATH)
  message(FATAL_ERROR
    "\n[Geode] No se encontró el SDK.\n"
    "Soluciones:\n"
    "  1) cmake -S . -B build -DGEODE_SDK_DIR=C:/ruta/al/geode-sdk\n"
    "  2) setx GEODE_SDK C:/ruta/al/geode-sdk (reinicia la terminal)\n"
    "  3) coloca el SDK en extern/geode-sdk\n"
    "  4) o usa -DDA_FETCH_GEODE=ON para clonarlo automáticamente\n"
  )
endif()

if(NOT EXISTS "${_GEODE_SDK_PATH}/CMakeLists.txt")
  message(FATAL_ERROR
    "[Geode] La ruta del SDK (${_GEODE_SDK_PATH}) no contiene un CMakeLists.txt válido."
  )
endif()

message(STATUS "Geode SDK detectado en: ${_GEODE_SDK_PATH}")

# ===== SDK como subdirectorio =====
add_subdirectory("${_GEODE_SDK_PATH}" "${CMAKE_CURRENT_BINARY_DIR}/_geode")

# ===== Fuentes del mod =====
file(GLOB_RECURSE DA_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

if(DA_UNITY_BUILD)
  message(STATUS "Unity Build: ACTIVADO")
else()
  message(STATUS "Unity Build: desactivado")
endif()

add_library(${PROJECT_NAME} SHARED ${DA_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD ${DA_UNITY_BUILD})

target_include_directories(${PROJECT_NAME} PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# Definiciones útiles (no declarar aquí dependencias de otros mods; eso va en mod.json)
target_compile_definitions(${PROJECT_NAME} PRIVATE
  $<$<CONFIG:Debug>:DA_DEBUG=1>
)

# ===== Salidas (bin/) por configuración =====
set(_OUT_DIR "${CMAKE_BINARY_DIR}/bin")
set_target_properties(${PROJECT_NAME} PROPERTIES
  OUTPUT_NAME "${PROJECT_NAME}"
  RUNTIME_OUTPUT_DIRECTORY "${_OUT_DIR}/$<CONFIG>"
  LIBRARY_OUTPUT_DIRECTORY "${_OUT_DIR}/$<CONFIG>"
  ARCHIVE_OUTPUT_DIRECTORY "${_OUT_DIR}/$<CONFIG>"
)

# ===== Integración con Geode (packaging/resources) =====
# Esta macro del SDK lee mod.json, registra resources y prepara empaquetado.
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
    message(WARNING
      "DA_COPY_TO_MODS_DIR=ON pero GEODE_MODS_DIR no está definido. "
      "Define la variable de entorno con la carpeta de mods."
    )
  endif()
endif()

message(STATUS "Listo: genera con 'cmake --build build' o usa 'geode build'.")
