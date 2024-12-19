
cmake_minimum_required(VERSION 3.0)

project(${PROJECT_NAME})

set(${PROJECT_NAME}_HEADERS
    FoxTree.h
    nanoflann.hpp)


set(${PROJECT_NAME}_SOURCES
    FoxTree.cpp
    Source.cpp)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES})

target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_DEPRECATE)
add_library(${PROJECT_NAME}_INDIVIDUALIZATION STATIC ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES})

