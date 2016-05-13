set(GAME_LIB_NAME ForlornMelodyGame)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    message(STATUS "Added parallel build arguments to CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
endif()

if (WIN32)
    message(STATUS "Using Windows config for ForlornMelodyGame")
    set(GLEW_LIBRARY "glew32")
    set(GLFW_LIBRARY "glfw3")
    set(GLEW_INCLUDE_DIR "C:\\Program Files (x86)\\glew\\include")
    set(GLFW_INCLUDE_DIR "C:\\Program Files (x86)\\glfw\\include")
    set(GLM_INCLUDE_DIR "C:\\Program Files (x86)\\glm\\include")
elseif (APPLE)
    message(STATUS "Using Apple config for ForlornMelodyGame")
    find_package(GLEW REQUIRED)
    find_package(GLFW REQUIRED)
    set(GLM_INCLUDE_DIR "/usr/local/include")
endif(WIN32)

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