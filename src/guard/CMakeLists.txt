cmake_minimum_required(VERSION 3.16)

add_library(guarder SHARED
        actionsProtector.c
        actionsProtector.h)

set_property(TARGET guarder PROPERTY C_STANDARD  17)
set_target_properties(guarder
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)