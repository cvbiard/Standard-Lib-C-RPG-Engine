cmake_minimum_required(VERSION 3.17)
project(RPG C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(RPG
        header.h
        main.c
        protos.c)
