cmake_minimum_required(VERSION 3.0)

project(${PROJECT_NAME})

set(${PROJECT_NAME}_HEADERS
    BasicStructure.h
    nanoflann.hpp
    TreeClassifier.h)


set(${PROJECT_NAME}_SOURCES
    TreeClassifier.cpp)

add_library(${PROJECT_NAME}_CLASSIFICATION STATIC ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES})

