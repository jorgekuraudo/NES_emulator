cmake_minimum_required(VERSION 3.16)
project(NES_emulator)

set(CMAKE_CXX_STANDARD 14)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/others/6502ops.csv
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(untitled1 main.cpp CPU6502.cpp includes/CPU6502.h includes/memory.h disassembler.cpp includes/disassembler.h machine.cpp includes/machine.h)