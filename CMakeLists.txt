cmake_minimum_required(VERSION 3.3)
project(ForlornMelody)

# Specify the list of directories to search for cmake modules.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

# Detect compiler version
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    MESSAGE(STATUS "64 bits compiler detected" )
    SET( EX_PLATFORM 64 )
    SET( EX_PLATFORM_NAME "x64" )
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    MESSAGE(STATUS "32 bits compiler detected" )
    SET( EX_PLATFORM 32 )
    SET( EX_PLATFORM_NAME "Win32" )
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    message(STATUS "Added parallel build arguments to CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    message(STATUS "Added compiler flag -std=c++11")
endif()

add_subdirectory(external)
add_subdirectory(GameEngineBase)
add_subdirectory(GameEngineCustom)
add_subdirectory(Game)

include_directories(.
        ${GLM_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIR}
        ${GLFW_INCLUDE_DIR})

if (WIN32)
    link_directories("C:\\Program Files (x86)\\glew\\lib\\${EX_PLATFORM_NAME}"
            "C:\\Program Files (x86)\\glfw\\lib\\${EX_PLATFORM_NAME}")
elseif(APPLE)
    link_directories("/usr/local/lib")
endif()

set(SOURCE_FILES main.cpp)
add_executable(ForlornMelody ${SOURCE_FILES})
target_link_libraries(ForlornMelody ForlornMelodyGame)

# Copy shaders to build dir
add_custom_command(TARGET ForlornMelody POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:ForlornMelody>/Resources)