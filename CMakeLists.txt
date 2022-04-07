cmake_minimum_required(VERSION 3.21)
project(MDLLib)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG  "-g")

add_library(MDLLib SHARED src/MDLLib.cpp src/CStudioHdr.cpp src/CStudioHdr.h src/source/datatypes.h)

add_executable(MDLLibTest src/test.cpp)
target_link_libraries(MDLLibTest MDLLib)