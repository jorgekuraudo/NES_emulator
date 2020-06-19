//
// Created by jorge on 2020/6/17.
//

#include "../include/machine.h"

machine::machine(CPU6502* cpu) {
    cpu6502 = cpu;
    log.open("/home/jorge/Desktop/log", std::ios::in);
    std::string line;
    while(std::getline(log, line)) {
        for(auto& c : line) {
            c = ::tolower(c);
        }
        std::string p{};
        p += line.at(65);
        p += line.at(66);
        P.push_back(std::stoi(p, nullptr , 16));

        std::string x{};
        x += line.at(55);
        x += line.at(56);
        X.push_back(std::stoi(x, nullptr , 16));

        std::string y{};
        y += line.at(60);
        y += line.at(61);
        Y.push_back(std::stoi(y, nullptr , 16));

        std::string a{};
        a += line.at(50);
        a += line.at(51);
        A.push_back(std::stoi(a, nullptr , 16));

        std::string pc{};
        pc += line.at(0);
        pc += line.at(1);
        pc += line.at(2);
        pc += line.at(3);
        PC.push_back(std::stoi(pc, nullptr , 16));

        std::string sp{};
        sp += line.at(71);
        sp += line.at(72);
        SP.push_back(std::stoi(sp, nullptr , 16));
    }
}

