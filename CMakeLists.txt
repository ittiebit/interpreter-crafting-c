cmake_minimum_required(VERSION 3.15)
project(clox_project C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm -DDEBUG")

file(GLOB_RECURSE MAIN_SOURCES
    clox/*.c
    utils/*.c
)

add_executable(main ${MAIN_SOURCES})

# Tests
target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/clox
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)
add_executable(keywords-test
    tests/keywords/keyword-test.c
    clox/scanner.c
    utils/hashmap.c
    clox/clox.c
)
target_include_directories(keywords-test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/clox
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/keywords
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/expressions
)
target_link_libraries(keywords-test m)
