#include <iostream>
#include <cstdlib>
#include <bitset>

#include "ac_int.h"
#include "ac_fixed.h"
#include "ALU.h"
#include "Processor.h"

#include "mc_scverify.h"
using namespace std;

class Processor
{
    public:
    ac_int<32,false> PC;
    ac_int<32,true> R[32];
    ac_int<1,false> invalid_instruction;
    ac_int<3,false> control_mem;
    ac_int<32,false> destination_mem;
    ac_int<32,true> value;
    ALU alu;

    Processor()
    {
        PC = 200;
        R[0] = 0;
        invalid_instruction = 0;
        std::cout << "\n" << "Initial PC is: " << PC << "\n" << std::endl;
    }

    ac_int<32,true> i_immediate(ac_int<32,true> instruction){

        ac_int<32,true> imm = 0;
        ac_int<1,false> imm_part_1 = instruction[20];
        ac_int<4,false> imm_part_2 = instruction.slc<4>(21);
        ac_int<6,false> imm_part_3 = instruction.slc<6>(25);
        ac_int<1,false> imm_part_4 = instruction[31];
        imm[0] = imm_part_1;
        imm.set_slc(1,imm_part_2);
        imm.set_slc(5,imm_part_3);
        imm.set_slc(11,imm_part_4);

        return imm;
    }

    ac_int<32,true> s_immediate(ac_int<32,true> instruction){

        ac_int<32,true> imm = 0;
        ac_int<1,false> imm_part_1 = instruction[7];
        ac_int<4,false> imm_part_2 = instruction.slc<4>(8);
        ac_int<6,false> imm_part_3 = instruction.slc<6>(25);
        ac_int<1,false> imm_part_4 = instruction[31];
        imm[0] = imm_part_1;
        imm.set_slc(1,imm_part_2);
        imm.set_slc(5,imm_part_3);
        imm.set_slc(11,imm_part_4);

        return imm;
    }

    ac_int<32,true> b_immediate(ac_int<32,true> instruction){

        ac_int<32,true> imm = 0;
        ac_int<4,false> imm_part_1 = instruction.slc<4>(8);
        ac_int<6,false> imm_part_2 = instruction.slc<6>(25);
        ac_int<1,false> imm_part_3 = instruction[7];
        ac_int<1,false> imm_part_4 = instruction[31];
        imm.set_slc(1,imm_part_1);
        imm.set_slc(5,imm_part_2);
        imm[11] = imm_part_3;
        imm.set_slc(12,imm_part_4);

        return imm;
    }
   
    ac_int<32,true> u_immediate(ac_int<32,true> instruction){

        ac_int<32,true> imm = 0;
        ac_int<8,false> imm_part_1 = instruction.slc<8>(12);
        ac_int<11,false> imm_part_2 = instruction.slc<11>(20);
        ac_int<1,false> imm_part_3 = instruction[31];
        imm.set_slc(12,imm_part_1);
        imm.set_slc(20,imm_part_2);
        imm[31] = imm_part_3;

        return imm;
    }

    ac_int<32,true> j_immediate(ac_int<32,true> instruction){
        
        ac_int<32,true> imm = 0;
        ac_int<4,false> imm_part_1 = instruction.slc<4>(21);
        ac_int<6,false> imm_part_2 = instruction.slc<6>(25);
        ac_int<1,false> imm_part_3 = instruction[20];
        ac_int<8,false> imm_part_4 = instruction.slc<8>(12);
        ac_int<1,false> imm_part_5 = instruction[31];
        imm.set_slc(1,imm_part_1);
        imm.set_slc(5,imm_part_2);
        imm[11] = imm_part_3;
        imm.set_slc(12,imm_part_4);
        imm.set_slc(20,imm_part_5);

        return imm;
    }
    

