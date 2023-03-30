cmake_minimum_required(VERSION 3.0)

project(libepeg)
set (VERSION "0.9.2")

option(BUILD_STATIC_LIB "Build static library" ON)
option(BUILD_SHARED_LIB "Build shared library" ON)


# Build configuration
################################################################################

set(CMAKE_POSITION_INDEPENDENT_CODE ON)


# Platform specific build configuration
################################################################################

if(MSVC)
    message(STATUS "Building for windows")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()


# Source files
################################################################################


file(GLOB_RECURSE libepeg_SRC src/lib/*.c)
list(SORT libepeg_SRC)


set(libepeg_HEADER src/lib/Epeg.h)

include(CheckIncludeFile)

if(DEFINED JPEG_INCLUDE_DIRECTORY)
    include_directories(BEFORE SYSTEM "${JPEG_INCLUDE_DIRECTORY}")
    list(APPEND CMAKE_REQUIRED_INCLUDES ${JPEG_INCLUDE_DIRECTORY})
endif()

if(DEFINED EXIF_INCLUDE_DIRECTORY)
    include_directories(BEFORE SYSTEM "${EXIF_INCLUDE_DIRECTORY}")
    list(APPEND CMAKE_REQUIRED_INCLUDES ${EXIF_INCLUDE_DIRECTORY})
endif()

CHECK_INCLUDE_FILE(jerror.h HAS_LIBJPEG)
if(NOT HAS_LIBJPEG)
    message(FATAL_ERROR "Error: jerror.h not found, you must first install libjpeg or specify JPEG_INCLUDE_DIRECTORY!")
endif()


CHECK_INCLUDE_FILE(libexif/exif-data.h HAS_LIBEXIF)
if(NOT HAS_LIBEXIF)
    message(FATAL_ERROR "Error: libexif/exif-data.h not found, you must first install libexif or specify EXIF_INCLUDE_DIRECTORY!")
endif()

# Build
################################################################################

# Build library
if (BUILD_SHARED_LIB)
    message("* Building shared library")
    add_library(epeg_shared SHARED ${libepeg_SRC})
    target_link_libraries(epeg_shared ${EXTRA_LIBS})
    set_target_properties(epeg_shared PROPERTIES OUTPUT_NAME epeg)
endif()

if (BUILD_STATIC_LIB)
    message("* Building static library")
    add_library(epeg_static STATIC ${libepeg_SRC})
    target_link_libraries(epeg_static ${EXTRA_LIBS})
    set_target_properties(epeg_static PROPERTIES OUTPUT_NAME epeg)
endif()

if (NOT EXISTS src/lib/config.h)
    file(WRITE src/lib/config.h "")
endif()


if (BUILD_SHARED_LIB)
    install(TARGETS epeg_shared DESTINATION lib)
endif()
if (BUILD_STATIC_LIB)
    install(TARGETS epeg_static DESTINATION lib)
endif()

install(FILES ${libepeg_HEADER} DESTINATION include)

