//
// Created by jorge on 2020/6/14.
//

#ifndef CPU6502_h
#define CPU6502_h

#include <cstdint>
#include <stdexcept>
#include "memory.h"

class CPU6502 {
private:
    MemoryMap memoryMap{};
    uint8_t** RAM{memoryMap.RAM};

    uint16_t address{}; // address the CPU is accessing
    uint16_t PC{};
    uint8_t SP{};
    uint8_t* opcode{RAM[PC]};
    long long int cycles{};

    //registers
    uint8_t X{};
    uint8_t Y{};
    uint8_t A{};

    //processor status
    bool C{}; // carry
    bool Z{}; // Zero
    bool I{true}; // Interrupt disable
    bool D{}; //Decimal (not used)
    bool B{}; // Break
    bool V{}; // Overflow
    bool N{}; // Negative
    uint8_t processor_status{};

public:
    int ins_count{};
    bool isOn{true}; //for debugging
    // give access to RAM in constructor
    CPU6502(MemoryMap& memoryMap);

    // utility functions
    bool checkForOverflow(uint8_t byte1, uint8_t byte2, bool carry);
    bool hasCrossPage(const uint16_t& current, const uint16_t& next);
    uint8_t twosComplement(const uint8_t& byte);
    void pushPC(); // push current PC onto the stack
    void popPC(); // retrieve PC from stack

    uint8_t read(uint16_t mem_addres);
    uint8_t write(uint16_t mem_address, uint8_t data);

    void incCycles(long long int cycles);
    long long int getCycles() const;

    void incPC(int inc);
    uint16_t getPC() const;
    uint8_t getSP() const;
    void setPC(uint16_t pc);
    void setSP(uint8_t sp);
    uint8_t* getOpcode();
    void updateOpcode();

    // flags setters and getters
    void setC(bool flag);   bool getC() const;
    void setZ(bool flag);   bool getZ() const;
    void setI(bool flag);   bool getI() const;
    void setD(bool flag);   bool getD() const;
    void setB(bool flag);   bool getB() const;
    void setV(bool flag);   bool getV() const;
    void setN(bool flag);   bool getN() const;
    // Processor status related functions
    uint8_t get_processor_status();
    void pushProcessorStatus();
    void popProcessorStatus();

    // registers setters and getters
    void setX(uint8_t byte);    uint8_t getX() const;
    void setY(uint8_t byte);    uint8_t getY() const;
    void setA(uint8_t byte);    uint8_t getA() const;

    // addressing modes
    void IMP();     void ZPY();     void IND();
    void ACC();     void REL();     void IDX_I();
    void IMM();     void ABS();     void I_IDX();
    void ZP();      void ABSX();
    void ZPX();     void ABSY();
    bool isIMM{false};
    bool isACC{false};

    // instructions
    void ADC(); void AND(); void ASL(); void BCC(); void BCS(); void BEQ(); void BIT(); void BMI();
    void BNE(); void BPL(); void BRK(); void BVC(); void BVS(); void CLC(); void CLD(); void CLI();
    void CLV(); void CMP(); void CPX(); void CPY(); void DEC(); void DEX(); void DEY(); void EOR();
    void INC(); void INX(); void INY(); void JMP(); void JSR(); void LDA(); void LDX(); void LDY();
    void LSR(); void NOP(); void ORA(); void PHA(); void PHP(); void PLA(); void PLP(); void ROL();
    void ROR(); void RTI(); void RTS(); void SBC(); void SEC(); void SED(); void SEI(); void STA();
    void STX(); void STY(); void TAX(); void TAY(); void TSX(); void TXA(); void TXS(); void TYA();

};
#endif