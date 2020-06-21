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
    ROM_file.read((char*)memoryMap.RAM[start], size);
}

void loadROM::inVRAM(int start, int size) {
    ROM_file.seekg(offset);
    ROM_file.read((char*)ppu_memoryMap.PPU_RAM[start], size);
}