//
// Created by jorge on 2020/6/14.
//

#include "../include/CPU6502.h"

// utilities
bool CPU6502::checkForOverflow(uint8_t byte1, uint8_t byte2, bool carry) {
    bool sign1 = ((byte1 & 0x80) != 0);
    bool sign2 = ((byte2 & 0x80) != 0);
    uint8_t result = byte1 + byte2 + carry;
    bool signResult = ((result & 0x80) != 0);
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
    *RAM[SP + 0x100] = (PC >> 8) & 0xff; // copy the MSB
    --SP;
    *RAM[SP + 0x100] = PC & 0xff; // copy the LSB
    --SP;
}

void CPU6502::popPC() {
    PC = *RAM[(uint8_t)(SP + 0x01) + 0x100] | (*RAM[(uint8_t)(SP + 0x02) + 0x100] << 8);
    SP += 2;
}

uint8_t CPU6502::get_processor_status() {
    uint8_t PS{};
    PS |= getC();
    PS |= (getZ() << 1);
    PS |= (getI() << 2);
    PS |= (getD() << 3);
    PS |= (0x00 << 4);
    PS |= (0x01 << 5);
    PS |= (getV() << 6);
    PS |= (getN() << 7);
    return PS;
}

void CPU6502::pushProcessorStatus() {
    processor_status = 0x00;
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
    processor_status = *RAM[(uint8_t )(SP + 0x01) + 0x100];

    processor_status |= (0x01 << 4);
    processor_status |= (0x01 << 5);

    setC((processor_status & 0x01) != 0);
    setZ((processor_status & 0x02) != 0);
    setI((processor_status & 0x04) != 0);
    setD((processor_status & 0x08) != 0);
    setV((processor_status & 0x40) != 0);
    setN((processor_status & 0x80) != 0);
    ++SP;
}
// registers getters and setters
uint8_t CPU6502::getA() const {return this->A;}
uint8_t CPU6502::getX() const {return this->X;}
uint8_t CPU6502::getY() const {return this->Y;}

void CPU6502::setA(uint8_t byte) {this->A = byte;}
void CPU6502::setX(uint8_t byte) {this->X = byte;}
void CPU6502::setY(uint8_t byte) {this->Y = byte;}

// flags getters and setters
bool CPU6502::getC() const {return this->C;}
bool CPU6502::getZ() const {return this->Z;}
bool CPU6502::getI() const {return this->I;}
bool CPU6502::getD() const {return this->D;}
bool CPU6502::getB() const {return this->B;}
bool CPU6502::getV() const {return this->V;}
bool CPU6502::getN() const {return this->N;}

void CPU6502::setC(bool flag) {this->C = flag;}
void CPU6502::setZ(bool flag) {this->Z = flag;}
void CPU6502::setI(bool flag) {this->I = flag;}
void CPU6502::setD(bool flag) {this->D = flag;}
void CPU6502::setB(bool flag) {this->B = flag;}
void CPU6502::setV(bool flag) {this->V = flag;}
void CPU6502::setN(bool flag) {this->N = flag;}

// cycles and PC operations
void CPU6502::incCycles(long long inc) {
    cycles += inc;
}

long long int CPU6502::getCycles() const {
    return cycles;
}

void CPU6502::incPC(int inc) {
    PC += inc;
}

uint16_t CPU6502::getPC() const {
    return PC;
}

// Addressing modes definitions
void CPU6502::IMP() {
}

void CPU6502::IMM() {
    isIMM = true;
}

void CPU6502::ZP() {
    if(opcode[1] <= 0xff) {
        address = opcode[1];
    }
    else {
        throw std::out_of_range("Address outside ZP");
    }
}

void CPU6502::ZPX() {
    uint8_t ZP_address = opcode[1] + getX();
    if(ZP_address <= 0xff) {
        address = ZP_address;
    }
    else {
        throw std::out_of_range("Address outside ZP");
    }
}

void CPU6502::ZPY() {
    uint8_t ZP_address = opcode[1] + getY();
    if(ZP_address <= 0xff) {
        address = ZP_address;
    }
    else {
        throw std::out_of_range("Address outside ZP");
    }
}

void CPU6502::ABS() {
    address = opcode[1] | (opcode[2] << 8);
}

void CPU6502::ABSX() {
    address = (opcode[1] | (opcode[2] << 8)) + getX();
}

void CPU6502::ABSY() {
    address = (opcode[1] | (opcode[2] << 8)) + getY();

}

void CPU6502::ACC() {
    isACC = true;
}

void CPU6502::REL() {
}

void CPU6502::IND() {
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
    address = (byte2 << 8) | byte1;
}

void  CPU6502::IDX_I() {
    uint8_t byte1 = *RAM[(uint8_t)(opcode[1] + getX())];
    uint8_t byte2 = *RAM[(uint8_t)(opcode[1] + getX() + 1)];
    address = byte1 | (byte2 << 8);
}

