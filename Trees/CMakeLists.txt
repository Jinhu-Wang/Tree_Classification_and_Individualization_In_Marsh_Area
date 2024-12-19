cmake_minimum_required(VERSION 3.2)

################################################################################

set(Trees3D_VERSION_MAJOR 1)
set(Trees3D_VERSION_MINOR 0)
set(Trees3D_VERSION_PATCH 0)
set(Trees3D_VERSION_STRING ${Trees3D_VERSION_MAJOR}.${Trees3D_VERSION_MINOR}.${Trees3D_VERSION_PATCH})

project(Trees3D VERSION ${Trees3D_VERSION_STRING})

################################################################################

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message("-- Setting C++17")

################################################################################


if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    if(WIN32) # windows
        set(CMAKE_INSTALL_PREFIX "C:/Program Files/Trees/${Trees3D_VERSION_STRING}" CACHE PATH "Installation directory" FORCE)
    elseif(APPLE) # macOS
        set(CMAKE_INSTALL_PREFIX "/usr/local/Cellar/Trees/${Trees3D_VERSION_STRING}" CACHE PATH "Installation directory" FORCE)
    else() # Linux: different from tradition, let's keep everything together
        set(CMAKE_INSTALL_PREFIX "/usr/local/trees-${Trees3D_VERSION_STRING}" CACHE PATH "Installation directory" FORCE)
    endif()
endif()

if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "17.0" CACHE STRING "Minimum macOS support" FORCE)
endif()

set(Trees3D_INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib CACHE PATH "Installation directory for libraries")
set(Trees3D_INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin CACHE PATH "Installation directory for executables")
set(Trees3D_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include CACHE PATH "Installation directory for header files")
set(Trees3D_INSTALL_CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/lib/CMake/Trees3D CACHE PATH "Installation directory for CMake files")
set(Trees3D_INSTALL_RESOURCE_DIR ${CMAKE_INSTALL_PREFIX}/resources CACHE PATH "Installation directory for CMake files")

# The include directory of the exported header files
set(Trees3D_EXPORT_INCLUDE_DIR "${Trees3D_BINARY_DIR}/include")

# Setting RPATH
# See https://cmake.org/Wiki/CMake_RPATH_handling
set(CMAKE_INSTALL_RPATH ${Trees3D_INSTALL_LIB_DIR} CACHE PATH "Installation runtime directory" FORCE)

################################################################################

