set (LIBS 
        jsoncpp
        pthread
        spdlog)

file (GLOB_RECURSE SRC kc/*.cpp)

add_library(${PROJECT_NAME} STATIC ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})

