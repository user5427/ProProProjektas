cmake_minimum_required(VERSION 3.27)
project(Programa C)

set(CMAKE_C_STANDARD 11)

add_executable(Programa main.c
        stringparser.c
        htmlpage.c)