    ac_int<1,false> run(ac_int<32,false> instruction_mem[256], ac_int<32,true> data_mem[256]){
        
        ac_int<32,true> temp;
        
        decode_instruction(instruction_mem[PC.slc<30>(2)]);
        
        if(control_mem == 2){
            
            temp = data_mem[value];
            R[destination_mem] = temp;
            std::cout << "Word: " << R[destination_mem] << " written in R[" << destination_mem << "]\n" << std::endl;
        }

        else if(control_mem == 3){
            
            temp = destination_mem;
            data_mem[value] = destination_mem;
            std::cout << "Word: " << data_mem[value] << " written in mem[" << destination_mem << "]\n" << std::endl;
        }

        else{

            std::cout << "Continue...\n" << std::endl;
        }

        return 1;        
    }

ac_int<32,true> execute(ac_int<4,false> ALU_opcode, ac_int<32,false> operation_1, ac_int<32,false> operation_2, ac_int<32,false> destination, ac_int<3,false> control){
        
        ac_int<32,true> result;
        ac_int<32,true> value_alu;

        control_mem = control;
        destination_mem = destination;

        if(control == 1){ 
            result = alu.operations(ALU_opcode, operation_1, operation_2);
            value_alu = result;
            R[destination_mem] = value_alu;
            std::cout << "\n" << "Result: "<< result << " written in register R[" << destination_mem << "] \n" << std::endl;
        }

        else if(control == 2){
            operation_1 = (operation_1).slc<30>(2);
            value = (operation_2).slc<30>(2) + operation_1;
            std::cout << "\n" << "Load word from memory address: mem[Operant 2 + "<< operation_1 << "] and write to R[" << destination_mem << "] \n" << std::endl;
            if((value > 256) || (value < 0)){
                std::cout << "Error: Memory out of bounds!\n" << endl;
                value = 0;
            }
        }

        else if(control == 3){
            operation_2 = (operation_2).slc<30>(2);
            value = (operation_1).slc<30>(2) + operation_2;
            std::cout << "\n" << "Store word from destination register and write to memory: mem[Operant 1 + " << operation_2 << "] \n" << std::endl;
            
            if((value > 256) || (value < 0)){
                std::cout << "Error: Memory out of bounds!\n" << std::endl;
                value = 0;
            }

            else if((destination_mem > 256) || (destination_mem < 0)){
                std::cout << "Error: Register memory out of bounds!\n" << std::endl;
                destination_mem = 0;
            }
        }

        else{
            std::cout << "Continue...\n" << std::endl;
        }

        return 0;
    }

    ac_int<1,false> check_decode(ac_int<1,false> invalid_instruction, ac_int<7,false> opcode, ac_int<4,false> ALU_opcode, ac_int<3,false> control){

        if(invalid_instruction == 0){
            std::cout << "Opcode: " << opcode << std::endl;
            std::cout << "ALU opcode: " << ALU_opcode << std::endl;
            std::cout << "Control: " << control << std::endl;
        }

        else{
            std::cout << "Break due to invalid parameters..." << std::endl;
        }

        return invalid_instruction;
    }

