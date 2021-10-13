set (LIBS 
        jsoncpp
        pthread
        spdlog
        )

file (GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/kc/*.cpp)
list(REMOVE_ITEM SRC  ${CMAKE_CURRENT_SOURCE_DIR}/kc/model/generator/main.cpp)

add_library(${PROJECT_NAME} STATIC ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})

add_executable(modelgen ${CMAKE_CURRENT_SOURCE_DIR}/kc/model/generator/main.cpp)
target_include_directories(modelgen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET modelgen PROPERTY CXX_STANDARD 20)
target_link_libraries(modelgen PUBLIC ${LIBS})
