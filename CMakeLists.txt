cmake_minimum_required(VERSION 3.27)

project(OpenGL_Project VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE "Release")
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE sources ${CMAKE_SOURCE_DIR}/src/*.c ${CMAKE_SOURCE_DIR}/src/*.cpp)
set(includes ${CMAKE_SOURCE_DIR}/include)

add_executable(OpenGL_Project ${sources})
set_property(TARGET OpenGL_Project PROPERTY CXX_STANDARD 20)
target_include_directories(OpenGL_Project PRIVATE ${includes} ${OPENGL_INCLUDE_DIR})
target_link_libraries(OpenGL_Project PRIVATE ${CMAKE_SOURCE_DIR}/lib-universal/libglfw.3.dylib ${OPENGL_LIBRARIES})
target_link_libraries(OpenGL_Project PRIVATE "-framework CoreFoundation")
target_compile_options(OpenGL_Project PRIVATE "-Wno-deprecated-volatile")
set_target_properties(OpenGL_Project PROPERTIES 
    BUILD_RPATH ${CMAKE_SOURCE_DIR}/lib-universal 
    INSTALL_RPATH ${CMAKE_SOURCE_DIR}/lib-universal)

install(TARGETS OpenGL_Project DESTINATION ${CMAKE_SOURCE_DIR}/bin)