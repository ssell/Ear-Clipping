cmake_minimum_required(VERSION 3.9)

# ---------------------------------------------------------------------- #
# - Common Settings                                                    - #
# ---------------------------------------------------------------------- #

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PARAM_COMPILER "" CACHE STRING "Compiler identifier added to library output name (FOO_PARAM_COMPILER => FOO_msvc140.dll)")
set(PARAM_ARCH "x86" CACHE STRING "Target output architecture (x86 or x64)")
set(PROJECT_API "BuildAPI_${PARAM_COMPILER}")
set(PROJECT_TEST "TestAPI_${PARAM_COMPILER}")
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/${PARAM_ARCH}")
set(COMPILE_LINK_FLAG "-m32")
set(PROJECT_SUFFIX "")

if(WIN32 OR WIN64)
    set(PROJECT_SUFFIX "_${PARAM_COMPILER}")                                                            # For Windows (msvc) we want to specify the compiler 
endif()

if(PARAM_ARCH STREQUAL "x86")
    set(COMPILE_LINK_FLAG "-m32")
else()
    set(COMPILE_LINK_FLAG "-m64")
endif()

project("EarClipping${PROJECT_SUFFIX}")                                                                 # Name of the parent project/solution

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})                                                       # Set output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# ---------------------------------------------------------------------- #
# - Ear-Clipping Library                                               - #
# ---------------------------------------------------------------------- #

set(API_PROJECT "EarClipping${PROJECT_SUFFIX}")                                                         # Project specific variables
set(API_ROOT_PATH "${CMAKE_SOURCE_DIR}/EarClipping")
set(API_SRC_ROOT_PATH "${API_ROOT_PATH}/src")

file(
    GLOB_RECURSE API_SRC_LIST                                                                           # List of sources
    LIST_DIRECTORIES false
    "${API_SRC_ROOT_PATH}/*.c*"
    "${API_SRC_ROOT_PATH}/*.h*"
    "${API_SRC_ROOT_PATH}/*.inl"
)

add_library(${API_PROJECT} ${API_SRC_LIST})                                                             # Add the library
set_target_properties(${API_PROJECT} PROPERTIES DEBUG_POSTFIX "d")                                      # Add a 'd' to debug builds
set_target_properties(${API_PROJECT} PROPERTIES COMPILE_FLAGS ${COMPILE_LINK_FLAG} LINK_FLAGS ${COMPILE_LINK_FLAG})  

foreach(_source IN ITEMS ${API_SRC_LIST})                                                               # Add the source files to the project 
    get_filename_component(_source_path "${_source}" PATH)                                              # https://stackoverflow.com/a/33813154/735425
    file(RELATIVE_PATH _source_path_rel "${API_SRC_ROOT_PATH}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

# ---------------------------------------------------------------------- #
# - Ear-Clipping Sample Project                                        - #
# ---------------------------------------------------------------------- #