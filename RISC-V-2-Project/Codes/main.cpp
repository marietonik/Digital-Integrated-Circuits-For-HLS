#include <iostream>
#include <cstdlib>

#include "ac_int.h"
#include "ALU.h"
#include "Processor.h"

int main() {

ac_int<32,false> i = 50;

ac_int<32,false> instruction_mem[256];
ac::init_array<AC_VAL_0>(instruction_mem, 256);
ac_int<32,true> data_mem[256];
ac::init_array<AC_VAL_0>(data_mem, 256); 

data_mem[100] = 100;
data_mem[101] = 101; 

// Test:
instruction_mem[50] = 0b00000000100100011000010100110011; // ADD   R[10], R[3],   R[9]
instruction_mem[51] = 0b01000000101001100000000010110011; // SUB   R[1],  R[12],  R[10]
instruction_mem[52] = 0b00000000001101001111001110110011; // AND   R[7],  R[9],   R[3]
instruction_mem[53] = 0b00000000010100011110001110110011; // OR    R[7],  R[3],   R[5]
instruction_mem[54] = 0b00000000100101010100010010110011; // XOR   R[9],  R[10],  R[9]
instruction_mem[55] = 0b00000000000100011010010100110011; // SLT   R[10], R[3],   R[1]
instruction_mem[56] = 0b00000000000100010011000010110011; // SLTU  R[1],  R[2],   R[1]
instruction_mem[57] = 0b01000000011100011101010010110011; // SRA   R[9],  R[3],   R[7]
instruction_mem[58] = 0b00000000001100001101000110110011; // SRL   R[3],  R[1],   R[3]
instruction_mem[59] = 0b00000000100000101001010100110011; // SLL   R[10], R[5],   R[8]
instruction_mem[60] = 0b00000010010000111000000010110011; // MUL   R[1],  R[7],   R[4]
instruction_mem[61] = 0b00000000000000000000000110010011; // ADDI  R[3],  R[0],   0
instruction_mem[62] = 0b00001110001100101111001100010011; // ANDI  R[6],  R[5],   001010101011
instruction_mem[63] = 0b00001110001100101110010000010011; // ORI   R[8],  R[5],   010010000011
instruction_mem[64] = 0b01011001001100110100001010010011; // XORI  R[5],  R[6],   010110010011
instruction_mem[65] = 0b00111100010100100010010100010011; // SLTI  R[10], R[4],   001111000101
instruction_mem[66] = 0b11111100010100101011010110010011; // SLTIU R[11], R[5],   111111000101
instruction_mem[67] = 0b01000000001101100101010010010011; // SRAI  R[12], R[9],   00011
instruction_mem[68] = 0b00000000001100100101010100010011; // SRLI  R[10], R[4],   00011
instruction_mem[69] = 0b00000000001100100001010110010011; // SLLI  R[11], R[4],   00011
instruction_mem[70] = 0b10111010100000001001001100110111; // LUI   R[6],  10111010100000001001
instruction_mem[71] = 0b00000000000000000000000100010111; // AUIPC R[2],  00000000000000000000
instruction_mem[72] = 0b00000000010000100010001010000011; // LW    R[5],  mem[R[4] + 1]
instruction_mem[73] = 0b00000000011000100010010000100011; // SW    mem[R[4] + 2],   R6
instruction_mem[74] = 0b00000000010100100000110001100011; // BEQ   R4,    R5,     24
instruction_mem[75] = 0b11111110010100100001011011100011; // BNE   R4,    R5,    -20
instruction_mem[76] = 0b00000000010100100100101001100011; // BLT   R[4],  R[5],   20
instruction_mem[77] = 0b00000000000000010101010001100011; // BGE   R[2],  R[0],   000000001000
instruction_mem[78] = 0b00000000010100100110001001100011; // BLTU  R[4],  R[5],   8
instruction_mem[79] = 0b11111110010100100111100011100011; // BGEU  R4,    R5,    -16
instruction_mem[80] = 0b00000000000000000000000000000000; // NOPs
instruction_mem[81] = 0b11111111010111111111010111101111; // JAL   R[11],        -13
instruction_mem[82] = 0b00000000000000110000000001100111; // JR    R[6]
instruction_mem[83] = 0b00000000000000001000100101100111; // JALR  R[18], R1,     0 

Processor processor;

while (instruction_mem[i] != 0) {

processor.run(instruction_mem, data_mem);
i++;

}

cout << "Execution finished." << "\n" << endl;

return 0;

}