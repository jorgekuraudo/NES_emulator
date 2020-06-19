cmake_minimum_required(VERSION 3.16)
project(NES_emulator)

set(CMAKE_CXX_STANDARD 14)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/others/6502ops.csv
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#remove this in the final version
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/others/nestest.nes
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB SOURCES
        include/*.h
        src/*.cpp
        )

add_executable(NES_emulator ${SOURCES})