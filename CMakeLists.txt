cmake_minimum_required(VERSION 3.16)

project(global-illu)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)

find_package(Qt5Widgets)
find_package(Threads)

find_package(OpenMP REQUIRED)

if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif() 

set(SOURCES main.cpp src/entities.cpp)
set(HEADERS include/gui.h include/image.h include/ray.h include/entities.h include/camera.h include/raytracer.h include/viewer.h include/octree.h include/bbox.h include/material.h)


if (MSVC)
add_definitions(-DNOMINMAX)
endif()

add_executable(global-illu ${SOURCES} ${HEADERS})
include_directories(global-illu include 3rd_party)

option(CLANG_FORMAT_TARGET "automatic clang format" OFF)
if(CLANG_FORMAT_TARGET)
    include(ClangFormat)
    add_clang_format(TARGET global-illu STYLE "LLVM" FILES ${SOURCES} ${HEADERS} -fopenmp)
endif(CLANG_FORMAT_TARGET)


target_link_libraries(global-illu Qt5::Widgets ${CMAKE_THREAD_LIBS_INIT} ${OpenMP_CXX_LIBRARIES})

