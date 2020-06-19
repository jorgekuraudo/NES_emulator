//
// Created by jorge on 2020/6/17.
//
#include "../include/loadROM.h"

loadROM::loadROM(std::string path, int offset, MemoryMap& memoryMap, PPU_MemoryMap& ppu_memoryMap) {
    ROM_file.open(path, std::ios::in | std::ios::binary);
    if(ROM_file.is_open()) {
        // do nothing
    } else {
        throw std::runtime_error("Could not open file.");
    }
    this->offset = offset;
    this->memoryMap = memoryMap;
    this->ppu_memoryMap = ppu_memoryMap;
}

loadROM::~loadROM() {
    ROM_file.close();
}

void loadROM::inRAM(int start, int size) {
    ROM_file.seekg(offset);
    char* buffer;
    for(int i = 0; i < size; ++i) {
        ROM_file.seekg(offset + i);
        ROM_file.read(buffer, 1);
        *memoryMap.RAM[start + i] = (uint8_t)buffer[0];
    }
}

void loadROM::inVRAM(int start, int size) {
    ROM_file.seekg(offset);
    char* buffer;
    for(int i = 0; i < size; ++i) {
        ROM_file.seekg(offset + i);
        ROM_file.read(buffer, 1);
        *ppu_memoryMap.PPU_RAM[start + i] = (uint8_t)buffer[0];
    }
}