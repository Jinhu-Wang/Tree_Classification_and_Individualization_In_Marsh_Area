# All available Trees3D libraries
set(CONF_Trees3D_COMPONENTS)

# Add all targets to the build-tree export set
set(EXPORT_TARGET_SET)


add_subdirectory(tree_classification)
list(APPEND CONF_Trees3D_COMPONENTS tree_classification)
list(APPEND EXPORT_TARGET_SET trees3d_tree_classification)

add_subdirectory(tree_individualization)
list(APPEND CONF_Trees3D_COMPONENTS tree_individualization)
list(APPEND EXPORT_TARGET_SET trees3d_tree_individualization)

export(TARGETS
        ${EXPORT_TARGET_SET}
        FILE "${Trees3D_BINARY_DIR}/Tree3DTargets.cmake")

export(PACKAGE Trees3D)

# ... for the build tree
set(CONF_Trees3D_INCLUDE_DIRS "${Trees3D_INCLUDE_DIR}")
set(CONF_Trees3D_LIBRARY_DIRS "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
set(CONF_Trees3D_VERSION ${Trees3D_VERSION_STRING})
configure_file(../cmake/Trees3DConfig.cmake.in
        "${Trees3D_BINARY_DIR}/Trees3DConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_Trees3D_INCLUDE_DIRS "${Trees3D_INSTALL_INCLUDE_DIR}")
set(CONF_Trees3D_LIBRARY_DIRS "${Trees3D_INSTALL_LIB_DIR}")
configure_file(../cmake/Trees3DConfig.cmake.in
        "${Trees3D_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/Trees3DConfig.cmake" @ONLY)
# ... for both
configure_file(../cmake/Trees3DConfigVersion.cmake.in
        "${Trees3D_BINARY_DIR}/Trees3DConfigVersion.cmake" @ONLY)

# Install the Trees3DConfig.cmake and Trees3DConfigVersion.cmake
install(FILES
        "${Trees3D_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Trees3DConfig.cmake"
        "${Trees3D_BINARY_DIR}/Trees3DConfigVersion.cmake"
        DESTINATION "${Trees3D_INSTALL_CMAKE_DIR}" COMPONENT dev)

# Install the export set for use with the install-tree
install(EXPORT Trees3DTargets DESTINATION
        "${Trees3D_INSTALL_CMAKE_DIR}" COMPONENT dev)

