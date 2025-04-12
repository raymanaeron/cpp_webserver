cmake_minimum_required(VERSION 3.16)
project(cpp_webserver)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpp_webserver
    src/main.cpp
)

if(WIN32)
    target_link_libraries(cpp_webserver ws2_32)
endif()
