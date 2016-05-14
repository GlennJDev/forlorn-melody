set(GAME_ENGINE_BASE_LIB_NAME GameEngineBase)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    message(STATUS "Added parallel build arguments to CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
endif()

find_package(OpenGL REQUIRED)

if (WIN32)
    message(STATUS "Using Windows config for GameEngineBase")
    set(GLEW_LIBRARY "glew32")
    set(GLFW_LIBRARY "glfw3")
    set(GLEW_INCLUDE_DIR "C:\\Program Files (x86)\\glew\\include")
    set(GLFW_INCLUDE_DIR "C:\\Program Files (x86)\\glfw\\include")
    set(GLM_INCLUDE_DIR "C:\\Program Files (x86)\\glm\\include")
elseif (APPLE)
    message(STATUS "Using Apple config for GameEngineBase")
    find_package(GLEW REQUIRED)
    find_package(GLFW REQUIRED)
    set(GLM_INCLUDE_DIR "/usr/local/include")
endif(WIN32)

message(STATUS "Using glew library: ${GLEW_LIBRARY}")
message(STATUS "Using glfw library: ${GLFW_LIBRARY}")

include_directories(.
        ../
        ../external/SOIL/src
        ${GLM_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIR}
        ${GLFW_INCLUDE_DIR})

set(GAME_ENGINE_BASE_DEPENDENCY_LIBS ${OPENGL_LIBRARY}
        ${GLEW_LIBRARY}
        ${GLFW_LIBRARY}
        SOIL)

file(GLOB SOURCES *.cpp)
add_library(${GAME_ENGINE_BASE_LIB_NAME} STATIC ${SOURCES})
target_link_libraries(${GAME_ENGINE_BASE_LIB_NAME} ${GAME_ENGINE_BASE_DEPENDENCY_LIBS})