//
// Created by jorge on 2020/6/16.
//

#ifndef dis_h
#define dis_h

#include <string>
#include <fstream>
#include <vector>

class disassembler
{
public:
    disassembler(std::string path);
    ~disassembler();

    void disassemble(int offset);

    void hexDump();

private:
    std::fstream file;

    std::fstream opcodes;

public:
    std::vector<std::string> lineParser(std::string line, char character);
};

#endif