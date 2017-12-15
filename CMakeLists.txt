cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(mqudp VERSION 0.1 LANGUAGES C)
set (CMAKE_C_STANDARD 11)

add_executable (mqudp src/main.c)
target_link_libraries(mqudp -lrt)

add_custom_command(TARGET mqudp 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mqudp> ${PROJECT_BINARY_DIR})
