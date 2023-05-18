#include <iostream>
#include <cstdlib>

#include "ac_int.h"
#include "ALU.h"
#include "Processor.h"

int main() {

// Initialize memories
ac_int<32,false> instruction_mem[256];
ac::init_array<AC_VAL_0>(instruction_mem, 256);
ac_int<32,true> data_mem[256];
ac::init_array<AC_VAL_0>(data_mem, 256);    

// Test:
instruction_mem[50] = 0b00000100011000000000001000010011; // ADDI R4, R0, 70
instruction_mem[51] = 0b00000000010000100010001010000011; // LW   R5, mem[ R4 + 1 ]
instruction_mem[52] = 0b00000000010100100000110001100011; // BEQ  R4, R5, 24
instruction_mem[53] = 0b00000000011000100010010000100011; // SW   mem[r4 + 2] = R6
instruction_mem[54] = 0b00000000000000000000000110010011; // ADDI R3, R0, 0
instruction_mem[55] = 0b10101010100000011011001100110111; // LUI   R6, 10101010100000011011
instruction_mem[56] = 0b00000000011000111111001110110011; // AND  R7, R7, R6

Processor processor = Processor();

processor.run(instruction_mem, data_mem);

}