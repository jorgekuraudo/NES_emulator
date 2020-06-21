//
// Created by jorge on 2020/6/16.
//

#include "../include/disassembler.h"
#include <iostream>
#include <cstdio>

disassembler::disassembler(std::string path)
{
    //open file to disassemble
    file.open(path, std::ios::in | std::ios::binary);
    if (file.is_open()) {
        std::cout << "File in " << path << " opened successfully" << std::endl;
    }
    else {
        std::cout << "Failed to open " << path << std::endl;
    }

    //open file containing opcodes for the 6502
    opcodes.open("./others/6502ops.csv", std::ios::in | std::ios::binary);
    if (opcodes.is_open()) {
        std::cout << "Opcodes" << " loaded successfully" << std::endl;
    }
    else {
        std::cout << "Failed to open opcodes file" << std::endl;
    }
}

void disassembler::disassemble(int offset)
{
    //load csv in memory (a vector)
    std::vector<std::vector<std::string>> codes{};
    std::string line{};
    while (std::getline(opcodes, line)) {
        std::vector<std::string> parsedLine = this->lineParser(line, ',');
        codes.push_back(parsedLine);
    }

    //opcodes are in order but many are not implemented so there are gaps between some codes, what makes using the opcode as the
    //index of the vector impossible, one solution could be using std::find for each opcode, but this, specially for large binaries,
    //is highly inefficient, instead it is better to add some zeroes in between elements
    std::vector<std::vector<std::string>> codes_copy = codes;
    codes.clear();
    for (int i = 1; i < codes_copy.size(); ++i) {
        //check for two consecutive opcodes, if they're not, add the necessary zeros
        int difference = stoi(codes_copy[i][0], 0, 16) - stoi(codes_copy[i-1][0], 0, 16);
        if (difference > 1) {
            codes.push_back(codes_copy[i-1]);
            for (int j = 0; j < difference - 1; ++j) {
                codes.push_back({ "-", "-", "-", "-", "-", "-"});
            }
        }
        else {
            codes.push_back(codes_copy[i-1]);
        }
    }
    //add last element (not included in the loop)
    codes.push_back(codes_copy[codes_copy.size() - 1]);

    //get bytes from file
    char buffer[3];
    while (!file.eof())
    {
        //advance to "offset" byte and read
        file.seekg(offset);
        file.read(buffer, 3);
        int opcode = buffer[0] & 0xff;
        //print instruction to screen - address, nemonics, bytes, mode, cycles, and flags
        printf("%04x ", offset); //address
        printf("%s ", codes[opcode][1].c_str()); //nemonics
        int instruction_bytes = stoi(codes[opcode][3], 0, 16);
        for (int i = 0; i < instruction_bytes; ++i) {
            printf("%02x ", buffer[i] & 0xff);
        }
        printf("   \t%s\t ", codes[opcode][2].c_str());
        printf("%s\n ", codes[opcode][5].c_str());
        //advance to next intruction bytes
        offset += instruction_bytes;
    }
}

void disassembler::hexDump()
{
    char buffer[1];
    int currentByte = 0;

    //print offset
    printf("Offset\t\t");
    for (int i = 0x00; i <= 0x0f; ++i) {
        printf("%02x ", i);
    } printf("\n\n\n");

    while (!file.eof()) {
        //format output in rows of 16 columns:
        printf("%06x\t\t", currentByte);
        for (int column = 0x00; column <= 0x0f; ++column) {
            file.seekg(currentByte);
            file.read(buffer, 1);
            printf("%02x ", buffer[0] & 0xff);
            ++currentByte;
        } printf("\n");
    }
}

std::vector<std::string> disassembler::lineParser(std::string line, char character)
{
    std::vector<std::string> result{};
    std::string temp{};
    for (auto& c : line) {
        if (c != character) {
            temp += c;
        }
        else {
            result.push_back(temp);
            temp.clear();
        }
    }
    //add last element
    result.push_back(temp);
    return result;
}

disassembler::~disassembler() {
    file.close();
    opcodes.close();
}