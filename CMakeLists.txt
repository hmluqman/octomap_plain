cmake_minimum_required(VERSION 2.8)

project(octomap_plain)

find_package(octomap REQUIRED)
find_package(PCL REQUIRED)

include_directories(
    include
    ${PCL_INCLUDE_DIRS}
    ${OCTOMAP_INCLUDE_DIRS}
    )
    
add_executable(${PROJECT_NAME} "octomap_pcd.cpp")
target_link_libraries(${PROJECT_NAME}
    ${PCL_LIBRARIES}  
    ${OCTOMAP_LIBRARIES}
)

add_library(kde_octomap
  src/octomap_kernel_density_estimation.cpp
)

target_link_libraries(kde_octomap
  ${OCTOMAP_LIBRARIES}
)

add_executable(automated "octomap_automated_data.cpp")
target_link_libraries(automated
    kde_octomap
    ${PCL_LIBRARIES}  
    ${OCTOMAP_LIBRARIES}
)

add_executable(just_octomap "just_octomap.cpp")
target_link_libraries(just_octomap
    ${OCTOMAP_LIBRARIES}
)

add_executable(ftest "function_testing.cpp")