void CPU6502::I_IDX() {
    uint8_t byte1 = *RAM[opcode[1]];
    uint8_t byte2 = *RAM[(uint8_t)(opcode[1] + 1)];
    address = (byte1 | (byte2 << 8)) + getY();
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
    setN((getA() & 0x80) != 0);
}

void CPU6502::AND() {
    uint8_t M = isIMM ? opcode[1] : *RAM[address];
    setA(getA() & M);
    isIMM = false;

    setZ(getA() == 0);
    setN((getA() & 0x80) != 0);
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
    setN((*RAM[address] & 0x80) != 0);
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
void CPU6502::CLV() {setV(false);}
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
    PC += 2;
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
    uint16_t result = getA() - M - (1 - getC());
    isIMM = false;

    //check for overflow flag
    setV(checkForOverflow(getA(), -M - (1 - getC()), getC())); //todo come back to this later
    //set flags
    setC(!(result > 0xff));
    setA(result & 0xff); // update accumulator
    setZ(getA() == 0);
    setN((getA() & 0x80) != 0);
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
    setN((getA() & 0x80) != 0);
    isIMM = false;
}

void CPU6502::LDX() {
    uint8_t M = isIMM ? opcode[1] : *RAM[address];
    setX(M);
    setZ(getX() == 0);
    setN((getX() & 0x80) != 0);
    isIMM = false;
}

void CPU6502::LDY() {
    uint8_t M = isIMM ? opcode[1] : *RAM[address];
    setY(M);
    setZ(getY() == 0);
    setN((getY() & 0x80) != 0);
    isIMM = false;
}

void CPU6502::LSR() {
    uint8_t byte{};
    if(isACC) {
        byte = getA();
        setC((byte & 0x01) != 0);
        byte = byte >> 1;
        setA(byte);
        isACC = false;
    }
    else {
        byte = *RAM[address];
        setC((byte & 0x01) != 0);
        byte = byte >> 1;
        write(address, byte);
    }
    setZ(byte == 0);
    setN((byte & 0x80) != 0);
}

void CPU6502::ROL() {
    uint8_t byte{};
    uint8_t old_carry = getC();
    if(isACC) {
        byte = getA();
        setC((byte & 0x80) != 0);
        byte = byte << 1;
        byte |= old_carry;
        setA(byte);
        isACC = false;
    }
    else {
        byte = *RAM[address];
        setC((byte & 0x80) != 0);
        byte = byte << 1;
        byte |= old_carry;
        write(address, byte);
    }
    setZ(byte == 0);
    setN((byte & 0x80) != 0);
}

void CPU6502::ROR() {
    uint8_t byte{};
    uint8_t old_carry = getC();
    if(isACC) {
        byte = getA();
        setC((byte & 0x01) != 0);
        byte = byte >> 1;
        byte |= (old_carry << 7);
        setA(byte);
        isACC = false;
    }
    else {
        byte = *RAM[address];
        setC((byte & 0x01) != 0);
        byte = byte >> 1;
        byte |= (old_carry << 7);
        write(address, byte);
    }
    setZ(byte == 0);
    setN((byte & 0x80) != 0);
}

void CPU6502::ASL() {
    uint8_t byte{};
    if(isACC) {
        byte = getA();
        setC((byte & 0x80) != 0);
        byte = byte << 1;
        setA(byte);
        isACC = false;
    }
    else {
        byte = *RAM[address];
        setC((byte & 0x80) != 0);
        byte = byte << 1;
        write(address, byte);
    }
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
    setA(*RAM[(uint8_t)(SP + 0x01) + 0x100]);
    setZ(getA() == 0);
    setN((getA() & 0x80) != 0);
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
    setX(SP);
    setZ(getX() == 0);
    setN((getX() & 0x80) != 0);
}

void CPU6502::TXA() {
    setA(getX());
    setZ(getA() == 0);
    setN((getA() & 0x80) != 0);
}

void CPU6502::TXS() {
    SP = getX();
}

void CPU6502::TYA() {
    setA(getY());
    setZ(getA() == 0);
    setN((getA() & 0x80) != 0);
}

void CPU6502::setPC(uint16_t pc) {
    PC = pc;
}

void CPU6502::setSP(uint8_t sp) {
    SP = sp;
}

CPU6502::CPU6502(MemoryMap &memoryMap) {
    this->memoryMap = memoryMap;
}

uint8_t *CPU6502::getOpcode() {
    return opcode;
}

void CPU6502::updateOpcode() {
    opcode = RAM[PC];
}

uint8_t CPU6502::getSP() const {
    return SP;
}

uint8_t CPU6502::read(uint16_t mem_address) {
    return *RAM[mem_address];
}

uint8_t CPU6502::write(uint16_t mem_address, uint8_t data) {
    *RAM[mem_address] = data;
    return 0;
}
