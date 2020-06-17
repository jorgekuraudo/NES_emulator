//
// Created by jorge on 2020/6/17.
//
#pragma once
#include "CPU6502.h"

class machine {
private:
    CPU6502 cpu6502;
public:
    machine(CPU6502 cpu);

    void emulate(uint8_t* opcode);

    void debugger();
};