    ac_int<32,true> decode_instruction(ac_int<32,true> instruction){

        ac_int<4,false> ALU_opcode;
        ac_int<32,false> destination;
        ac_int<32,false> operation_1;
        ac_int<32,false> operation_2;
        ac_int<3,false> control;
        
        ac_int<1,false> invalid_instruction = 0;
        ac_int<7,false> opcode = instruction.slc<7>(0);

        switch(opcode){

            case 51:
            { 
                ac_int<5,false> rd = instruction.slc<5>(7);
                ac_int<3,false> func3 = instruction.slc<3>(12);
                ac_int<5,false> rs1 = instruction.slc<5>(15);
                ac_int<5,false> rs2 = instruction.slc<5>(20);
                ac_int<7,false> func7 = instruction.slc<7>(25);

                if(func7 == 0){
                    
                    if(func3 == 0){
                        std::cout << "ADD Instruction" << std::endl;
                        ALU_opcode = 1;
                    }
                    else if(func3 == 1){
                        std::cout << "SLL Instruction" << std::endl;
                        ALU_opcode = 2;
                    }
                    else if(func3 == 2){
                        std::cout << "SLT Instruction" << std::endl;
                        ALU_opcode = 3;
                    }
                    else if(func3 == 3){
                        std::cout << "SLTU Instruction" << std::endl;
                        ALU_opcode = 4;
                    }
                    else if(func3 == 4){
                        std::cout << "XOR Instruction" << std::endl;
                        ALU_opcode = 5;
                    }
                    else if(func3 == 5){
                        std::cout << "SRL Instruction" << std::endl;
                        ALU_opcode = 6;
                    }
                    else if(func3 == 6){
                        std::cout << "OR Instruction" << std::endl;
                        ALU_opcode = 7;
                    }
                    else if(func3 == 7){
                        std::cout << "AND Instruction" << std::endl;
                        ALU_opcode = 8;
                    }
                    else{
                        std::cout << "Invalid func3" << std::endl;
                        invalid_instruction = 1;
                        ALU_opcode = 0;
                    }

                }

                else if(func7[0] == 1){

                    if(func3 == 0){
                        std::cout << "MUL Instruction" << std::endl;
                        ALU_opcode = 9;
                    }
                    else{
                        std::cout << "Invalid func3" << std::endl;
                        invalid_instruction = 1;
                        ALU_opcode = 0;
                    }
                }

                else if(func7[5] == 1){

                    if(func3 == 0){
                        std::cout << "SUB Instruction" << std::endl;
                        ALU_opcode = 10;
                    }

                    else if(func3 == 5){
                        std::cout << "SRA Instruction" << std::endl;
                        ALU_opcode = 11;
                    }

                    else{
                        std::cout << "Invalid func3" << std::endl;
                        invalid_instruction = 1;
                        ALU_opcode = 0;
                    }
                }

                else{
                    std::cout << "Invalid func7" << std::endl;
                    invalid_instruction = 1;
                    ALU_opcode = 0;
                }
                
                destination = rd;
                operation_1 = R[rs1];
                operation_2 = R[rs2];
                control = 1;

                std::cout << "\n" << "Operant 1: R[" << rs1 << "]" << std::endl;
                std::cout << "Operant 2: R[" << rs2 << "]" << std::endl;
                std::cout << "Destination: R[" << destination << "]" << "\n" << std::endl;
                PC = PC + 4;

                break;
            }
            case 19: 
            {
                ac_int<5,false> rd = instruction.slc<5>(7);
                ac_int<3,false> func3 = instruction.slc<3>(12);
                ac_int<5,false> rs1 = instruction.slc<5>(15);
        
                ac_int<20,false> sign_imm = -1;
                ac_int<32,true> sext_imm = i_immediate(instruction); 

                if(sext_imm[11] == 1){
                    sext_imm.set_slc(12,sign_imm);
                }

                if (func3 == 0){
                    std::cout << "ADDI Instruction" << std::endl;
                    ALU_opcode = 1;
                }

                else if(func3 == 1){
                    std::cout << "SLLI Instruction" << std::endl;
                    ALU_opcode = 2;
                }

                else if(func3 == 2){
                    std::cout << "SLTI Instruction" << std::endl;
                    ALU_opcode = 3;
                }

                else if(func3 == 3){
                    std::cout << "SLTIU Instruction" << std::endl;
                    ALU_opcode = 4;
                }        

                else if(func3 == 4){
                    std::cout << "XORI Instruction" << std::endl;
                    ALU_opcode = 5;
                }

                else if(func3 == 5){
                    
                    if(sext_imm[10] == 1){
                        std::cout << "SRAI instruction" << std::endl;
                        ALU_opcode = 11;
                    }

                    else if(sext_imm.slc<7>(5) == 0){
                        std::cout << "SRLI instruction" << std::endl;
                        ALU_opcode = 6;
                    }

                    else{
                        std::cout << "Invalid func3" << std::endl;
                        invalid_instruction = 1;
                        ALU_opcode = 0;
                    }
                }

                else if(func3 == 6){
                    std::cout << "ORI Instruction" << std::endl;
                    ALU_opcode = 7;
                }

                else if(func3 == 7){
                    std::cout << "ANDI Instruction" << std::endl;
                    ALU_opcode = 8;
                }

                else{
                    std::cout << "Invalid func3" << std::endl;
                    invalid_instruction = 1;
                    ALU_opcode = 0;
                }

                destination = rd;
                operation_1 = R[rs1];
                operation_2 = sext_imm;
                control = 1;

                std::cout << "\n" << "Operant 1: R[" << rs1 << "]" << std::endl;
                std::cout << "Operant 2: " << bitset::bitset<32>(sext_imm) << std::endl;
                std::cout << "Destination: R[" << destination << "]" << "\n" << std::endl;
                PC = PC + 4;

                break;
            }
            case 3: 
            {
            ac_int<5,false> rd = instruction.slc<5>(7);
            ac_int<3,false> func3 = instruction.slc<3>(12);
            ac_int<5,false> rs1 = instruction.slc<5>(15);

            ac_int<20,false> sign_imm = -1;
            ac_int<32,true> sext_imm = i_immediate(instruction); 

            if(sext_imm[11] == 1){
                sext_imm.set_slc(12,sign_imm);
            }

            if(func3 == 2){
                std::cout << "LW instruction" << std::endl;
                ALU_opcode = 0;
            }

            else{
                std::cout << "Invalid func3" << std::endl;
                invalid_instruction = 1;
                ALU_opcode = 0;
            }

            destination = rd;
            operation_1 = sext_imm;
            operation_2 = R[rs1];
            control = 2;

            std::cout << "\n" << "Operant 1: " << (sext_imm).slc<30>(2) << std::endl;
            std::cout << "Operant 2: R[" << rs1 << "]" << std::endl;
            std::cout << "Destination: R[" << destination << "]" << "\n" << std::endl;
            PC = PC + 4;

            break;
            }
            case 35: 
            {
            ac_int<3,false> func3 = instruction.slc<3>(12);
            ac_int<5,false> rs1 = instruction.slc<5>(15);
            ac_int<5,false> rs2 = instruction.slc<5>(20);

            ac_int<20,false> sign_imm = -1;
            ac_int<32,true> sext_imm = s_immediate(instruction); 
            
            if(sext_imm[11] == 1){
                sext_imm.set_slc(12,sign_imm);
            }

            if(func3 == 2){
                std::cout << "SW instruction" << std::endl;
                ALU_opcode = 0;
            }

            else{
                std::cout << "Invalid func3" << std::endl;
                invalid_instruction = 1;
                ALU_opcode = 0;
            }

            destination = R[rs2];
            operation_1 = R[rs1];
            operation_2 = sext_imm;
            control = 3;

            std::cout << "\n" << "Operant 1: R[" << rs1 << "]" << std::endl;
            std::cout << "Operant 2: " << (sext_imm).slc<30>(2) << std::endl;
            std::cout << "Destination: R[" << rs2 << "]" << "\n" << std::endl;
            PC = PC + 4;

            break;
            }
            case 55: 
            {
            ac_int<5,false> rd = instruction.slc<5>(7);
            ac_int<32,true> sext_imm = u_immediate(instruction);

            std::cout << "LUI instruction" << std::endl;
            
            ALU_opcode = 0;
            destination = rd;
            operation_1 = sext_imm;
            operation_2 = 0;
            control = 5;

            std::cout << "\n" << "Input Operant 1: " << bitset::bitset<32>(sext_imm) << std::endl;
            std::cout << "Operant 1: " << bitset::bitset<32>(sext_imm) << std::endl;
            std::cout << "Operant 2: " << operation_2 << std::endl;
            std::cout << "Destination: R[" << destination << "]" << "\n" << std::endl;
            std::cout << "Result: "<< bitset::bitset<32>(sext_imm) << "\n" << std::endl;

            PC = PC + 4;
            break;
            }
            case 23: 
            {
            ac_int<5,false> rd = instruction.slc<5>(7);
            ac_int<32,true> sext_imm = u_immediate(instruction);
            
            std::cout << "AUIPC instruction" << std::endl;

            ALU_opcode = 0;
            destination = rd;
            operation_1 = sext_imm;
            operation_2 = PC;
            control = 5;
            
            std::cout << "\n" << "Input Operant 1: " << bitset::bitset<32>(sext_imm) << std::endl;
            std::cout << "Operant 1: " << bitset::bitset<32>(sext_imm) << std::endl;
            std::cout << "Operant 2: " << operation_2 << std::endl;
            std::cout << "Destination: R[" << destination << "]" << "\n" << std::endl;
            std::cout << "Result: "<< bitset::bitset<32>(sext_imm + PC) << "\n" << std::endl;
            
            PC = PC + 4;  
            break;
            }
            case 111:
            { 
            ac_int<5,false> rd = instruction.slc<5>(7);

            ac_int<12,false> sign_imm = -1;
            ac_int<32,true> sext_imm = j_immediate(instruction);
            
            if(sext_imm[20] == 1){
                sext_imm.set_slc(11,sign_imm);
            }
            
            std::cout << "JAL instruction" << std::endl;
            ALU_opcode = 0;
            destination = rd;
            operation_1 = PC;
            operation_2 = sext_imm;
            control = 4;
            
            std::cout << "\n" << "Operant 1: " << bitset::bitset<32>(operation_1) << std::endl;
            std::cout << "Operant 2: " << bitset::bitset<32>(sext_imm) << std::endl;
            std::cout << "Destination: R[" << destination << "]" << "\n" << std::endl;

            R[rd] = PC + 4;
            PC = operation_1 + operation_2;
        
            break;
            }
            case 103: 
            {
            ac_int<5,false> rd = instruction.slc<5>(7);
            ac_int<3,false> func3 = instruction.slc<12>(20);
            ac_int<5,false> rs1 = instruction.slc<5>(15);

            ac_int<20,false> sign_imm = -1;
            ac_int<32,true> sext_imm = i_immediate(instruction);
            
            if(sext_imm[11] == 1){
                sext_imm.set_slc(12,sign_imm);
            }

            if((sext_imm == 0)&&(rd == 0)&&(func3 == 0)){
                std::cout << "JR instruction" << std::endl;
                ALU_opcode = 0;
            }
            
            else if(func3 == 0){
                std::cout << "JALR instruction" << std::endl;
                ALU_opcode = 0;
            }

            else{
                std::cout << "Invalid instruction" << std::endl;
                invalid_instruction = 1;
                ALU_opcode = 0;
            }

            destination = rd;
            operation_1 = R[rs1];
            operation_2 = sext_imm;
            control = 4;

            std::cout << "\n" << "Operant 1: R[" << operation_1 << "]" << std::endl;
            std::cout << "Operant 2: " << sext_imm << std::endl;
            std::cout << "Destination: R[" << destination << "]" << "\n" << std::endl;

            R[rd] = PC + 4;
            PC = operation_1 + operation_2;

            break;
            }
            case 99:
            {
            ac_int<3,false> func3 = instruction.slc<3>(12);
            ac_int<5,false> rs1 = instruction.slc<5>(15);
            ac_int<5,false> rs2 = instruction.slc<5>(20);

            ac_int<20,false> sign_imm = -1;
            ac_int<32,true> sext_imm = b_immediate(instruction);
            ac_int<1,false> result;

            destination = sext_imm;
            operation_1 = R[rs1];
            operation_2 = R[rs2];
            control = 0;
            
            if(sext_imm[11] == 1){
                sext_imm.set_slc(12,sign_imm);
            }

            if(func3 == 0){
                std::cout << "BEQ instruction" << std::endl;
                ALU_opcode = 12;
                result = (operation_1 == operation_2);
                std::cout << "R[" << rs1 << "] == R[" << rs2 << "]" << std::endl;
            }

            else if(func3 == 1){
                std::cout << "BNE instruction" << std::endl;
                ALU_opcode = 13;
                result = (operation_1 != operation_2);
                std::cout << "R[" << rs1 << "] != R[" << rs2 << "]" << std::endl;
            }

            else if(func3 == 4){
                std::cout << "BLT instruction" << std::endl;
                ALU_opcode = 3;
                result = (operation_1 < operation_2);
                std::cout << "R[" << rs1 << "] < R[" << rs2 << "]" << std::endl;
            }

            else if(func3 == 5){
                std::cout << "BGE instruction" << std::endl;
                ALU_opcode = 14;
                result = (operation_1 >= operation_2);
                std::cout << "R[" << rs1 << "] >= R[" << rs2 << "]" << std::endl;
            }

            else if(func3 == 6){
                std::cout << "BLTU instruction" << std::endl;
                ALU_opcode = 4;
                result = (operation_1 < operation_2);
                std::cout << "R[" << rs1 << "] <= R[" << rs2 << "]" << std::endl;
            }

            else if(func3 == 7){
                std::cout << "BGEU instruction" << std::endl;
                ALU_opcode = 15;
                result = (operation_1 >= operation_2);
                std::cout << "R[" << rs1 << "] >= R[" << rs2 << "]" << std::endl;
            }

            else{
                std::cout << "Invalid func3" << std::endl;
                invalid_instruction = 1;
                ALU_opcode = 0;
                result = 0;
            }

            if(result){
                PC = PC + destination;
            }
            else{
                PC = PC + 4;
            }

            std::cout << "Result: "<< result << "\n" << std::endl;
            break;
            }

            default:  
            invalid_instruction = 1;
            opcode = 0;
            ALU_opcode = 0;
            operation_1 = 0;
            operation_2 = 0;
            destination = 0;
        }

        check_decode(invalid_instruction, opcode, ALU_opcode, control);
        execute(ALU_opcode, operation_1, operation_2, destination, control);
        return 0;
    }
};

