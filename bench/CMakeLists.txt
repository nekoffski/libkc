file(GLOB_RECURSE BENCH_SRC *.cpp)

add_definitions(-DSPDLOG_ACTIVE_LEVEL=6)
add_executable(${PROJECT_NAME}_benchmark ${BENCH_SRC})

target_link_libraries(${PROJECT_NAME}_benchmark kc benchmark::benchmark benchmark::benchmark_main)
set_property(TARGET ${PROJECT_NAME}_benchmark PROPERTY CXX_STANDARD 20)
