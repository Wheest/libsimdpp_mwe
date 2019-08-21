cmake_minimum_required (VERSION 3.8)
project (SIMD_MWE)

add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/libsimdpp/ build)
list(APPEND Mwe_INCLUDE_DIRS PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/libsimdpp/)
include_directories(${Mwe_INCLUDE_DIRS})

add_executable(simd_mwe src/main.cpp)