### Configuration
set(Trees3D_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(Trees3D_THIRD_PARTY ${Trees3D_ROOT}/3rd_party)

set(Trees3D_INCLUDE_DIR ${Trees3D_ROOT})
set(Trees3D_SOURCE_DIR ${Trees3D_ROOT})
set(Trees3D_BINARY_DIR ${CMAKE_BINARY_DIR})

### conditionally compile certain modules depending on libraries found on the system
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

### Where to put the libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Trees3D_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${Trees3D_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${Trees3D_BINARY_DIR}/lib)


################################################################################

# Build shared libraries
if(MINGW) # Workaround for using MinGW on Windows
    set(Trees3D_BUILD_SHARED_LIBS OFF)
else()
    option(Trees3D_BUILD_SHARED_LIBS "Build shared libaries" ON)
endif()
# Build tutorials
option(Trees3D_BUILD_TUTORIALS "Build Trees tutorials" OFF)
# Build documentation
option(Trees3D_BUILD_DOCUMENTATION "Build Trees documentation" OFF)
# Build tests
option(Trees3D_BUILD_TESTS "Build Trees tests programs" OFF)


################################################################################

# Discourage users to build Trees directly in its root directory
if(${CMAKE_CURRENT_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    if(NOT SAME_BUILD_AND_SOURCE_FOLDER_WARNING_ALREADY_ISSUED)
        message(WARNING "It is NOT advised to build Trees directly in its root directory!")
        set(SAME_BUILD_AND_SOURCE_FOLDER_WARNING_ALREADY_ISSUED TRUE)
    endif()
endif()

# Detect whether this is a top-level project
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(Trees3D_TOPLEVEL_PROJECT ON)
else()
    set(Easy3D_TOPLEVEL_PROJECT OFF)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################

include(GenerateExportHeader)
if(Trees3D_BUILD_SHARED_LIBS)
    set(Trees3D_LIB_TYPE SHARED)
    #Ref: Create dlls on Windows without declspec() using new CMake export all feature
    #     https://cmake.org/cmake/help/v3.4/prop_tgt/WINDOWS_EXPORT_ALL_SYMBOLS.html#prop_tgt:WINDOWS_EXPORT_ALL_SYMBOLS
    #     https://www.kitware.com/create-dlls-on-windows-without-declspec-using-new-cmake-export-all-feature/
    if(MSVC)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    endif()
else()
    set(Trees3D_LIB_TYPE STATIC)
endif()

################################################################################

# Make relative paths absolute (needed later on)
foreach(path LIB BIN INCLUDE CMAKE)
    set(var Trees3D_INSTALL_${path}_DIR)
    if(NOT IS_ABSOLUTE "${${var}}")
        set(${var} "${Trees3D_INSTALL_DIR}/${${var}}")
    endif()
endforeach()

function(add_module module headers sources private_dependencies public_dependencies)
    add_library(trees3d_${module} ${Trees3D_LIB_TYPE} ${headers} ${sources})

    if(MSVC)
        # get a clean windows
        target_compile_definitions(trees3d_${module} PUBLIC NOMINMAX WIN32_LEAN_AND_MEAN VC_EXTRALEAN _CRT_SECURE_NO_WARNINGS _CRT_SECURE_NO_DEPRECATE)
    endif()

    #Ref: Create dlls on Windows without declspec() using new CMake export all feature
    #     https://www.kitware.com/create-dlls-on-windows-without-declspec-using-new-cmake-export-all-feature/
    generate_export_header(trees3d_${module}
        EXPORT_FILE_NAME "${Trees3D_EXPORT_INCLUDE_DIR}/trees3d/${module}/export.h"
    )
    target_include_directories(trees3d_${module} PUBLIC
        "$<BUILD_INTERFACE:${Trees3D_INCLUDE_DIR}>"
        "$<BUILD_INTERFACE:${Trees3D_EXPORT_INCLUDE_DIR}>"
        "$<INSTALL_INTERFACE:${Trees3D_INSTALL_INCLUDE_DIR}>"
    )
    target_link_libraries(trees3d_${module}
        PRIVATE ${private_dependencies}
        PUBLIC ${public_dependencies}
    )
    set_target_properties(trees3d_${module} PROPERTIES
        FOLDER "trees3d"
        SOVERSION ${Trees3D_VERSION_MAJOR}
        VERSION ${Trees3D_VERSION_STRING}
    )

    # Alias target (recommended by policy CMP0028) and it looks nicer
    message(STATUS "Adding target: trees3d::${module} (trees3d_${module})")
    add_library(trees3d::${module} ALIAS trees3d_${module})
    set_property(TARGET trees3d_${module} PROPERTY EXPORT_NAME trees3d::${module})
endfunction()

function(install_module module)
    set_target_properties(trees3d_${module} PROPERTIES PUBLIC_HEADER "${${module}_headers}")
    install(TARGETS trees3d_${module}
        # IMPORTANT: Add the library to the "export-set"
        EXPORT Trees3DTargets
        RUNTIME DESTINATION "${Trees3D_INSTALL_BIN_DIR}" COMPONENT bin
        LIBRARY DESTINATION "${Trees3D_INSTALL_LIB_DIR}" COMPONENT lib
        ARCHIVE DESTINATION "${Trees3D_INSTALL_LIB_DIR}" COMPONENT lib
        PUBLIC_HEADER DESTINATION "${Trees3D_INSTALL_INCLUDE_DIR}/trees3d/${module}"
        COMPONENT dev)
endfunction()

################################################################################

add_subdirectory(3rd_party)

add_subdirectory(trees3d)

add_subdirectory(examples)

if(Trees3D_BUILD_TUTORIALS)
    add_subdirectory(tutorials)
endif()

if(Trees3D_BUILD_TESTS)
    add_subdirectory(tests)
endif()


################################################################################

if(Trees3D_BUILD_DOCUMENTATION)
    # generation of Trees3D documentation requires doxygen
    find_package(Doxygen)
    if(${DOXYGEN_FOUND})
        message(STATUS "Found Doxygen: " ${DOXYGEN_EXECUTABLE})
        # configures Doxygen
        configure_file(docs/Doxyfile.in Doxyfile @ONLY)
        add_custom_target(Documentation ALL ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating Doxygen documentation")
    else()
        message(WARNING "Building documentation requires Doxygen but Doxygen was not found. Building documentation is "
            "thus disabled. You can set 'DOXYGEN_EXECUTABLE' to the directory containing the Doxygen executable "
            "if Doxygen already exists (otherwise install Doxygen first).")
    endif()
endif()

################################################################################

install(DIRECTORY
    "${Trees3D_ROOT}/resources/fonts"
    "${Trees3D_ROOT}/resources/shaders"
    "${Trees3D_ROOT}/resources/colormaps"
    "${Trees3D_ROOT}/resources/textures"
    DESTINATION
    "${Trees3D_INSTALL_RESOURCE_DIR}")

install(FILES
    "${Trees3D_ROOT}/ReadMe.md"
    "${Trees3D_ROOT}/LICENSE"
    DESTINATION
    "${CMAKE_INSTALL_PREFIX}"
)

install(DIRECTORY
    "${Trees3D_EXPORT_INCLUDE_DIR}/trees3d"
    DESTINATION
    "${Trees3D_INSTALL_INCLUDE_DIR}")

################################################################################

### Summary
message(STATUS "****************************************************************************")
message(STATUS "    Package       : Trees3D")
message(STATUS "    Version       : ${Trees3D_VERSION_STRING}")

message(STATUS "    Generator     : ${CMAKE_GENERATOR}")
if(CMAKE_CONFIGURATION_TYPES)
    message(STATUS "    Configuration : ${CMAKE_CONFIGURATION_TYPES}")
elseif(CMAKE_BUILD_TYPE)
    message(STATUS "    Build type    : ${CMAKE_BUILD_TYPE}")

endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "    System        : 64-bit ${CMAKE_SYSTEM_NAME}")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(STATUS "    System        : 32-bit ${CMAKE_SYSTEM_NAME}")
    message(FATAL_ERROR "32-bit not supported! You should specify CMake option '-DCMAKE_GENERATOR_PLATFORM=x64'")
endif()
if(APPLE)
    message(STATUS "    Minimum macOS support : ${CMAKE_OSX_DEPLOYMENT_TARGET}")
    mark_as_advanced(CMAKE_OSX_ARCHITECTURES)
endif()

message(STATUS "----------------------------------------------------------------------------")

message(STATUS "    Build shared libraries :  ${Trees3D_BUILD_SHARED_LIBS}")
message(STATUS "    Build tutorials        :  ${Trees3D_BUILD_TUTORIALS}")
message(STATUS "    Build documentation    :  ${Trees3D_BUILD_DOCUMENTATION}")
message(STATUS "    Build tests            :  ${Trees3D_BUILD_TESTS}")

message(STATUS "----------------------------------------------------------------------------")

message(STATUS "    Build directory        :  ${Trees3D_BINARY_DIR}")
message(STATUS "    Installation directory :  ${CMAKE_INSTALL_PREFIX}")

message(STATUS "****************************************************************************")


