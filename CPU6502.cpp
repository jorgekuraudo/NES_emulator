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

void CPU6502::pushPC() {
    *RAM[SP + 0x100] = PC >> 8; // copy the MSB
    --SP;
    *RAM[SP + 0x100] = PC; // copy the LSB
    --SP;

}

void CPU6502::popPC() {
    PC = *RAM[(uint8_t)(SP + 0x101)] | (*RAM[(uint8_t)(SP + 0x102)] << 8);
}

uint8_t CPU6502::get_processor_status() {
    uint8_t PS{};
    PS |= getC();
    PS |= (getZ() << 1);
    PS |= (getI() << 2);
    PS |= (getD() << 3);
    PS |= (0x01 << 4);
    PS |= (0x01 << 5);
    PS |= (getV() << 6);
    PS |= (getN() << 7);
    return PS;
}

void CPU6502::pushProcessorStatus() {
    processor_status |= getC();
    processor_status |= (getZ() << 1);
    processor_status |= (getI() << 2);
    processor_status |= (getD() << 3);
    processor_status |= (0x01 << 4);
    processor_status |= (0x01 << 5);
    processor_status |= (getV() << 6);
    processor_status |= (getN() << 7);
    *RAM[SP + 0x100] = processor_status;
    --SP;
}

void CPU6502::popProcessorStatus() {
    processor_status = *RAM[(uint8_t )(SP + 0x101)];
    setC((processor_status & 0x01) != 0);
    setZ((processor_status & 0x02) != 0);
    setI((processor_status & 0x04) != 0);
    setD((processor_status & 0x08) != 0);
    setV((processor_status & 0x40) != 0);
    setN((processor_status & 0x80) != 0);
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
bool CPU6502::getD() {return this->D;}
bool CPU6502::getB() {return this->B;}
bool CPU6502::getV() {return this->V;}
bool CPU6502::getN() {return this->N;}

void CPU6502::setC(bool flag) {this->C = flag;}
void CPU6502::setZ(bool flag) {this->Z = flag;}
void CPU6502::setI(bool flag) {this->I = flag;}
void CPU6502::setD(bool flag) {this->D = flag;}
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
}

