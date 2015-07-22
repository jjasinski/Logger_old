cmake_minimum_required (VERSION 3.0)

project (logger)
message (STATUS "* ${PROJECT_NAME}")


file (GLOB test_main "main.cpp")
file (GLOB headers "*.hpp")

file (GLOB logger_headers "logger/*.hpp")
file (GLOB logger_sources "logger/*.cpp")

source_group ("Header Files\\logger" FILES ${logger_headers})
source_group ("Source Files\\logger" FILES ${logger_sources})

include_directories (.)

add_executable (${PROJECT_NAME} ${test_main})
#add_library (${PROJECT_NAME} ${headers} ${logger_headers} ${logger_sources})