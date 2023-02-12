cmake_minimum_required(VERSION 3.5)

project(Parallels VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
  ${CMAKE_SOURCE_DIR}/aco
  ${CMAKE_SOURCE_DIR}/graph
  ${CMAKE_SOURCE_DIR}/interface
)

set(HEADERS
  interface/interface.h
  graph/s21_graph.h
  aco/aco.h
)

set(SOURCES
  interface/interface.cc
  graph/s21_graph.cc
  aco/aco.cc
  main.cc
)

add_executable(
  ${PROJECT_NAME}
  ${HEADERS}
  ${SOURCES}
)
