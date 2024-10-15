set (LIBS jsoncpp pthread spdlog stb fmt)

file (GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(${PROJECT_NAME} STATIC ${SRC})

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../ ABSOLUTE)
target_include_directories(${PROJECT_NAME} PUBLIC ${PARENT_DIR})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})
