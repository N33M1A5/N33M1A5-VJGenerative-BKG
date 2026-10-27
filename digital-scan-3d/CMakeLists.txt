cmake_minimum_required(VERSION 3.20)
project(N33M1A5DigitalScan3D LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

find_package(GLEW CONFIG REQUIRED)
include(FetchContent)
FetchContent_Declare(ffgl GIT_REPOSITORY https://github.com/resolume/ffgl.git GIT_TAG master)
FetchContent_MakeAvailable(ffgl)

file(GLOB FFGL_SRC CONFIGURE_DEPENDS
  ${ffgl_SOURCE_DIR}/source/lib/ffgl/*.cpp
  ${ffgl_SOURCE_DIR}/source/lib/ffglex/*.cpp
  ${ffgl_SOURCE_DIR}/source/lib/ffglquickstart/*.cpp)

add_library(N33M1A5DigitalScan3D SHARED src/DigitalScan3D.cpp ${FFGL_SRC})
target_include_directories(N33M1A5DigitalScan3D PRIVATE
  ${ffgl_SOURCE_DIR}/source/lib
  ${ffgl_SOURCE_DIR}/source/lib/ffglquickstart)
target_compile_definitions(N33M1A5DigitalScan3D PRIVATE NOMINMAX _CRT_SECURE_NO_WARNINGS)
target_link_libraries(N33M1A5DigitalScan3D PRIVATE opengl32 GLEW::GLEW)
set_target_properties(N33M1A5DigitalScan3D PROPERTIES OUTPUT_NAME "N33M1A5_DigitalScan3D")