void machine::emulate(uint8_t* opcode) {
    switch (*opcode)
    {
        case 0x00:	//BRK
        {
            cpu6502->BRK();
            cpu6502->incCycles(7);
            break;
        }
        case 0x01:	//ORA Ind,X
        {
            cpu6502->IDX_I();
            cpu6502->ORA();
            cpu6502->incCycles(6);
            cpu6502->incPC(2);
            break;

        }
        case 0x02: printf("Opcode 0x02 hasn't been implemented yet\n"); break;
        case 0x03: printf("Opcode 0x03 hasn't been implemented yet\n"); break;
        case 0x04: printf("Opcode 0x04 hasn't been implemented yet\n"); break;
        case 0x05:	//ORA ZP
        {
            cpu6502->ZP();
            cpu6502->ORA();
            cpu6502->incCycles(3);
            cpu6502->incPC(2);
            break;
        }
        case 0x06:	//ASL ZP
        {
            cpu6502->ZP();
            cpu6502->ASL();
            cpu6502->incCycles(5);
            cpu6502->incPC(2);
            break;
        }
        case 0x07: printf("Opcode 0x07 hasn't been implemented yet\n"); break;
        case 0x08:	//PHP
        {
            cpu6502->IMP();
            cpu6502->PHP();
            cpu6502->incCycles(3);
            cpu6502->incPC(1);
            break;
        }
        case 0x09:	//ORA Im
        {
            cpu6502->IMM();
            cpu6502->ORA();
            cpu6502->incCycles(2);
            cpu6502->incPC(2);
            break;
        }
        case 0x0a:	//ASL Acc
        {
            cpu6502->ACC();
            cpu6502->ASL();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0x0b: printf("Opcode 0x0b hasn't been implemented yet\n"); break;
        case 0x0c: printf("Opcode 0x0c hasn't been implemented yet\n"); break;
        case 0x0d:	//ORA Abs
        {
            cpu6502->ABS();
            cpu6502->ORA();
            cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0x0e:	//ASL Abs
        {
            cpu6502->ABS();
            cpu6502->ASL();
            cpu6502->incCycles(6);
            cpu6502->incPC(3);
            break;
        }
        case 0x0f: printf("Opcode 0x0f hasn't been implemented yet\n"); break;
        case 0x10:	//BPL Rel
        {
            cpu6502->BPL();
            break;
        }
        case 0x11:	//ORA Ind,Y
        {
            cpu6502->I_IDX();
            cpu6502->ORA();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 2) ? cpu6502->incCycles(6) : cpu6502->incCycles(5);
            cpu6502->incPC(2);
            break;

        }
        case 0x12: printf("Opcode 0x12 hasn't been implemented yet\n"); break;
        case 0x13: printf("Opcode 0x13 hasn't been implemented yet\n"); break;
        case 0x14: printf("Opcode 0x14 hasn't been implemented yet\n"); break;
        case 0x15:	//ORA ZP,X
        {
            cpu6502->ZPX();
            cpu6502->ORA();
            cpu6502->incCycles(4);
            cpu6502->incPC(2);
            break;
        }
        case 0x16:	//ASL ZP,X
        {
            cpu6502->ZPX();
            cpu6502->ASL();
            cpu6502->incCycles(6);
            cpu6502->incPC(2);
            break;
        }
        case 0x17: printf("Opcode 0x17 hasn't been implemented yet\n"); break;
        case 0x18:	//CLC
        {
            cpu6502->IMP();
            cpu6502->CLC();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0x19:	//ORA Abs,Y
        {
            cpu6502->ABSY();
            cpu6502->ORA();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 3) ? cpu6502->incCycles(5) : cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0x1a: printf("Opcode 0x1a hasn't been implemented yet\n"); break;
        case 0x1b: printf("Opcode 0x1b hasn't been implemented yet\n"); break;
        case 0x1c: printf("Opcode 0x1c hasn't been implemented yet\n"); break;
        case 0x1d:	//ORA Abs,X
        {
            cpu6502->ABSX();
            cpu6502->ORA();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 3) ? cpu6502->incCycles(5) : cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0x1e:	//ASL Abs,X
        {
            cpu6502->ABSX();
            cpu6502->ASL();
            cpu6502->incCycles(6);
            cpu6502->incPC(3);
            break;
        }
        case 0x1f: printf("Opcode 0x1f hasn't been implemented yet\n"); break;
        case 0x20:	//JSR Abs
        {
            cpu6502->ABS();
            cpu6502->JSR();
            cpu6502->incCycles(3);
            break;
        }
        case 0x21:	//AND Ind,X
        {
            cpu6502->IDX_I();
            cpu6502->AND();
            cpu6502->incCycles(6);
            cpu6502->incPC(2);
            break;

        }
        case 0x22: printf("Opcode 0x22 hasn't been implemented yet\n"); break;
        case 0x23: printf("Opcode 0x23 hasn't been implemented yet\n"); break;
        case 0x24:	//BIT ZP
        {
            cpu6502->ZP();
            cpu6502->BIT();
            cpu6502->incCycles(3);
            cpu6502->incPC(2);
            break;
        }
        case 0x25:	//AND ZP
        {
            cpu6502->ZP();
            cpu6502->AND();
            cpu6502->incCycles(3);
            cpu6502->incPC(2);
            break;
        }
        case 0x26:	//ROL ZP
        {
            cpu6502->ZP();
            cpu6502->ROL();
            cpu6502->incCycles(5);
            cpu6502->incPC(2);
            break;
        }
        case 0x27: printf("Opcode 0x27 hasn't been implemented yet\n"); break;
        case 0x28:	//PLP
        {
            cpu6502->IMP();
            cpu6502->PLP();
            cpu6502->incCycles(4);
            cpu6502->incPC(1);
            break;
        }
        case 0x29:	//AND Im
        {
            cpu6502->IMM();
            cpu6502->AND();
            cpu6502->incCycles(2);
            cpu6502->incPC(2);
            break;
        }
        case 0x2a:	//ROL Acc
        {
            cpu6502->ACC();
            cpu6502->ROL();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0x2b: printf("Opcode 0x2b hasn't been implemented yet\n"); break;
        case 0x2c:	//BIT Abs
        {
            cpu6502->ABS();
            cpu6502->BIT();
            cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0x2d:	//AND Abs
        {
            cpu6502->ABS();
            cpu6502->AND();
            cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0x2e:	//ROL Abs
        {
            cpu6502->ABS();
            cpu6502->ROL();
            cpu6502->incCycles(6);
            cpu6502->incPC(3);
            break;
        }
        case 0x2f: printf("Opcode 0x2f hasn't been implemented yet\n"); break;
        case 0x30:	//BMI Rel
        {
            cpu6502->BMI();
            break;
        }
        case 0x31:	//AND Ind,Y
        {
            cpu6502->I_IDX();
            cpu6502->AND();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 2) ? cpu6502->incCycles(6) : cpu6502->incCycles(5);
            cpu6502->incPC(2);
            break;

        }
        case 0x32: printf("Opcode 0x32 hasn't been implemented yet\n"); break;
        case 0x33: printf("Opcode 0x33 hasn't been implemented yet\n"); break;
        case 0x34: printf("Opcode 0x34 hasn't been implemented yet\n"); break;
        case 0x35:	//AND ZP,X
        {
            cpu6502->ZPX();
            cpu6502->AND();
            cpu6502->incCycles(4);
            cpu6502->incPC(2);
            break;
        }
        case 0x36:	//ROL ZP,X
        {
            cpu6502->ZPX();
            cpu6502->ROL();
            cpu6502->incCycles(6);
            cpu6502->incPC(2);
            break;
        }
        case 0x37: printf("Opcode 0x37 hasn't been implemented yet\n"); break;
        case 0x38:	//SEC
        {
            cpu6502->IMP();
            cpu6502->SEC();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0x39:	//AND Abs,Y
        {
            cpu6502->ABSY();
            cpu6502->AND();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 3) ? cpu6502->incCycles(5) : cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0x3a: printf("Opcode 0x3a hasn't been implemented yet\n"); break;
        case 0x3b: printf("Opcode 0x3b hasn't been implemented yet\n"); break;
        case 0x3c: printf("Opcode 0x3c hasn't been implemented yet\n"); break;
        case 0x3d:	//AND Abs,X
        {
            cpu6502->ABSX();
            cpu6502->AND();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 3) ? cpu6502->incCycles(5) : cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0x3e:	//ROL Abs,X
        {
            cpu6502->ABSX();
            cpu6502->ROL();
            cpu6502->incCycles(7);
            cpu6502->incPC(3);
            break;
        }
        case 0x3f: printf("Opcode 0x3f hasn't been implemented yet\n"); break;
        case 0x40:	//RTI
        {
            cpu6502->IMP();
            cpu6502->RTI();
            cpu6502->incCycles(6);
            break;
        }
        case 0x41:	//EOR Ind,X
        {
            cpu6502->IDX_I();
            cpu6502->EOR();
            cpu6502->incCycles(6);
            cpu6502->incPC(2);
            break;

        }
        case 0x42: printf("Opcode 0x42 hasn't been implemented yet\n"); break;
        case 0x43: printf("Opcode 0x43 hasn't been implemented yet\n"); break;
        case 0x44: printf("Opcode 0x44 hasn't been implemented yet\n"); break;
        case 0x45:	//EOR ZP
        {
            cpu6502->ZP();
            cpu6502->EOR();
            cpu6502->incCycles(3);
            cpu6502->incPC(2);
            break;
        }
        case 0x46:	//LSR ZP
        {
            cpu6502->ZP();
            cpu6502->LSR();
            cpu6502->incCycles(5);
            cpu6502->incPC(2);
            break;
        }
        case 0x47: printf("Opcode 0x47 hasn't been implemented yet\n"); break;
        case 0x48:	//PHA
        {
            cpu6502->IMP();
            cpu6502->PHA();
            cpu6502->incCycles(3);
            cpu6502->incPC(1);
            break;
        }
        case 0x49:	//EOR Im
        {
            cpu6502->IMM();
            cpu6502->EOR();
            cpu6502->incCycles(2);
            cpu6502->incPC(2);
            break;
        }
        case 0x4a:	//LSR Acc
        {
            cpu6502->ACC();
            cpu6502->LSR();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0x4b: printf("Opcode 0x4b hasn't been implemented yet\n"); break;
        case 0x4c:	//JMP Abs
        {
            cpu6502->ABS();
            cpu6502->JMP();
            cpu6502->incCycles(3);
            break;
        }
        case 0x4d:	//EOR Abs
        {
            cpu6502->ABS();
            cpu6502->EOR();
            cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0x4e:	//LSR Abs
        {
            cpu6502->ABS();
            cpu6502->LSR();
            cpu6502->incCycles(6);
            cpu6502->incPC(3);
            break;
        }
        case 0x4f: printf("Opcode 0x4f hasn't been implemented yet\n"); break;
        case 0x50:	//BVC Rel
        {
            cpu6502->BVC();
            break;
        }
        case 0x51:	//EOR Ind,Y
        {
            cpu6502->I_IDX();
            cpu6502->EOR();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 2) ? cpu6502->incCycles(6) : cpu6502->incCycles(5);
            cpu6502->incPC(2);
            break;

        }
        case 0x52: printf("Opcode 0x52 hasn't been implemented yet\n"); break;
        case 0x53: printf("Opcode 0x53 hasn't been implemented yet\n"); break;
        case 0x54: printf("Opcode 0x54 hasn't been implemented yet\n"); break;
        case 0x55:	//EOR ZP,X
        {
            cpu6502->ZPX();
            cpu6502->EOR();
            cpu6502->incCycles(4);
            cpu6502->incPC(2);
            break;
        }
        case 0x56:	//LSR ZP,X
        {
            cpu6502->ZPX();
            cpu6502->LSR();
            cpu6502->incCycles(6);
            cpu6502->incPC(2);
            break;
        }
        case 0x57: printf("Opcode 0x57 hasn't been implemented yet\n"); break;
        case 0x58:	//CLI
        {
            cpu6502->IMP();
            cpu6502->CLI();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0x59:	//EOR Abs,Y
        {
            cpu6502->ABSY();
            cpu6502->EOR();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 3) ? cpu6502->incCycles(5) : cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0x5a: printf("Opcode 0x5a hasn't been implemented yet\n"); break;
        case 0x5b: printf("Opcode 0x5b hasn't been implemented yet\n"); break;
        case 0x5c: printf("Opcode 0x5c hasn't been implemented yet\n");  break;
        case 0x5d:	//EOR Abs,X
        {
            cpu6502->ABSX();
            cpu6502->EOR();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 3) ? cpu6502->incCycles(5) : cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0x5e:	//LSR Abs,X
        {
            cpu6502->ABSX();
            cpu6502->LSR();
            cpu6502->incCycles(6);
            cpu6502->incPC(3);
            break;
        }
        case 0x5f: printf("Opcode 0x5f hasn't been implemented yet\n"); break;
        case 0x60:	//RTS
        {
            cpu6502->IMP();
            cpu6502->RTS();
            cpu6502->incCycles(6);
            cpu6502->incPC(1);
            break;
        }
        case 0x61:	//ADC Ind,X
        {
            cpu6502->IDX_I();
            cpu6502->ADC();
            cpu6502->incCycles(6);
            cpu6502->incPC(2);
            break;
        }
        case 0x62: printf("Opcode 0x62 hasn't been implemented yet\n"); break;
        case 0x63: printf("Opcode 0x63 hasn't been implemented yet\n"); break;
        case 0x64: printf("Opcode 0x64 hasn't been implemented yet\n"); break;
        case 0x65:	//ADC ZP
        {
            cpu6502->ZP();
            cpu6502->ADC();
            cpu6502->incCycles(3);
            cpu6502->incPC(2);
            break;
        }
        case 0x66:	//ROR ZP
        {
            cpu6502->ZP();
            cpu6502->ROR();
            cpu6502->incCycles(5);
            cpu6502->incPC(2);
            break;
        }
        case 0x67: printf("Opcode 0x67 hasn't been implemented yet\n"); break;
        case 0x68:	//PLA
        {
            cpu6502->IMP();
            cpu6502->PLA();
            cpu6502->incCycles(4);
            cpu6502->incPC(1);
            break;
        }
        case 0x69:	//ADC Im
        {
            cpu6502->IMM();
            cpu6502->ADC();
            cpu6502->incCycles(2);
            cpu6502->incPC(2);
            break;
        }
        case 0x6a:	//ROR Acc
        {
            cpu6502->ACC();
            cpu6502->ROR();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0x6b: printf("Opcode 0x6b hasn't been implemented yet\n"); break;
        case 0x6c:	//JMP Ind
        {
            cpu6502->IND();
            cpu6502->JMP();
            cpu6502->incCycles(5);
            break;
        }
        case 0x6d:	//ADC Abs
        {
            cpu6502->ABS();
            cpu6502->ADC();
            cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0x6e:	//ROR Abs
        {
            cpu6502->ABS();
            cpu6502->ROR();
            cpu6502->incCycles(6);
            cpu6502->incPC(3);
            break;
        }
        case 0x6f: printf("Opcode 0x6f hasn't been implemented yet\n"); break;
        case 0x70:	//BVS Rel
        {
            cpu6502->BVS();
            break;
        }
        case 0x71:	//ADC Ind,Y
        {
            cpu6502->I_IDX();
            cpu6502->ADC();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 2) ? cpu6502->incCycles(6) : cpu6502->incCycles(5);
            cpu6502->incPC(2);
            break;
        }
        case 0x72: printf("Opcode 0x72 hasn't been implemented yet\n"); break;
        case 0x73: printf("Opcode 0x73 hasn't been implemented yet\n"); break;
        case 0x74: printf("Opcode 0x74 hasn't been implemented yet\n"); break;
        case 0x75:	//ADC ZP,X
        {
            cpu6502->ZPX();
            cpu6502->ADC();
            cpu6502->incCycles(4);
            cpu6502->incPC(2);
            break;
        }
        case 0x76:	//ROR ZP,X
        {
            cpu6502->ZPX();
            cpu6502->ROR();
            cpu6502->incCycles(6);
            cpu6502->incPC(2);
            break;
        }
        case 0x77: printf("Opcode 0x77 hasn't been implemented yet\n"); break;
        case 0x78:	//SEI
        {
            cpu6502->IMP();
            cpu6502->SEI();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0x79:	//ADC Abs,Y
        {
            cpu6502->ABSY();
            cpu6502->ADC();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 3) ? cpu6502->incCycles(5) : cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0x7a: printf("Opcode 0x7a hasn't been implemented yet\n"); break;
        case 0x7b: printf("Opcode 0x7b hasn't been implemented yet\n"); break;
        case 0x7c: printf("Opcode 0x7c hasn't been implemented yet\n"); break;
        case 0x7d:	//ADC Abs,X
        {
            cpu6502->ABSX();
            cpu6502->ADC();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 3) ? cpu6502->incCycles(5) : cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0x7e:	//ROR Abs,X
        {
            cpu6502->ABSX();
            cpu6502->ROR();
            cpu6502->incCycles(7);
            cpu6502->incPC(3);
            break;
        }
        case 0x7f: printf("Opcode 0x7f hasn't been implemented yet\n"); break;
        case 0x80: printf("Opcode 0x80 hasn't been implemented yet\n");  break;
        case 0x81:	//STA Ind,X
        {
            cpu6502->IDX_I();
            cpu6502->STA();
            cpu6502->incCycles(6);
            cpu6502->incPC(2);
            break;
        }
        case 0x82: printf("Opcode 0x82 hasn't been implemented yet\n"); break;
        case 0x83: printf("Opcode 0x83 hasn't been implemented yet\n"); break;
        case 0x84:	//STY ZP
        {
            cpu6502->ZP();
            cpu6502->STY();
            cpu6502->incCycles(3);
            cpu6502->incPC(2);
            break;
        }
        case 0x85:	//STA ZP
        {
            cpu6502->ZP();
            cpu6502->STA();
            cpu6502->incCycles(3);
            cpu6502->incPC(2);
            break;
        }
        case 0x86:	//STX ZP
        {
            cpu6502->ZP();
            cpu6502->STX();
            cpu6502->incCycles(3);
            cpu6502->incPC(2);
            break;
        }
        case 0x87: printf("Opcode 0x87 hasn't been implemented yet\n"); break;
        case 0x88:	//DEY
        {
            cpu6502->IMP();
            cpu6502->DEY();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0x89: printf("Opcode 0x89 hasn't been implemented yet\n"); break;
        case 0x8a:	//TXA
        {
            cpu6502->IMP();
            cpu6502->TXA();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0x8b: printf("Opcode 0x8b hasn't been implemented yet\n"); break;
        case 0x8c:	//STY Abs
        {
            cpu6502->ABS();
            cpu6502->STY();
            cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0x8d:	//STA Abs
        {
            cpu6502->ABS();
            cpu6502->STA();
            cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0x8e:	//STX Abs
        {
            cpu6502->ABS();
            cpu6502->STX();
            cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0x8f: printf("Opcode 0x8f hasn't been implemented yet\n"); break;
        case 0x90:	//BCC Rel
        {
            cpu6502->BCC();
            break;
        }
        case 0x91:	//STA Ind,Y
        {
            cpu6502->I_IDX();
            cpu6502->STA();
            cpu6502->incCycles(6);
            cpu6502->incPC(2);
            break;
        }
        case 0x92: printf("Opcode 0x92 hasn't been implemented yet\n"); break;
        case 0x93: printf("Opcode 0x93 hasn't been implemented yet\n"); break;
        case 0x94:	//STY ZP,X
        {
            cpu6502->ZPX();
            cpu6502->STY();
            cpu6502->incCycles(4);
            cpu6502->incPC(2);
            break;
        }
        case 0x95:	//STA ZP,X
        {
            cpu6502->ZPX();
            cpu6502->STA();
            cpu6502->incCycles(4);
            cpu6502->incPC(2);
            break;
        }
        case 0x96:	//STX ZP,Y
        {
            cpu6502->ZPY();
            cpu6502->STX();
            cpu6502->incCycles(4);
            cpu6502->incPC(2);
            break;
        }
        case 0x97: printf("Opcode 0x97 hasn't been implemented yet\n"); break;
        case 0x98:	//TYA
        {
            cpu6502->IMP();
            cpu6502->TYA();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0x99:	//STA Abs,Y
        {
            cpu6502->ABSY();
            cpu6502->STA();
            cpu6502->incCycles(5);
            cpu6502->incPC(3);
            break;
        }
        case 0x9a:	//TXS
        {
            cpu6502->IMP();
            cpu6502->TXS();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0x9b: printf("Opcode 0x9b hasn't been implemented yet\n"); break;
        case 0x9c: printf("Opcode 0x9c hasn't been implemented yet\n"); break;
        case 0x9d:	//STA Abs,X
        {
            cpu6502->ABSX();
            cpu6502->STA();
            cpu6502->incCycles(5);
            cpu6502->incPC(3);
            break;
        }
        case 0x9e: printf("Opcode 0x9e hasn't been implemented yet\n"); break;
        case 0x9f: printf("Opcode 0x9f hasn't been implemented yet\n"); break;
        case 0xa0:	//LDY Im
        {
            cpu6502->IMM();
            cpu6502->LDY();
            cpu6502->incCycles(2);
            cpu6502->incPC(2);
            break;
        }
        case 0xa1:	//LDA Ind,X
        {
            cpu6502->IDX_I();
            cpu6502->LDA();
            cpu6502->incCycles(6);
            cpu6502->incPC(2);
            break;
        }
        case 0xa2:	//LDX Im
        {
            cpu6502->IMM();
            cpu6502->LDX();
            cpu6502->incCycles(2);
            cpu6502->incPC(2);
            break;
        }
        case 0xa3: printf("Opcode 0xa3 hasn't been implemented yet\n"); break;
        case 0xa4:	//LDY ZP
        {
            cpu6502->ZP();
            cpu6502->LDY();
            cpu6502->incCycles(3);
            cpu6502->incPC(2);
            break;
        }
        case 0xa5:	//LDA ZP
        {
            cpu6502->ZP();
            cpu6502->LDA();
            cpu6502->incCycles(3);
            cpu6502->incPC(2);
            break;
        }
        case 0xa6:	//LDX ZP
        {
            cpu6502->ZP();
            cpu6502->LDX();
            cpu6502->incCycles(3);
            cpu6502->incPC(2);
            break;
        }
        case 0xa7: printf("Opcode 0xa7 hasn't been implemented yet\n"); break;
        case 0xa8:	//TAY
        {
            cpu6502->IMP();
            cpu6502->TAY();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0xa9:	//LDA Im
        {
            cpu6502->IMM();
            cpu6502->LDA();
            cpu6502->incCycles(2);
            cpu6502->incPC(2);
            break;
        }
        case 0xaa:	//TAX
        {
            cpu6502->IMP();
            cpu6502->TAX();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0xab: printf("Opcode 0xab hasn't been implemented yet\n"); break;
        case 0xac:	//LDY Abs
        {
            cpu6502->ABS();
            cpu6502->LDY();
            cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0xad:	//LDA Abs
        {
            cpu6502->ABS();
            cpu6502->LDA();
            cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0xae:	//LDX Abs
        {
            cpu6502->ABS();
            cpu6502->LDX();
            cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0xaf: printf("Opcode 0xaf hasn't been implemented yet\n"); break;
        case 0xb0:	//BCS Rel
        {
            cpu6502->BCS();
            break;
        }
        case 0xb1:	//LDA Ind,Y
        {
            cpu6502->I_IDX();
            cpu6502->LDA();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 2) ? cpu6502->incCycles(6) : cpu6502->incCycles(5);
            cpu6502->incPC(2);
            break;
        }
        case 0xb2: printf("Opcode 0xb2 hasn't been implemented yet\n"); break;
        case 0xb3: printf("Opcode 0xb3 hasn't been implemented yet\n"); break;
        case 0xb4:	//LDY ZP,X
        {
            cpu6502->ZPX();
            cpu6502->LDY();
            cpu6502->incCycles(4);
            cpu6502->incPC(2);
            break;
        }
        case 0xb5:	//LDA ZP,X
        {
            cpu6502->ZPX();
            cpu6502->LDA();
            cpu6502->incCycles(4);
            cpu6502->incPC(2);
            break;
        }
        case 0xb6:	//LDX ZP,Y
        {
            cpu6502->ZPY();
            cpu6502->LDX();
            cpu6502->incCycles(4);
            cpu6502->incPC(2);
            break;
        }
        case 0xb7: printf("Opcode 0xb7 hasn't been implemented yet\n"); break;
        case 0xb8:	//CLV
        {
            cpu6502->IMP();
            cpu6502->CLV();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0xb9:	//LDA Abs,Y
        {
            cpu6502->ABSY();
            cpu6502->LDA();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 3) ? cpu6502->incCycles(5) : cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0xba:	//TSX
        {
            cpu6502->IMP();
            cpu6502->TSX();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0xbb: printf("Opcode 0xbb hasn't been implemented yet\n"); break;
        case 0xbc:	//LDY Abs,X
        {
            cpu6502->ABSX();
            cpu6502->LDY();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 3) ? cpu6502->incCycles(5) : cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0xbd:	//LDA Abs,X
        {
            cpu6502->ABSX();
            cpu6502->LDA();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 3) ? cpu6502->incCycles(5) : cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0xbe:	//LDX Abs,Y
        {
            cpu6502->ABSY();
            cpu6502->LDX();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 3) ? cpu6502->incCycles(5) : cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0xbf: printf("Opcode 0xbf hasn't been implemented yet\n"); break;
        case 0xc0:	//CPY Im
        {
            cpu6502->IMM();
            cpu6502->CPY();
            cpu6502->incCycles(2);
            cpu6502->incPC(2);
            break;
        }
        case 0xc1:	//CMP Ind,X
        {
            cpu6502->IDX_I();
            cpu6502->CMP();
            cpu6502->incCycles(6);
            cpu6502->incPC(2);
            break;
        }
        case 0xc2: printf("Opcode 0xc2 hasn't been implemented yet\n"); break;
        case 0xc3: printf("Opcode 0xc3 hasn't been implemented yet\n"); break;
        case 0xc4:	//CPY ZP
        {
            cpu6502->ZP();
            cpu6502->CPY();
            cpu6502->incCycles(3);
            cpu6502->incPC(2);
            break;
        }
        case 0xc5:	//CMP ZP
        {
            cpu6502->ZP();
            cpu6502->CMP();
            cpu6502->incCycles(3);
            cpu6502->incPC(2);
            break;
        }
        case 0xc6:	//DEC ZP
        {
            cpu6502->ZP();
            cpu6502->DEC();
            cpu6502->incCycles(5);
            cpu6502->incPC(2);
            break;
        }
        case 0xc7: printf("Opcode 0xc7 hasn't been implemented yet\n"); break;
        case 0xc8:	//INY
        {
            cpu6502->IMP();
            cpu6502->INY();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0xc9:	//CMP Im
        {
            cpu6502->IMM();
            cpu6502->CMP();
            cpu6502->incCycles(2);
            cpu6502->incPC(2);
            break;
        }
        case 0xca:	//DEX
        {
            cpu6502->IMP();
            cpu6502->DEX();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0xcb: printf("Opcode 0xcb hasn't been implemented yet\n"); break;
        case 0xcc:	//CPY Abs
        {
            cpu6502->ABS();
            cpu6502->CPY();
            cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0xcd:	//CMP Abs
        {
            cpu6502->ABS();
            cpu6502->CMP();
            cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0xce:	//DEC Abs
        {
            cpu6502->ABS();
            cpu6502->DEC();
            cpu6502->incCycles(6);
            cpu6502->incPC(3);
            break;
        }
        case 0xcf: printf("Opcode 0xcf hasn't been implemented yet\n"); break;
        case 0xd0:	//BNE Rel
        {
            cpu6502->BNE();
            break;
        }
        case 0xd1:	//CMP Ind,Y
        {
            cpu6502->I_IDX();
            cpu6502->CMP();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 2) ? cpu6502->incCycles(6) : cpu6502->incCycles(5);
            cpu6502->incPC(2);
            break;
        }
        case 0xd2: printf("Opcode 0xd2 hasn't been implemented yet\n"); break;
        case 0xd3: printf("Opcode 0xd3 hasn't been implemented yet\n"); break;
        case 0xd4: printf("Opcode 0xd4 hasn't been implemented yet\n"); break;
        case 0xd5:	//CMP ZP,X
        {
            cpu6502->ZPX();
            cpu6502->CMP();
            cpu6502->incCycles(4);
            cpu6502->incPC(2);
            break;
        }
        case 0xd6:	//DEC ZP,X
        {
            cpu6502->ZPX();
            cpu6502->DEC();
            cpu6502->incCycles(6);
            cpu6502->incPC(2);
            break;
        }
        case 0xd7: printf("Opcode 0xd7 hasn't been implemented yet\n"); break;
        case 0xd8:	//CLD
        {
            cpu6502->IMP();
            cpu6502->CLD();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0xd9:	//CMP Abs,Y
        {
            cpu6502->ABSY();
            cpu6502->CMP();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 3) ? cpu6502->incCycles(5) : cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0xda: printf("Opcode 0xda hasn't been implemented yet\n"); break;
        case 0xdb: printf("Opcode 0xdb hasn't been implemented yet\n"); break;
        case 0xdc: printf("Opcode 0xdc hasn't been implemented yet\n"); break;
        case 0xdd:	//CMP Abs,X
        {
            cpu6502->ABSX();
            cpu6502->CMP();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 3) ? cpu6502->incCycles(5) : cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0xde:	//DEC Abs,X
        {
            cpu6502->ABSX();
            cpu6502->DEC();
            cpu6502->incCycles(7);
            cpu6502->incPC(3);
            break;
        }
        case 0xdf: printf("Opcode 0xdf hasn't been implemented yet\n"); break;
        case 0xe0:	//CPX Im
        {
            cpu6502->IMM();
            cpu6502->CPX();
            cpu6502->incCycles(2);
            cpu6502->incPC(2);
            break;
        }
        case 0xe1:	//SBC Ind,X
        {
            cpu6502->IDX_I();
            cpu6502->SBC();
            cpu6502->incCycles(6);
            cpu6502->incPC(2);
            break;
        }
        case 0xe2: printf("Opcode 0xe2 hasn't been implemented yet\n"); break;
        case 0xe3: printf("Opcode 0xe3 hasn't been implemented yet\n"); break;
        case 0xe4:	//CPX ZP
        {
            cpu6502->ZP();
            cpu6502->CPX();
            cpu6502->incCycles(3);
            cpu6502->incPC(2);
            break;
        }
        case 0xe5:	//SBC ZP
        {
            cpu6502->ZP();
            cpu6502->SBC();
            cpu6502->incCycles(3);
            cpu6502->incPC(2);
            break;
        }
        case 0xe6:	//INC ZP
        {
            cpu6502->ZP();
            cpu6502->INC();
            cpu6502->incCycles(5);
            cpu6502->incPC(2);
            break;
        }
        case 0xe7: printf("Opcode 0xe7 hasn't been implemented yet\n"); break;
        case 0xe8:	//INX
        {
            cpu6502->IMP();
            cpu6502->INX();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0xe9:	//SBC Im
        {
            cpu6502->IMM();
            cpu6502->SBC();
            cpu6502->incCycles(2);
            cpu6502->incPC(2);
            break;
        }
        case 0xea:	//NOP
        {
            cpu6502->IMP();
            cpu6502->NOP();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0xeb: printf("Opcode 0xeb hasn't been implemented yet\n"); break;
        case 0xec:	//CPX Abs
        {
            cpu6502->ABS();
            cpu6502->CPX();
            cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0xed:	//SBC Abs
        {
            cpu6502->ABS();
            cpu6502->SBC();
            cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0xee:	//INC Abs
        {
            cpu6502->ABS();
            cpu6502->INC();
            cpu6502->incCycles(6);
            cpu6502->incPC(3);
            break;
        }
        case 0xef: printf("Opcode 0xef hasn't been implemented yet\n"); break;
        case 0xf0:	//BEQ Rel
        {
            cpu6502->BEQ();
            break;
        }
        case 0xf1:	//SBC Ind,Y
        {
            cpu6502->I_IDX();
            cpu6502->SBC();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 2) ? cpu6502->incCycles(6) : cpu6502->incCycles(5);
            cpu6502->incPC(2);
            break;
        }
        case 0xf2: printf("Opcode 0xf2 hasn't been implemented yet\n"); break;
        case 0xf3: printf("Opcode 0xf3 hasn't been implemented yet\n"); break;
        case 0xf4: printf("Opcode 0xf4 hasn't been implemented yet\n"); break;
        case 0xf5:	//SBC ZP,X
        {
            cpu6502->ZPX();
            cpu6502->SBC();
            cpu6502->incCycles(4);
            cpu6502->incPC(2);
            break;
        }
        case 0xf6:	//INC ZP,X
        {
            cpu6502->ZPX();
            cpu6502->INC();
            cpu6502->incCycles(6);
            cpu6502->incPC(2);
            break;
        }
        case 0xf7: printf("Opcode 0xf7 hasn't been implemented yet\n"); break;
        case 0xf8:	//SED
        {
            cpu6502->IMP();
            cpu6502->SED();
            cpu6502->incCycles(2);
            cpu6502->incPC(1);
            break;
        }
        case 0xf9:	//SBC Abs,Y
        {
            cpu6502->ABSY();
            cpu6502->SBC();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 3) ? cpu6502->incCycles(5) : cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0xfa: printf("Opcode 0xfa hasn't been implemented yet\n"); break;
        case 0xfb: printf("Opcode 0xfb hasn't been implemented yet\n"); break;
        case 0xfc: printf("Opcode 0xfc hasn't been implemented yet\n"); break;
        case 0xfd:	//SBC Abs,X
        {
            cpu6502->ABSX();
            cpu6502->SBC();
            cpu6502->hasCrossPage(cpu6502->getPC(), cpu6502->getPC() + 3) ? cpu6502->incCycles(5) : cpu6502->incCycles(4);
            cpu6502->incPC(3);
            break;
        }
        case 0xfe:	//INC Abs,X
        {
            cpu6502->ABSX();
            cpu6502->INC();
            cpu6502->incCycles(7);
            cpu6502->incPC(3);
            break;
        }
        case 0xff: printf("Opcode 0xff hasn't been implemented yet\n"); break;
    }
}

void machine::debugger() {
    printf("PC: %04x\t opcode: %02x %02x %02x\t A: %02x X: %02x Y: %02x\t flags: %02x\t cycles: %d\t stack: %02x %02x %02x %02x %02x %02x | %02x\t %.02f%%\n",
            cpu6502->getPC(), cpu6502->getOpcode()[0], cpu6502->getOpcode()[1], cpu6502->getOpcode()[2],
            cpu6502->getA(), cpu6502->getX(), cpu6502->getY(),
            cpu6502->get_processor_status(), cpu6502->getCycles(),
            cpu6502->read(0x1ff), cpu6502->read(0x1fe), cpu6502->read(0x1fd), cpu6502->read(0x1fc), cpu6502->read(0x1fb), cpu6502->read(0x1fa),
            cpu6502->getSP(),
           (double)cpu6502->ins_count/8991*100);

    if(cpu6502->get_processor_status() != P[cpu6502->ins_count]) {
        printf("FLAG PROBLEM\n");
        cpu6502->isOn = false;
    }
    if(cpu6502->getX() != X[cpu6502->ins_count]) {
        printf("X PROBLEM\n");
        cpu6502->isOn = false;
    }
    if(cpu6502->getY() != Y[cpu6502->ins_count]) {
        printf("Y PROBLEM\n");
        cpu6502->isOn = false;
    }
    if(cpu6502->getA() != A[cpu6502->ins_count]) {
        printf("A PROBLEM\n");
        cpu6502->isOn = false;
    }
    if(cpu6502->getPC() != PC[cpu6502->ins_count]) {
        printf("PC PROBLEM\n");
        cpu6502->isOn = false;
    }
    if(cpu6502->getSP() != SP[cpu6502->ins_count]) {
        printf("SP PROBLEM\n");
        cpu6502->isOn = false;
    }
    ++cpu6502->ins_count;
}

machine::~machine() {
    log.close();
}
