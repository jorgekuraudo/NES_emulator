cmake_minimum_required(VERSION 3.16)
project(NES_emulator)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled1 main.cpp CPU6502.cpp includes/CPU6502.h includes/memory.h)