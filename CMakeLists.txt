cmake_minimum_required(VERSION 4.3)
project(Writer C)

set(CMAKE_C_STANDARD 90)

add_executable(Writer src/main.c
        include/plataform.h
        plataform/linux/process.c
        plataform/windows/process.c
        plataform/linux/threads.c
        plataform/linux/mutex.c
        plataform/windows/threads.c
        plataform/windows/mutex.c)
