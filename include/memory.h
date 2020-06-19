//
// Created by jorge on 2020/6/14.
//

#ifndef memory_h
#define memory_h

#include <cstdint>

class MemoryMap {
public:
    void initRam();

    //array containing the whole RAM
    uint8_t(*RAM[0xffff]);

private:

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

class PPU_MemoryMap {
public:
    void initPPU_RAM();

    //array containing the whole PPU RAM
    uint8_t(*PPU_RAM[0x4000]);

private:
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

    //OAM RAM
    uint8_t OAM[0xff]{};
};

#endif