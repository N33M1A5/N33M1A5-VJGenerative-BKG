cmake_minimum_required(VERSION 3.20)
project(N33M1A5DigitalScan3D LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include(FetchContent)
FetchContent_Declare(ffgl
  GIT_REPOSITORY https://github.com/resolume/ffgl.git
  GIT_TAG master)
FetchContent_GetProperties(ffgl)
if(NOT ffgl_POPULATED)
  FetchContent_Populate(ffgl)
endif()

if(NOT DEFINED ENV{VCPKG_ROOT})
  message(FATAL_ERROR "VCPKG_ROOT not defined; install GLEW with vcpkg before configuring.")
endif()
set(VCPKG_INSTALLED "$ENV{VCPKG_ROOT}/installed/x64-windows")
find_path(GLEW_INCLUDE_DIR GL/glew.h HINTS "${VCPKG_INSTALLED}/include" REQUIRED)
find_library(GLEW_LIBRARY NAMES glew32 glew32s HINTS "${VCPKG_INSTALLED}/lib" REQUIRED)

file(GLOB FFGL_SRC CONFIGURE_DEPENDS
  "${ffgl_SOURCE_DIR}/source/lib/ffgl/*.cpp"
  "${ffgl_SOURCE_DIR}/source/lib/ffglex/*.cpp"
  "${ffgl_SOURCE_DIR}/source/lib/ffglquickstart/*.cpp")

add_library(N33M1A5DigitalScan3D SHARED src/DigitalScan3D.cpp ${FFGL_SRC})
target_include_directories(N33M1A5DigitalScan3D PRIVATE
  "${ffgl_SOURCE_DIR}/source/lib"
  "${ffgl_SOURCE_DIR}/source/lib/ffglquickstart"
  "${GLEW_INCLUDE_DIR}")
target_compile_definitions(N33M1A5DigitalScan3D PRIVATE NOMINMAX _CRT_SECURE_NO_WARNINGS)
target_link_libraries(N33M1A5DigitalScan3D PRIVATE opengl32 "${GLEW_LIBRARY}")
set_target_properties(N33M1A5DigitalScan3D PROPERTIES OUTPUT_NAME "N33M1A5_DigitalScan3D")
