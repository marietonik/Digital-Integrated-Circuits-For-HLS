#include <iostream>
#include <cstdlib>

#include "ac_int.h"
#include "ALU.h"
#include "Processor.h"

int main() {
ac_int<32,true> instructions = 0b000100011000000000001000010011; // ADDI R4, R0, 70
Processor processor_obj;
processor_obj.decode_instruction(instructions);
}