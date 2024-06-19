cmake_minimum_required(VERSION 3.19)

project("rice-runtime" C)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(LIBARCHIVE REQUIRED libarchive)


include_directories(
	${GLIB_INCLUDE_DIRS}
	${LIBARCHIVE_INCLUDE_DIRS})

link_directories(${GLIB_LIBRARY_DIRS} ${LIBARCHIVE_LIBRARY_DIRS})

file(GLOB_RECURSE SRC_COMMON ${CMAKE_CURRENT_SOURCE_DIR}/src/common/*.c)
file(GLOB_RECURSE SRC_LINUX ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/*.c)

set(SRC ${SRC_COMMON})

if (UNIX)
	set(SRC ${SRC} ${SRC_LINUX})
endif()

add_library(${CMAKE_PROJECT_NAME} ${SRC})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/include)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h)

target_link_libraries(${CMAKE_PROJECT_NAME}
	${GLIB_LIBRARIES}
	${LIBARCHIVE_LIBRARIES})