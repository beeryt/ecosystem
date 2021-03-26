cmake_minimum_required(VERSION 3.16)
project(Ecosystem
  VERSION 0.1.0
  DESCRIPTION "")

add_subdirectory(libs/gfx)

add_executable(main src/main.cpp)
target_include_directories(main PUBLIC include)
target_link_libraries(main PRIVATE gfx)

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