CCS_MAIN(int argc, char argv){

ac_int<32,false> i = 50;
ac_int<32,false> instruction_mem[256];
ac::init_array<AC_VAL_0>(instruction_mem, 256);
ac_int<32,true> data_mem[256];
ac::init_array<AC_VAL_0>(data_mem, 256);

data_mem[100] = 100;
data_mem[104] = 101; 

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
instruction_mem[73] = 0b00000000011000100010010000100011; // SW    mem[R[4] + 2], R6

/*
instruction_mem[74] = 0b00000000010100100000110001100011; // BEQ   R[4],  R[5],   24
instruction_mem[75] = 0b11111110010100100001011011100011; // BNE   R[4],  R[5],   -20
instruction_mem[76] = 0b00000000010100100100101001100011; // BLT   R[4],  R[5],   20
instruction_mem[77] = 0b00000000000000010101010001100011; // BGE   R[2],  R[0],   000000001000
instruction_mem[78] = 0b00000000010100100110001001100011; // BLTU  R[4],  R[5],   8
instruction_mem[79] = 0b11111110010100100111100011100011; // BGEU  R4,    R5,     -16
instruction_mem[80] = 0b00000000000000000000000000000000; // NOPs
instruction_mem[81] = 0b11111111010111111111010111101111; // JAL   R[11],         -13
instruction_mem[82] = 0b00000000000000110000000001100111; // JR    R[6]
instruction_mem[83] = 0b00000000000000001000100101100111; // JALR  R[18], R1,     0 
*/
Processor processor;

while (instruction_mem[i] != 0) {

processor.run(instruction_mem, data_mem);
i++;

}

cout << "Execution finished." << "\n" << endl;

for (i = 0; i < 256; i++) {

    if(data_mem[i] != 0){

    cout << "mem[" << data_mem[i] << "]\n" << endl;

    }

    i++;
}

CCS_RETURN(0);

}
