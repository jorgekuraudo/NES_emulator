#include <iostream>
#include "../include/disassembler.h"
#include "../include/loadROM.h"
#include "../include/machine.h"
#include <string>

int main() {
    //initialize RAM and VRAM
    MemoryMap memMap;
    memMap.initRam();
    uint8_t** RAM = memMap.RAM;

    PPU_MemoryMap ppu_memMap;
    ppu_memMap.initPPU_RAM();
    uint8_t** VRAM = ppu_memMap.PPU_RAM;

    //load ROM in memory
    loadROM ROM("./build/nestest.nes", 0x10, memMap, ppu_memMap);
    ROM.inRAM(0x8000, 0x4000);
    ROM.inRAM(0xc000, 0x4000);

    //initialize CPU
    CPU6502 cpu(memMap);
    cpu.setPC(0xc000);
    cpu.setSP(0xfd);

    //initialize machine
    machine NES(&cpu);

    //main loop
    while(cpu.isOn) {
        cpu.updateOpcode();
        NES.debugger();
        NES.emulate(cpu.getOpcode());
    }

    return 0;
}
