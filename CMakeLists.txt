cmake_minimum_required(VERSION 3.16)
project(Ecosystem
  VERSION 0.1.0
  DESCRIPTION "")
add_compile_options(-g3 -Wall -Wextra -O0)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(libs)

set(MAIN_SOURCES
  src/main.cpp
  src/camera.cpp
  src/grid.cpp
  src/random.cpp
  src/circlemesh.cpp
)

set(MAIN_LIBS
  glm::glm
  spdlog::spdlog
)

add_executable(main-sdl ${MAIN_SOURCES})
target_include_directories(main-sdl PRIVATE include)
target_link_libraries(main-sdl PRIVATE gfx-sdl ${MAIN_LIBS})

add_executable(main-glfw ${MAIN_SOURCES})
target_include_directories(main-glfw PRIVATE include)
target_link_libraries(main-glfw PRIVATE gfx-glfw ${MAIN_LIBS})

option(BUILD_DOC "Build documentation" OFF)
find_package(Doxygen)
if (BUILD_DOC AND DOXYGEN_FOUND)
  # Configure Doxyfile
  set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

  add_custom_target(docs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating documentation with Doxygen"
    VERBATIM)
elseif(BUILD_DOC)
  message("Doxygen needs to be installed to generate the documentation")
endif()
