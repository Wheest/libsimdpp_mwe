cmake_minimum_required (VERSION 3.8)
project (SIMD_MWE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O0 -g3 -fno-slp-vectorize -march=native")
# set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3")
# set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g3")

add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/libsimdpp/ build)
list(APPEND Mwe_INCLUDE_DIRS PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/libsimdpp/)
include_directories(${Mwe_INCLUDE_DIRS})

add_executable(simd_mwe src/main.cpp)
