cmake_minimum_required (VERSION 3.0)

message (STATUS "logger examples:")

# simple
project (logger_simple)
message (STATUS "* ${PROJECT_NAME}")

add_executable (${PROJECT_NAME} simple.cpp)
SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY FOLDER examples)
target_link_libraries (${PROJECT_NAME} logger)
target_include_directories (${PROJECT_NAME} PUBLIC logger)

# disabled
project (logger_disabled)
message (STATUS "* ${PROJECT_NAME}")

add_executable (${PROJECT_NAME} disabled.cpp)
SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY FOLDER examples)
target_link_libraries (${PROJECT_NAME} logger)
#target_include_directories (${PROJECT_NAME} PUBLIC logger)
