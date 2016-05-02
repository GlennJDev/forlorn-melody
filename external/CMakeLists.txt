if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    message(STATUS "Added parallel build arguments to CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
endif()

include_directories(SOIL/src)

### Simple OpenGL Image Library ###

file(GLOB SOIL_SOURCES SOIL/src/*.c)

add_library(SOIL STATIC ${SOIL_SOURCES})