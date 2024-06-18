cmake_minimum_required(VERSION 3.19)

project("rice-runtime" C)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GLIB REQUIRED glib-2.0)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/export
	${GLIB_INCLUDE_DIRS}
)

link_directories(${GLIB_LIBRARY_DIRS})

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.c)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/**/*.h)
file(GLOB EXPORT ${CMAKE_CURRENT_SOURCE_DIR}/export/**/*.h)

add_library(${CMAKE_PROJECT_NAME} ${HEADERS} ${SRC})
target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h")

target_link_libraries(${CMAKE_PROJECT_NAME} ${GLIB_LIBRARIES})


install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)