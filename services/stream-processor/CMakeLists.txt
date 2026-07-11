cmake_minimum_required(VERSION 3.20)
project(stream-processor
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Dependencies
find_library(RDKAFKA_LIB rdkafka++)
find_path(RDKAFKA_INCLUDE_DIR librdkafka/rdkafkacpp.h)

find_library(HIREDIS_LIB hiredis)
find_path(HIREDIS_INCLUDE_DIR hiredis/hiredis.h)

# Gather sources and headers
file(GLOB_RECURSE LIB_SOURCES src/*.cpp)
list(FILTER LIB_SOURCES EXCLUDE REGEX "src/main\\.cpp$")
file(GLOB_RECURSE HEADERS include/*.h)

# Library target
if(LIB_SOURCES)
    add_library(${PROJECT_NAME}_lib ${LIB_SOURCES} ${HEADERS})
    target_include_directories(${PROJECT_NAME}_lib PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/third_party
        ${RDKAFKA_INCLUDE_DIR}
        ${HIREDIS_INCLUDE_DIR}
    )
    target_link_libraries(${PROJECT_NAME}_lib PUBLIC
        ${RDKAFKA_LIB}
        ${HIREDIS_LIB}
    )
endif()

# Main executable
add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party
)

if(LIB_SOURCES)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${RDKAFKA_LIB}
        ${HIREDIS_LIB}
    )
endif()

# Testing
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    set(GTEST_ROOT "/opt/homebrew")
    find_package(GTest QUIET HINTS "/opt/homebrew" NO_DEFAULT_PATH)
    if(NOT GTest_FOUND)
        find_package(GTest QUIET)
    endif()

    file(GLOB_RECURSE TEST_SOURCES test/*.cpp)
    if(TEST_SOURCES)
        add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})
        target_link_libraries(${PROJECT_NAME}_tests PRIVATE ${PROJECT_NAME}_lib)
        if(GTest_FOUND)
            target_link_libraries(${PROJECT_NAME}_tests PRIVATE GTest::gtest GTest::gtest_main)
        endif()
        add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)
    endif()
endif()
