project(pairwise_levenshtein_benchmark)
cmake_minimum_required(VERSION 2.8.1 FATAL_ERROR)
set(CMAKE_MODULE_PATH  "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

# Activate C++11 support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(FATAL_ERROR "${CMAKE_CXX_COMPILER} does not have C++11 support")
endif()


# Build the index
add_executable(build_index build_index.cpp)
set_target_properties(build_index
    PROPERTIES
    RUNTIME_OUTPUT_NAME build-index
    )

# Query the index, the exciting stuff
add_executable(query_index query_index.cpp)
set_target_properties(query_index
    PROPERTIES
    RUNTIME_OUTPUT_NAME query-index
    )

