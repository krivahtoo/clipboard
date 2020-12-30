cmake_minimum_required(VERSION 3.0.0)

project(clipboard VERSION 0.1.0)

add_executable(clipboard clipboard.cpp)

add_subdirectory(clip)

target_link_libraries(clipboard clip)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
