set(GAME_LIB_NAME ForlornMelodyGame)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    message(STATUS "Added parallel build arguments to CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
endif()

include_directories(./
        ../
        ${GLM_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIR}
        ${GLFW_INCLUDE_DIR})

set(FORLORN_MELODY_GAME_DEPENDENCY_LIBS
        GameEngineBase
        GameEngineCustom)

file(GLOB SOURCES *.cpp)
add_library(${GAME_LIB_NAME} ${SOURCES})
target_link_libraries(${GAME_LIB_NAME} ${FORLORN_MELODY_GAME_DEPENDENCY_LIBS})