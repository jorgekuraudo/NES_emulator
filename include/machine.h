//
// Created by jorge on 2020/6/17.
//
#ifndef machine_h
#define machine_h

#include "CPU6502.h"
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>

class machine {
private:
    CPU6502* cpu6502;
    //for debugging purposes:
    std::fstream log;
    std::vector<int> P;
    std::vector<int> X;
    std::vector<int> Y;
    std::vector<int> A;
    std::vector<int> PC;
    std::vector<int> SP;
public:
    machine(CPU6502* cpu);
    ~machine();

    void emulate(uint8_t* opcode);

    void debugger();
};

#endif