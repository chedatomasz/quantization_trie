cmake_minimum_required(VERSION 3.13)
project(quantization_cheda C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(quantization_cheda
        engine.c
        engine.h
        input.c
        input.h
        instruction_interface.h
        main.c
        output.c
        output.h
        safe_alloc.c
        safe_alloc.h
        structures.c
        structures.h)
