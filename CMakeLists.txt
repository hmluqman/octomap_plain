cmake_minimum_required(VERSION 2.8)

project(octomap_plain)

find_package(octomap REQUIRED)
find_package(PCL REQUIRED)

include_directories(
    ${PCL_INCLUDE_DIRS}
    ${OCTOMAP_INCLUDE_DIRS}
    )
    
if(CMAKE_VERSION VERSION_LESS "3.1")
    if(CMAKE_COMPILER_IS_GNUCXX)
      include(CheckCXXCompilerFlag)
      check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
      check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
      if(COMPILER_SUPPORTS_CXX11)
    add_definitions("-std=c++11")
      elseif(COMPILER_SUPPORTS_CXX0X)
    add_definitions("-std=c++0x")
      else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
      endif()
    endif()
  else()
    set(CMAKE_CXX_STANDARD 11)
endif()

    
add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME}
    ${PCL_LIBRARIES}  
    ${OCTOMAP_LIBRARIES}
)

