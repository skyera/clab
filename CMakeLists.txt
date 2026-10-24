cmake_minimum_required(VERSION 3.16)
project(clab LANGUAGES C)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/uthash/src/uthash.h")
    message(FATAL_ERROR "uthash is missing. Run: git submodule update --init --recursive")
endif()

add_executable(uthash_demo src/main.c)
set_target_properties(uthash_demo PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)
target_include_directories(uthash_demo PRIVATE third_party/uthash/src)
if(MSVC)
    target_compile_options(uthash_demo PRIVATE /W4)
else()
    target_compile_options(uthash_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(CTest)
if(BUILD_TESTING)
    add_test(NAME uthash_demo_runs COMMAND uthash_demo)
    set_tests_properties(uthash_demo_runs PROPERTIES
        PASS_REGULAR_EXPRESSION "After cleanup: 0"
    )
endif()
