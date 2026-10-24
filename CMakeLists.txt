cmake_minimum_required(VERSION 3.16)
project(clab LANGUAGES C)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/uthash/src/uthash.h")
    message(FATAL_ERROR "uthash is missing. Run: git submodule update --init --recursive")
endif()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED YES)
set(CMAKE_C_EXTENSIONS NO)

function(clab_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

add_library(contacts STATIC src/contacts.c)
target_include_directories(contacts PUBLIC src third_party/uthash/src)
clab_warnings(contacts)

add_executable(uthash_demo src/main.c)
target_link_libraries(uthash_demo PRIVATE contacts)
clab_warnings(uthash_demo)

include(CTest)
if(BUILD_TESTING)
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/Unity/src/unity.c")
        message(FATAL_ERROR "Unity is missing. Run: git submodule update --init --recursive")
    endif()
    add_subdirectory(third_party/Unity EXCLUDE_FROM_ALL)
    add_executable(test_contacts tests/test_contacts.c)
    target_link_libraries(test_contacts PRIVATE contacts unity)
    clab_warnings(test_contacts)
    add_test(NAME contacts_unit_tests COMMAND test_contacts)

    add_test(NAME uthash_demo_runs COMMAND uthash_demo)
    set_tests_properties(uthash_demo_runs PROPERTIES
        PASS_REGULAR_EXPRESSION "After cleanup: 0"
    )
endif()
