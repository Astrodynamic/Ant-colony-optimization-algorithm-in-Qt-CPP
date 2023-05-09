cmake_minimum_required(VERSION 3.15)

project(ACOAlgorithms VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
  ${CMAKE_SOURCE_DIR}/aco
  ${CMAKE_SOURCE_DIR}/graph
  ${CMAKE_SOURCE_DIR}/interface
)

set(HEADERS
  ${CMAKE_SOURCE_DIR}/interface/interface.h
  ${CMAKE_SOURCE_DIR}/graph/graph.h
  ${CMAKE_SOURCE_DIR}/aco/aco.h
)

set(SOURCES
  ${CMAKE_SOURCE_DIR}/interface/interface.cc
  ${CMAKE_SOURCE_DIR}/graph/graph.cc
  ${CMAKE_SOURCE_DIR}/aco/aco.cc
)

set(PROJECT_SOURCES
  ${HEADERS}
  ${SOURCES}
  ${CMAKE_SOURCE_DIR}/main.cc
)

add_executable(
  ${PROJECT_NAME}
  ${PROJECT_SOURCES}
)

target_compile_options(
  ${PROJECT_NAME}
  PRIVATE
  -Wall
  -Werror
  -Wextra
  -Wpedantic
  -fsanitize=address
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    -fsanitize=address
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Add cppcheck
find_program(CPPCHECK cppcheck)

if(CPPCHECK)
  message(STATUS "cppcheck found: ${CPPCHECK}")
  list(
    APPEND CPPCHECK_ARGS
    "--enable=all"
    "--inconclusive"
    "--quiet"
    "--language=c++"
    "--std=c++17"
    "--suppress=unusedFunction"
    "--suppress=missingInclude"
    "--suppress=unusedStructMember"
    "--suppress=variableScope"
    "--suppress=functionStatic"
    "--suppress=*:${PROJECT_SOURCE_DIR}/lib/*"
  )
  add_custom_target(
    cppcheck
    COMMAND ${CPPCHECK} ${CPPCHECK_ARGS} ${HEADERS} ${SOURCES}
  )
else()
  message(STATUS "cppcheck not found")
endif()

# Add clang-format
find_program(CLANG_FORMAT clang-format)

if(CLANG_FORMAT)
  message(STATUS "clang-format found: ${CLANG_FORMAT}")
  list(
    APPEND CLANG_ARGS
    "-i"
    "-style=Google"
    "--verbose"
  )
  add_custom_target(
    clang-format
    COMMAND ${CLANG_FORMAT} ${CLANG_ARGS} ${HEADERS} ${SOURCES}
  )
else()
  message(STATUS "clang-format not found")
endif()
