find_package(benchmark REQUIRED)

file(GLOB_RECURSE BENCH_SRC *.cpp)

add_executable(${PROJECT_NAME}_benchmark ${BENCH_SRC})
target_link_libraries(${PROJECT_NAME}_benchmark kc benchmark)
set_property(TARGET ${PROJECT_NAME}_benchmark PROPERTY CXX_STANDARD 20)
