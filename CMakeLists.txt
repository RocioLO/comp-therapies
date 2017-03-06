cmake_minimum_required(VERSION 3.6)

project(ctlab)

find_package(MITK REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

mitk_create_executable(${PROJECT_NAME} DEPENDS MitkQtWidgetsExt)

