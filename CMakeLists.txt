cmake_minimum_required (VERSION 3.5.1)
project(Markovpp)

if(WIN32)
   set(CMAKE_CXX_FLAGS "-std=c++11 -W4 -EHsc")
elseif(UNIX)
   set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Werror -pedantic")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(COPY demo/example.txt DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )

set( CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build Type")
set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")

set( LIB_TYPE "STATIC" CACHE STRING "Build as static or shared lib")
set_property(CACHE LIB_TYPE PROPERTY STRINGS "STATIC" "SHARED")

if((${LIB_TYPE} STREQUAL "SHARED") AND WIN32)
   set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(markovpp ${LIB_TYPE} src/Markovpp.cpp)

add_executable(demo demo/Demo.cpp )
target_link_libraries( demo markovpp )
