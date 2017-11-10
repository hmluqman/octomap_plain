cmake_minimum_required(VERSION 2.8)

project(octomap_plain)

find_package(octomap REQUIRED)
find_package(PCL REQUIRED)

include_directories(
    ${PCL_INCLUDE_DIRS}
    ${OCTOMAP_INCLUDE_DIRS}
    )
    
add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME}
    ${PCL_LIBRARIES}  
    ${OCTOMAP_LIBRARIES}
)

