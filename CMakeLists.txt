cmake_minimum_required (VERSION 3.5.1)
project(Markovpp)

if(WIN32)
   set(CMAKE_CXX_FLAGS "-std=c++11 -W4 -EHsc")
elseif(UNIX)
   set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Werror -pedantic")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(COPY example.txt DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )

set( SOURCES "include/Markovpp.cpp" )
set( SOURCES ${SOURCES} "include/Markovpp.h")
add_executable(demo Demo.cpp ${SOURCES} )
