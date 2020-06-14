//
// Created by jorge on 2020/6/14.
//

#include "includes/CPU6502.h"
#include "includes/memory.h"
#include <stdexcept>

// utilities
bool CPU6502::checkForOverflow(uint8_t byte1, uint8_t byte2, bool carry) {
    bool sign1 = (byte1 & 0x80) != 0;
    bool sign2 = (byte2 & 0x80) != 0;
    uint8_t result = byte1 + byte2 + carry;
    bool signResult = (result & 0x80) != 0;
    if (sign1 != sign2) {
        return false;
    }
    else {
        if (sign1 != signResult) {
            return true;
        }
        else {
            return false;
        }

    }
}

bool CPU6502::hasCrossPage(const uint16_t& current, const uint16_t& next) {
    if ((current & 0xff00) == (next & 0xff00)) {
        return false;
    }
    else {
        return true;
    }
}

uint8_t CPU6502::twosComplement(const uint8_t& byte) {
    return abs(byte - 256);
}

// registers getters and setters
uint8_t CPU6502::getA() {return this->A;}
uint8_t CPU6502::getX() {return this->X;}
uint8_t CPU6502::getY() {return this->Y;}

void CPU6502::setA(uint8_t byte) {this->A = byte;}
void CPU6502::setX(uint8_t byte) {this->X = byte;}
void CPU6502::setY(uint8_t byte) {this->Y = byte;}

// flags getters and setters
bool CPU6502::getC() {return this->C;}
bool CPU6502::getZ() {return this->Z;}
bool CPU6502::getI() {return this->I;}
bool CPU6502::getB() {return this->B;}
bool CPU6502::getV() {return this->V;}
bool CPU6502::getN() {return this->N;}

void CPU6502::setC(bool flag) {this->C = flag;}
void CPU6502::setZ(bool flag) {this->Z = flag;}
void CPU6502::setI(bool flag) {this->I = flag;}
void CPU6502::setB(bool flag) {this->B = flag;}
void CPU6502::setV(bool flag) {this->V = flag;}
void CPU6502::setN(bool flag) {this->N = flag;}

// Addressing modes definitions
uint16_t CPU6502::IMP() {
    return 0;
}

uint8_t CPU6502::IMM() {
    isIMM = true;
    return opcode[1];
}

uint16_t CPU6502::ZP() {
    if(opcode[1] <= 0xff) {
        return opcode[1];
    }
    else {
        throw std::out_of_range("Address outside ZP");
        return -1;
    }
}

uint16_t CPU6502::ZPX() {
    uint8_t ZP_address = opcode[1] + getX();
    if(ZP_address <= 0xff) {
        return ZP_address;
    }
    else {
        throw std::out_of_range("Address outside ZP");
        return -1;
    }
}

uint16_t CPU6502::ZPY() {
    uint8_t ZP_address = opcode[1] + getY();
    if(ZP_address <= 0xff) {
        return ZP_address;
    }
    else {
        throw std::out_of_range("Address outside ZP");
        return -1;
    }
}

uint16_t CPU6502::ABS() {
    return opcode[1] | (opcode[2] << 8);
}

uint16_t CPU6502::ABSX() {
    uint16_t abs_address = (opcode[1] | (opcode[2] << 8)) + getX();
    return abs_address;
}

uint16_t CPU6502::ABSY() {
    uint16_t abs_address = (opcode[1] | (opcode[2] << 8)) + getY();
    return abs_address;
}

uint16_t CPU6502::ACC() {
    isACC = true;
    return 0;
}

uint16_t CPU6502::REL() {
    return 0;
}

uint16_t CPU6502::IND() {
    uint16_t address1 = (opcode[2] << 8) | opcode[1];
    uint8_t byte1 = *RAM[address1];
    //this is a bug in the original 6502 processor
    uint8_t byte2;
    if (opcode[1] == 0xff) {
        byte2 = *RAM[(opcode[2] << 8) | 0x00];
    }
    else {
        byte2 = *RAM[address1 + 1];
    }
    uint16_t ind_address = (byte2 << 8) | byte1;
    return ind_address;
}

uint16_t  CPU6502::IDX_I() {
    uint8_t byte1 = *RAM[(uint8_t)(opcode[1] + getX())];
    uint8_t byte2 = *RAM[(uint8_t)(opcode[1] + getX() + 1)];
    uint16_t ind_address = byte1 | (byte2 << 8);
    return ind_address;
}

uint16_t CPU6502::I_IDX() {
    uint8_t byte1 = *RAM[opcode[1]];
    uint8_t byte2 = *RAM[(uint8_t)(opcode[1] + 1)];
    uint16_t ind_address = (byte1 | (byte2 << 8)) + getY();
    return ind_address;
}

// Instruction set
void CPU6502::ADC() {
    uint8_t M = isIMM ? opcode[1] : *RAM[address];
    uint16_t result = getA() + M + getC();
    isIMM = false;

    //check for overflow flag
    setV(checkForOverflow(getA(), M, getC()));
    //set flags
    setC(result > 0xff);
    setA(result & 0xff); // update accumulator
    setZ(getA() == 0);
    setN(getA() & 0x80);

    //check how many cycles have passed
    hasCrossPage(PC, PC + 3) ? cycles += 5 : cycles += 4;
    PC += 3;
}

void CPU6502::AND() {
    uint8_t M = isIMM ? opcode[1] : *RAM[address];
    setA(getA() & M);
    isIMM = false;

    setZ(getA() == 0);
    setN(getA() & 0x80);

    cycles += 6;
    PC += 2;
}

void CPU6502::ASL() {
    uint8_t M = isACC ? getA() : *RAM[address];
    uint8_t& byte = M;
    setC((byte & 0x80) != 0);
    byte = byte << 1;

    setZ(byte == 0);
    setN((byte & 0x80) != 0);

    cycles += 5;
    PC += 2;
}

void CPU6502::BCC() {
    if (!getC()) {
        if ((opcode[1] & 0x80) != 0) {
            hasCrossPage(PC, PC + 2 - twosComplement(opcode[1])) ? cycles += 4 : cycles += 3;
            PC += 2 - twosComplement(opcode[1]);
        }
        else {
            hasCrossPage(PC, PC + opcode[1]) ? cycles += 4 : cycles += 3;
            PC += 2 + opcode[1];
        }
    }
    else {
        cycles += 2;
        PC += 2;
    }
}