void CPU6502::AND() {
    uint8_t M = isIMM ? opcode[1] : *RAM[address];
    setA(getA() & M);
    isIMM = false;

    setZ(getA() == 0);
    setN(getA() & 0x80);
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

void CPU6502::BCS() {
    if (getC()) {
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

void CPU6502::BEQ() {
    if (getZ()) {
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

void CPU6502::BNE() {
    if (!getZ()) {
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

void CPU6502::BMI() {
    if (getN()) {
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

void CPU6502::BPL() {
    if (!getN()) {
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

void CPU6502::BVS() {
    if (getV()) {
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

void CPU6502::BVC() {
    if (!getV()) {
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

void CPU6502::BIT() {
    uint8_t result = getA() & *RAM[address];

    setZ(result == 0);
    setV(*RAM[address] & 0x40);
    setN(*RAM[address] & 0x80);
}

void CPU6502::BRK() {
    pushPC();
    pushProcessorStatus();
    setB(true);
    setI(true);
    PC = *RAM[0xfffe] | (*RAM[0xffff] << 8);
}

void CPU6502::CLC() {setC(false);}
void CPU6502::CLD() {setD(false);}
void CPU6502::CLI() {setI(false);}
void CPU6502::CLV() {setI(false);}
void CPU6502::SEC() {setC(true);}
void CPU6502::SED() {setD(true);}
void CPU6502::SEI() {setI(true);}

void CPU6502::CMP() {
    uint8_t target = isIMM ? opcode[1] : *RAM[address];;
    setC(getA() >= target);
    setZ(getA() == target);
    setN(((getA() - target) & 0x80) != 0);
    isIMM = false;
}

void CPU6502::CPX() {
    uint8_t target = isIMM ? opcode[1] : *RAM[address];;
    setC(getX() >= target);
    setZ(getX() == target);
    setN(((getX() - target) & 0x80) != 0);
    isIMM = false;
}

void CPU6502::CPY() {
    uint8_t target = isIMM ? opcode[1] : *RAM[address];
    setC(getY() >= target);
    setZ(getY() == target);
    setN(((getY() - target) & 0x80) != 0);
    isIMM = false;
}

void CPU6502::DEC() {
    *RAM[address] -= 1;
    setZ(*RAM[address] == 0);
    setN((*RAM[address] & 0x80) != 0);
}

void CPU6502::DEX() {
    setX(getX() - 1);
    setZ(getX() == 0);
    setN((getX() & 0x80) != 0);
}

void CPU6502::DEY() {
    setY(getY() - 1);
    setZ(getY() == 0);
    setN((getY() & 0x80) != 0);
}

void CPU6502::INC() {
    *RAM[address] += 1;
    setZ(*RAM[address] == 0);
    setN((*RAM[address] & 0x80) != 0);
}

void CPU6502::INX() {
    setX(getX() + 1);
    setZ(getX() == 0);
    setN((getX() & 0x80) != 0);
}

void CPU6502::INY() {
    setY(getY() + 1);
    setZ(getY() == 0);
    setN((getY() & 0x80) != 0);
}

void CPU6502::EOR() {
    uint8_t target = isIMM ? opcode[1] : *RAM[address];
    setA(getA() ^ target);
    setZ(getA() == 0);
    setN((getA() & 0x80) != 0);
}

void CPU6502::ORA() {
    uint8_t target = isIMM ? opcode[1] : *RAM[address];
    setA(getA() | target);
    setZ(getA() == 0);
    setN((getA() & 0x80) != 0);
}

void CPU6502::JMP() {
    PC = address;
}

void CPU6502::JSR() {
    pushPC();
    PC = address;
}

void CPU6502::RTS() {
    popPC();
}

void CPU6502::RTI() {
    popProcessorStatus();
    popPC();
}

void CPU6502::SBC() {
    uint8_t M = isIMM ? opcode[1] : *RAM[address];
    uint16_t result = getA() - M -(1 - getC());
    isIMM = false;

    //check for overflow flag
    setV(checkForOverflow(getA(), -M, getC()));
    //set flags
    setC(result > 0xff);
    setA(result & 0xff); // update accumulator
    setZ(getA() == 0);
    setN(getA() & 0x80);
}

void CPU6502::STA() {
    *RAM[address] = getA();
}

void CPU6502::STX() {
    *RAM[address] = getX();
}

void CPU6502::STY() {
    *RAM[address] = getY();
}

void CPU6502::LDA() {
    uint8_t M = isIMM ? opcode[1] : *RAM[address];
    setA(M);
    setZ(getA() == 0);
    setN(getA() & 0x80);
    isIMM = false;
}

void CPU6502::LDX() {
    uint8_t M = isIMM ? opcode[1] : *RAM[address];
    setX(M);
    setZ(getA() == 0);
    setN(getA() & 0x80);
    isIMM = false;
}

void CPU6502::LDY() {
    uint8_t M = isIMM ? opcode[1] : *RAM[address];
    setY(M);
    setZ(getA() == 0);
    setN(getA() & 0x80);
    isIMM = false;
}

void CPU6502::LSR() {
    uint8_t byte{};
    if(isACC) {
        byte = getA();
        byte = byte >> 1;
        setA(byte);
        isACC = false;
    }
    else {
        uint8_t& byte = *RAM[address];
        byte = byte >> 1;
    }
    setC((byte & 0x01) != 0);
    setZ(byte == 0);
    setN((byte & 0x80) != 0);
}

void CPU6502::ROL() {
    uint8_t byte{};
    uint8_t old_carry = getC();
    if(isACC) {
        byte = getA();
        byte = byte << 1;
        byte |= old_carry;
        setA(byte);
        isACC = false;
    }
    else {
        uint8_t& byte = *RAM[address];
        byte = byte << 1;
        byte |= old_carry;
    }
    setC((byte & 0x01) != 0);
    setZ(byte == 0);
    setN((byte & 0x80) != 0);
}

void CPU6502::ROR() {
    uint8_t byte{};
    uint8_t old_carry = getC();
    if(isACC) {
        byte = getA();
        byte = byte >> 1;
        byte |= (old_carry << 7);
        setA(byte);
        isACC = false;
    }
    else {
        uint8_t& byte = *RAM[address];
        byte = byte >> 1;
        byte |= (old_carry << 7);
    }
    setC((byte & 0x01) != 0);
    setZ(byte == 0);
    setN((byte & 0x80) != 0);
}

void CPU6502::ASL() {
    uint8_t byte{};
    if(isACC) {
        byte = getA();
        byte = byte << 1;
        setA(byte);
        isACC = false;
    }
    else {
        uint8_t& byte = *RAM[address];
        byte = byte << 1;
    }
    setC((byte & 0x01) != 0);
    setZ(byte == 0);
    setN((byte & 0x80) != 0);
}

void CPU6502::NOP() {

}

void CPU6502::PHA() {
    *RAM[SP + 0x100] = getA();
    --SP;
}

void CPU6502::PHP() {
    pushProcessorStatus();
}

void CPU6502::PLA() {
    setA(*RAM[(uint8_t)(SP + 0x101)]);
    ++SP;
}

void CPU6502::PLP() {
    popProcessorStatus();
}

void CPU6502::TAX() {
    setX(getA());
    setZ(getX() == 0);
    setN((getX() & 0x80) != 0);
}

void CPU6502::TAY() {
    setY(getA());
    setZ(getY() == 0);
    setN((getY() & 0x80) != 0);
}

void CPU6502::TSX() {
    setX(*RAM[SP + 0x100]);
    setZ(getX() == 0);
    setN((getX() & 0x80) != 0);
}

void CPU6502::TXA() {
    setA(getX());
    setZ(getA() == 0);
    setN((getA() & 0x80) != 0);
}

void CPU6502::TXS() {
    *RAM[SP + 0x100] = getX();
}

void CPU6502::TYA() {
    setA(getY());
    setZ(getA() == 0);
    setN((getA() & 0x80) != 0);
}