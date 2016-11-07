
#Require a recent version of cmake

cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)



# This project is C++ based.

project(matrix_matrix_hpx_single_file CXX)



# This adds the HPX cmake configuration directory to the search path.

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    
    ${HPX_ROOT}/share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules)



# Instruct cmake to find the HPX settings

find_package(HPX)



include_directories(${HPX_INCLUDE_DIR})

link_directories(${HPX_LIBRARY_DIR})



add_hpx_executable(matrix_matrix_hpx_single_file_exe

              ESSENTIAL

              SOURCES main.cpp
              
                       COMPONENT_DEPENDENCIES iostreams)