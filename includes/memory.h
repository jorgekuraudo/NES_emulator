//
// Created by jorge on 2020/6/14.
//
#pragma once
#include <cstdint>

struct MemoryMap {
    void initRam() {
        //initialize work RAM and mirrors
        for (int i = 0; i < 0x800; ++i) {
            RAM[i] = &work_RAM[i];
            //mirror three times
            RAM[i + 0x800] = &work_RAM[i];
            RAM[i + 0x1000] = &work_RAM[i];
            RAM[i + 0x1800] = &work_RAM[i];
        }

        //initialize PPU Ctrl Registers
        for (int i = 0; i < 0x8; ++i) {
            RAM[0x2000 + i] = &PPU_reg[i];
        }
        //mirror PPU registers (1023 times)
        for (int i = 0; i < 1023; ++i) {
            for (int j = 0; j < 8; ++j) {
                RAM[0x2008 + i * 8 + j] = &PPU_reg[j];
            }
        }

        //initialize APU Registers
        for (int i = 0; i < 0x20; ++i) {
            RAM[0x4000 + i] = &PPU_reg[i];
        }

        //initialize Expansion ROM
        for (int i = 0; i < 0x1fe0; ++i) {
            RAM[0x4020 + i] = &PPU_reg[i];
        }

        //initialize SRAM
        for (int i = 0; i < 0x2000; ++i) {
            RAM[0x6000 + i] = &SRAM[i];
        }

        //initialize PRG-ROM 1
        for (int i = 0; i < 0x4000; ++i) {
            RAM[0x8000 + i] = &PRG1[i];
        }

        //initialize PRG-ROM 2
        for (int i = 0; i < 0x4000; ++i) {
            RAM[0xc000 + i] = &PRG2[i];
        }
    }
    //array containing the whole RAM
    uint8_t(*RAM[0xffff]) {nullptr};

    //0x800 for the work RAM
    uint8_t work_RAM[0x800]{};

    //PPU Ctrl Registers
    uint8_t PPU_reg[0x8]{};

    //APU registers
    uint8_t APU_reg[0x20]{};

    //Cartridge Expansion ROM
    uint8_t expansion_ROM[0x1fdf]{};

    //SRAM
    uint8_t SRAM[0x2000]{};

    //PRG-ROM 1
    uint8_t PRG1[0x4000]{};

    //PRG-ROM 2
    uint8_t PRG2[0x4000]{};
};

MemoryMap memMap;
uint8_t** RAM = memMap.RAM;


struct PPU_MemoryMap {
    void initPPU_RAM() {
        //initialize pattern tables
        for (int i = 0; i < 0x1000; ++i) {
            PPU_RAM[i] = &pattern_table0[i];
            PPU_RAM[i + 0x1000] = &pattern_table1[i];
        }

        //nametables
        for (int i = 0; i < 0x3c0; ++i) {
            PPU_RAM[0x2000 + i] = &nametable0[i];
            PPU_RAM[0x2400 + i] = &nametable1[i];
            PPU_RAM[0x2800 + i] = &nametable2[i];
            PPU_RAM[0x2c00 + i] = &nametable3[i];
        }

        //attributes
        for (int i = 0; i < 0x40; ++i) {
            PPU_RAM[0x23c0 + i] = &att_table0[i];
            PPU_RAM[0x27c0 + i] = &att_table1[i];
            PPU_RAM[0x2bc0 + i] = &att_table2[i];
            PPU_RAM[0x2fc0 + i] = &att_table3[i];
        }

        //Mirror of 2000h-2EFFh
        for (int i = 0; i < 0x0f00; ++i) {
            PPU_RAM[0x3000 + i] = PPU_RAM[0x2000 + i];
        }

        //background and sprite palettes
        for (int i = 0; i < 0x10; ++i) {
            PPU_RAM[0x3f00 + i] = &BG_palette[i];
            PPU_RAM[0x3f10 + i] = &sprite_palette[i];
        }

        //Mirror of 3F00h-3F1Fh
        for (int i = 0; i < 0xe0; ++i) {
            PPU_RAM[0x3f20 + i] = PPU_RAM[0x3f00 + i];
        }
    }
    //array containing the whole PPU RAM
    uint8_t(*PPU_RAM[0x4000]) { nullptr };

    //pattern table 0
    uint8_t pattern_table0[0x1000]{};

    //pattern table 1
    uint8_t pattern_table1[0x1000]{};

    //name table and attribute table 0
    uint8_t nametable0[0x3c0];
    uint8_t att_table0[0x40];

    //name table and attribute table 1
    uint8_t nametable1[0x3c0];
    uint8_t att_table1[0x40];

    //name table and attribute table 2
    uint8_t nametable2[0x3c0];
    uint8_t att_table2[0x40];

    //name table and attribute table 3
    uint8_t nametable3[0x3c0];
    uint8_t att_table3[0x40];

    //background palette
    uint8_t BG_palette[0x10];

    //sprite palette
    uint8_t sprite_palette[0x10];
};

PPU_MemoryMap PPU_memMap;
uint8_t** VRAM = PPU_memMap.PPU_RAM;

//OAM RAM
uint8_t OAM[0xff]{};