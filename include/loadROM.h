//
// Created by jorge on 2020/6/17.
//
#ifndef loadROM_h
#define loadROM_h

#include "memory.h"
#include <fstream>

class loadROM {
public:
    loadROM(std::string path, int offset, MemoryMap& memoryMap, PPU_MemoryMap& ppu_memoryMap);
    ~loadROM(); //close files here

    void inRAM(int start, int size);
    void inVRAM(int start, int size);

private:

    int offset{};
    std::fstream ROM_file;
    MemoryMap memoryMap;
    PPU_MemoryMap ppu_memoryMap;
};

#endif