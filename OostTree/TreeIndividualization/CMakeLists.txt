
cmake_minimum_required(VERSION 3.0)

project(${PROJECT_NAME})

set(${PROJECT_NAME}_HEADERS
    FoxTree.h
    nanoflann.hpp)


set(${PROJECT_NAME}_SOURCES
    FoxTree.cpp)


add_library(${PROJECT_NAME}_INDIVIDUALIZATION STATIC ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES})

