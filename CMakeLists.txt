cmake_minimum_required(VERSION 3.9)

project(apply_directory_icon)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

include_directories(/usr/include/glib-2.0)
include_directories(/usr/lib/x86_64-linux-gnu/glib-2.0/include)

set(SOURCE_FILES
        src/main.cpp
        src/program.cpp
        src/program.hpp
        )

add_executable(apply_directory_icon ${SOURCE_FILES})
target_link_libraries(apply_directory_icon -lglib-2.0 -lgio-2.0 -lspeed -lstdc++fs)
install(TARGETS apply_directory_icon DESTINATION bin)
