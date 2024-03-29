cmake_minimum_required(VERSION 3.10)
project(kc)

set(GCC_COVERAGE_COMPILE_FLAGS "--coverage -O0 -fprofile-arcs -ftest-coverage -fno-elide-constructors -fno-inline")
set(GCC_COVERAGE_LINK_FLAGS    "-lgcov")

find_package(Threads)

if (ENABLE_COVERAGE)
    message("$$ Triggering build with coverage support")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
    string(REPLACE "-O3" "" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
else()
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O2")
endif()

add_definitions(-DSPDLOG_COMPILED_LIB=1)

add_subdirectory(3rdparty/stb)
add_subdirectory(src)

if (ENABLE_UNIT_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if (ENABLE_BENCHMARKS)
    add_subdirectory(3rdparty/benchmark)
    add_subdirectory(bench)
endif()