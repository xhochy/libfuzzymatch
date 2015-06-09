project(libfuzzymatch)
cmake_minimum_required( VERSION 2.8.6 )
#set( CMAKE_MODULE_PATH  "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules" )

include(GNUInstallDirs)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
        "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif(NOT CMAKE_BUILD_TYPE)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag( "-std=c++11" CXX11_FOUND )
check_cxx_compiler_flag( "-std=c++0x" CXX0X_FOUND )
check_cxx_compiler_flag( "-stdlib=libc++" LIBCPP_FOUND )
if(CXX11_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(CXX0X_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(FATAL_ERROR "${CMAKE_CXX_COMPILER} does not support C++11, please use a
    different compiler")
endif()
if(LIBCPP_FOUND AND APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

add_subdirectory(src)
add_subdirectory(benchmark)

configure_file(LibFuzzyMatchBuildTreeSettings.cmake.in
    "${PROJECT_BINARY_DIR}/LibFuzzyMatchBuildTreeSettings.cmake"
    @ONLY)
configure_file(LibFuzzyMatchConfig.cmake.in
    "${PROJECT_BINARY_DIR}/LibFuzzyMatchConfig.cmake"
    @ONLY)
configure_file(LibFuzzyMatchConfigVersion.cmake.in
    "${PROJECT_BINARY_DIR}/LibFuzzyMatchConfigVersion.cmake"
    @ONLY)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/LibFuzzyMatchConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/LibFuzzyMatchConfigVersion.cmake
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/LibFuzzyMatch
    )
