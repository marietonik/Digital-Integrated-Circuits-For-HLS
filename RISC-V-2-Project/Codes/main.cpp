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

instruction_mem[50] = 0b00000000100100011000010100110011; // ADD  R[10], R[3],  R[9]
instruction_mem[51] = 0b01000000101001100000000010110011; // SUB  R[1],  R[12], R[10]
instruction_mem[52] = 0b00000000001101001111001110110011; // AND  R[7],  R[9],  R[3]
instruction_mem[53] = 0b00000000010100011110001110110011; // OR   R[7],  R[3],  R[5]
instruction_mem[54] = 0b00000000100101010100010010110011; // XOR  R[9],  R[10], R[9]
instruction_mem[55] = 0b00000000000100011010010100110011; // SLT  R[10], R[3],  R[1]
instruction_mem[56] = 0b00000000000100010011000010110011; // SLTU R[1],  R[2],  R[1]
instruction_mem[57] = 0b01000000011100011101010010110011; // SRA  R[9],  R[3],  R[7]
instruction_mem[58] = 0b00000000001100001101000110110011; // SRL  R[3],  R[1],  R[1]
instruction_mem[59] = 0b00000000100000101001010100110011; // SLL  R[10], R[5],  R[8]
instruction_mem[60] = 0b00000010010000111000000010110011; // MUL  R[1],  R[4],  R[7]


Processor processor;

while (instruction_mem[i] != 0) {

processor.run(instruction_mem, data_mem);
i++;

}

cout << "Execution finished." << "\n" << endl;

return 0;

}