cmake_minimum_required(VERSION 3.20)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

project(${PROJECT_NAME})

set(${PROJECT_NAME}_HEADERS
    )

set(${PROJECT_NAME}_SOURCES
    main.cpp)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "examples")

target_include_directories(${PROJECT_NAME} PRIVATE ${Mie_INCLUDE_DIR})

target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_DEPRECATE)

target_link_libraries(${PROJECT_NAME} PRIVATE 3rd_lastools trees3d::tree_classification)
