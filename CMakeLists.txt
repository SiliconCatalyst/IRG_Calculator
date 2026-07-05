cmake_minimum_required(VERSION 3.16)
project(irg_calc CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -Wpedantic)

# --- Core library: pure calculation logic, no I/O ---
add_library(irg_core
    src/core/irg_calculator.cpp
)
target_include_directories(irg_core PUBLIC include)

# --- i18n library: language/text lookup, no I/O, no calculation ---
add_library(irg_i18n
    src/i18n/translator.cpp
)
target_include_directories(irg_i18n PUBLIC include)

# --- CLI library: console I/O + orchestration ---
add_library(irg_cli
    src/cli/console_io.cpp
    src/cli/application.cpp
)
target_include_directories(irg_cli PUBLIC include)
target_link_libraries(irg_cli PUBLIC irg_core irg_i18n)

# --- Executable ---
add_executable(irg_calc src/main.cpp)
target_link_libraries(irg_calc PRIVATE irg_cli)

# --- Tests ---
option(IRG_BUILD_TESTS "Build the test suite" ON)
if(IRG_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
