
cmake_minimum_required(VERSION 3.0)



# Set the version.
set(LiDARTree_MAJOR_VERSION 0)
set(LiDARTree_MINOR_VERSION 1)
set(LiDARTree_PATCH_VERSION 1)
set(LiDARTree_VERSION ${LiDARTree_MAJOR_VERSION}.${LiDARTree_MINOR_VERSION}.${LiDARTree_PATCH_VERSION})


project(LiDARTree VERSION ${LiDARTree_VERSION} LANGUAGES CXX)


add_subdirectory(TreeClassification)

add_subdirectory(TreeIndividualization)

