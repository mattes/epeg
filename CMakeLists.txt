cmake_minimum_required(VERSION 3.0)

project(libepeg)
set (VERSION "0.9.2")

option(BUILD_STATIC_LIB "Build static library" ON)
option(BUILD_SHARED_LIB "Build shared library" ON)
option(BUILD_PROGRAM "Build command line tool" OFF)

# Build configuration
# this project needs libjpeg and libexif.
# in order to build the shared library, the static or shared libraries of libjpeg
# and libexif must be found.
# if they are not in a standard location, you can specify:
# CMAKE_FIND_ROOT_PATH or JPEG_INCLUDE_DIRECTORY/EXIF_INCLUDE_DIRECTORY and
# JPEG_LIB_DIRECTORY/EXIF_LIB_DIRECTORY
################################################################################

set(CMAKE_POSITION_INDEPENDENT_CODE ON)


# Platform specific build configuration
################################################################################

if(WIN32)
    message(STATUS "Building for windows")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()


# Source files
################################################################################


file(GLOB_RECURSE libepeg_SRC src/lib/*.c)
list(SORT libepeg_SRC)

set(libepeg_HEADER src/lib/Epeg.h)

file(GLOB_RECURSE epeg_SRC src/bin/*.c)
list(SORT epeg_SRC)


find_path(JPEG_DIR_HEADER NAME jerror.h HINTS "${JPEG_INCLUDE_DIRECTORY}")
if (NOT JPEG_DIR_HEADER)
    message(FATAL_ERROR "Error: jerror.h not found, you must first install libjpeg or specify JPEG_INCLUDE_DIRECTORY!")
endif()

message(STATUS "Found jerror.h...")
include_directories(BEFORE SYSTEM "${JPEG_DIR_HEADER}")


find_path(EXIF_DIR_HEADER NAME libexif/exif-data.h HINTS "${EXIF_INCLUDE_DIRECTORY}")
if (NOT EXIF_DIR_HEADER)
    message(FATAL_ERROR "Error: libexif/exif-data.h not found, you must first install libexif or specify EXIF_INCLUDE_DIRECTORY!")
endif()

message(STATUS "Found libexif/exif-data.h...")
include_directories(BEFORE SYSTEM "${EXIF_DIR_HEADER}")


if(CMAKE_BUILD_TYPE STREQUAL "Release")
   add_compile_options(-fdata-sections -ffunction-sections)
endif()

# Build
################################################################################

# Build library
if (BUILD_SHARED_LIB OR BUILD_PROGRAM)
    if(NOT DEFINED JPEG_LIB_DIRECTORY)
        set(JPEG_LIB_DIRECTORY ${JPEG_DIR_HEADER}/../lib)
    endif()
    find_library(JPEG_LIBRARY NAMES jpeg HINTS ${JPEG_LIB_DIRECTORY})
    if (NOT JPEG_LIBRARY)
        message(FATAL_ERROR "Error: cant find jpeg library, you must first install libjpeg or specify JPEG_LIB_DIRECTORY!")
    endif()


    if(NOT DEFINED EXIF_LIB_DIRECTORY)
        set(EXIF_LIB_DIRECTORY ${EXIF_DIR_HEADER}/../lib)
    endif()
    find_library(EXIF_LIBRARY NAMES exif HINTS ${EXIF_LIB_DIRECTORY})
    if (NOT EXIF_LIBRARY)
        message(FATAL_ERROR "Error: cant find exif library, you must first install libexif or specify EXIF_LIB_DIRECTORY!")
    endif()
endif()

if(BUILD_SHARED_LIB)
    message(STATUS "Building shared library")
    add_library(epeg_shared SHARED ${libepeg_SRC})
    if(WIN32)
        target_compile_definitions(epeg_shared PUBLIC BUILDING_DLL)
    endif()
    target_link_libraries(epeg_shared ${JPEG_LIBRARY} ${EXIF_LIBRARY})
    set_target_properties(epeg_shared PROPERTIES OUTPUT_NAME epeg)

    install(TARGETS epeg_shared DESTINATION lib)
endif()

if (BUILD_STATIC_LIB OR BUILD_PROGRAM)
    message(STATUS "Building static library")
    add_library(epeg_static STATIC ${libepeg_SRC})
    set_target_properties(epeg_static PROPERTIES OUTPUT_NAME epeg)
    install(TARGETS epeg_static DESTINATION lib)
endif()

if (BUILD_PROGRAM)
    set(dll_lib m)
    add_executable(epeg_bin ${epeg_SRC})
    message(STATUS "Building command line tool")
    target_link_libraries(epeg_bin epeg_static ${JPEG_LIBRARY} ${EXIF_LIBRARY})
    target_link_libraries(epeg_bin -Wl,--gc-sections)
    target_link_libraries(epeg_bin -Wl,-Bdynamic ${dll_lib})
    set_target_properties(epeg_bin PROPERTIES OUTPUT_NAME epeg)
    install(TARGETS epeg_bin DESTINATION bin)
endif()

if (NOT EXISTS src/lib/config.h)
    file(WRITE src/lib/config.h "")
endif()


install(FILES ${libepeg_HEADER} DESTINATION include)

