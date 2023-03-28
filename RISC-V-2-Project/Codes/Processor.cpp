#include "ALU.h"
#include "Processor.h"
#include "Immediate_Types.h"
#include "ac_int.h"
#include <iostream>
using namespace std;

ac_int<32,false> PC;
ac_int<32,false> R[32];

/*
ac_int<32,false> Processor::read_instruction(ac_int<32,false> instruction_memory[256]){
    return instruction_memory[PC.slc<30>(2)];
}
*/

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

            destination = rd;
            operation_1 = R[rs1];
            operation_2 = R[rs2];
            control = 1;

            if(func7 == 0){
                
                if(func3 == 0){
                    std::cout << "ADD Instruction" << endl;
                    ALU_opcode = 1;
                }
                else if(func3 == 1){
                    std::cout << "SLL Instruction" << endl;
                    ALU_opcode = 2;
                }
                else if(func3 == 2){
                    std::cout << "SLT Instruction" << endl;
                    ALU_opcode = 3;
                }
                else if(func3 == 3){
                    std::cout << "SLTU Instruction" << endl;
                    ALU_opcode = 4;
                }
                else if(func3 == 4){
                    std::cout << "XOR Instruction" << endl;
                    ALU_opcode = 5;
                }
                else if(func3 == 5){
                    std::cout << "SRL Instruction" << endl;
                    ALU_opcode = 6;
                }
                else if(func3 == 6){
                    std::cout << "OR Instruction" << endl;
                    ALU_opcode = 7;
                }
                else if(func3 == 7){
                    std::cout << "AND Instruction" << endl;
                    ALU_opcode = 8;
                }
                else{
                    std::cout << "Invalid func3" << endl;
                    invalid_instruction = 1;
                    ALU_opcode = 0;
                }

            }

            else if(func7[0] == 1){

                if(func3 == 0){
                    std::cout << "MUL Instruction" << endl;
                    ALU_opcode = 9;
                }
                else{
                    std::cout << "Invalid func3" << endl;
                    invalid_instruction = 1;
                    ALU_opcode = 0;
                }
            }

            else if(func7[5] == 1){

                if(func3 == 0){
                    std::cout << "SUB Instruction" << endl;
                    ALU_opcode = 10;
                }

                else if(func3 == 5){
                    std::cout << "SRA Instruction" << endl;
                    ALU_opcode = 11;
                }

                else{
                    std::cout << "Invalid func3" << endl;
                    invalid_instruction = 1;
                    ALU_opcode = 0;
                }
            }

            else{
                std::cout << "Invalid func7" << endl;
                invalid_instruction = 1;
                ALU_opcode = 0;
            }
            
            break;
        }
        case 19: 
        {
            ac_int<5,false> rd = instruction.slc<5>(7);
            ac_int<3,false> func3 = instruction.slc<3>(12);
            ac_int<5,false> rs1 = instruction.slc<5>(15);
    
            ac_int<20,false> sign_imm = -1;
            ac_int<32,true> sext_imm = 0; //Immediate_Types::i_immediate(instruction); 

            if(sext_imm[11] == 1){
                sext_imm.set_slc(12,sign_imm);
            }

            destination = rd;
            operation_1 = R[rs1];
            operation_2 = sext_imm;
            control = 1;

            if (func3 == 0){
                std::cout << "ADDI Instruction" << endl;
                ALU_opcode = 1;
            }

            else if(func3 == 1){
                std::cout << "SLLI Instruction" << endl;
                ALU_opcode = 2;
            }

            else if(func3 == 2){
                std::cout << "SLTI Instruction" << endl;
                ALU_opcode = 3;
            }

            else if(func3 == 3){
                std::cout << "SLTIU Instruction" << endl;
                ALU_opcode = 4;
            }        

            else if(func3 == 4){
                std::cout << "XORI Instruction" << endl;
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
                    std::cout << "Invalid func3" << endl;
                    invalid_instruction = 1;
                    ALU_opcode = 0;
                }
            }

            else if(func3 == 6){
                std::cout << "ORI Instruction" << endl;
                ALU_opcode = 7;
            }

            else if(func3 == 7){
                std::cout << "ANDI Instruction" << endl;
                ALU_opcode = 8;
            }

            else{
                std::cout << "Invalid func3" << endl;
                invalid_instruction = 1;
                ALU_opcode = 0;
            }

            break;
        }
        case 3: 
        {
        ac_int<5,false> rd = instruction.slc<5>(7);
        ac_int<3,false> func3 = instruction.slc<3>(12);
        ac_int<5,false> rs1 = instruction.slc<5>(15);

        ac_int<20,false> sign_imm = -1;
        ac_int<32,true> sext_imm = 0; //Immediate_Types::i_immediate(instruction); 

        destination = rd;
        operation_1 = sext_imm;
        operation_2 = R[rs1];
        control = 2;

        if(sext_imm[11] == 1){
            sext_imm.set_slc(12,sign_imm);
        }

        if(func3 == 2){
            std::cout << "LW instruction" << std::endl;
            ALU_opcode = 0;
        }

        else{
            std::cout << "Invalid func3" << endl;
            invalid_instruction = 1;
            ALU_opcode = 0;
        }

        break;
        }
        case 35: 
        {
        ac_int<3,false> func3 = instruction.slc<3>(12);
        ac_int<5,false> rs1 = instruction.slc<5>(15);
        ac_int<5,false> rs2 = instruction.slc<5>(20);

        ac_int<20,false> sign_imm = -1;
        ac_int<32,true> sext_imm = 0; //Immediate_Types::s_immediate(instruction); 
        
        if(sext_imm[11] == 1){
            sext_imm.set_slc(12,sign_imm);
        }

        destination = R[rs2];
        operation_1 = R[rs1];
        operation_2 = sext_imm;
        control = 3;

        if(func3 == 2){
            std::cout << "SW instruction" << std::endl;
            ALU_opcode = 0;
        }

        else{
            std::cout << "Invalid func3" << endl;
            invalid_instruction = 1;
            ALU_opcode = 0;
        }

        break;
        }
        case 55: 
        {
        ac_int<5,false> rd = instruction.slc<5>(7);
        ac_int<32,true> sext_imm = 0; //Immediate_Types::u_immediate(instruction);

        destination = rd;
        operation_1 = sext_imm;
        operation_2 = 0;
        control = 1;

        std::cout << "LUI instruction" << std::endl;
        ALU_opcode = 0;

        break;
        }
        case 23: 
        {
        ac_int<5,false> rd = instruction.slc<5>(7);
        ac_int<32,true> sext_imm = 0; //Immediate_Types::u_immediate(instruction);

        destination = rd;
        operation_1 = sext_imm;
        operation_2 = PC;
        control = 1;
        
        std::cout << "AUIPC instruction" << std::endl;
        ALU_opcode = 0;
        
        break;
        }
        case 111:
        { 
        ac_int<5,false> rd = instruction.slc<5>(7);

        ac_int<12,false> sign_imm = -1;
        ac_int<32,true> sext_imm = 0; //Immediate_Types::j_immediate(instruction);
        
        if(sext_imm[20] == 1){
            sext_imm.set_slc(11,sign_imm);
        }

        destination = rd;
        operation_1 = PC;
        operation_2 = sext_imm;
        control = 4;
        
        std::cout << "JAL instruction" << std::endl;
        ALU_opcode = 0;
        
        break;
        }
        case 103: 
        {
        ac_int<5,false> rd = instruction.slc<5>(7);
        ac_int<3,false> func3 = instruction.slc<3>(12);
        ac_int<5,false> rs1 = instruction.slc<5>(15);

        ac_int<20,false> sign_imm = -1;
        ac_int<32,true> sext_imm = 0; //Immediate_Types::i_immediate(instruction);

        destination = rd;
        operation_1 = R[rs1];
        operation_2 = sext_imm;
        control = 4;
        
        if(sext_imm[11] == 1){
            sext_imm.set_slc(20,sign_imm);
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
            std::cout << "Invalid instruction" << endl;
            invalid_instruction = 1;
            ALU_opcode = 0;
        }

        break;
        }
        case 99:
        {
        ac_int<3,false> func3 = instruction.slc<3>(12);
        ac_int<5,false> rs1 = instruction.slc<5>(15);
        ac_int<5,false> rs2 = instruction.slc<5>(20);

        ac_int<20,false> sign_imm = -1;
        ac_int<32,true> sext_imm = 0; //Immediate_Types::b_immediate(instruction);

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
        }

        else if(func3 == 1){
            std::cout << "BNE instruction" << std::endl;
            ALU_opcode = 13;
        }

        else if(func3 == 4){
            std::cout << "BLT instruction" << std::endl;
            ALU_opcode = 3;
        }

        else if(func3 == 5){
            std::cout << "BGE instruction" << std::endl;
            ALU_opcode = 14;
        }

        else if(func3 == 6){
            std::cout << "BLTU instruction" << std::endl;
            ALU_opcode = 4;
        }

        else if(func3 == 7){
            std::cout << "BGEU instruction" << std::endl;
            ALU_opcode = 15;
        }

        else{
            std::cout << "Invalid func3" << endl;
            invalid_instruction = 1;
            ALU_opcode = 0;
        }

        break;
        }

        default:
        
        if(invalid_instruction == 1){
            cout << "Break due to invalid parameters..." << endl;
        }

        else{
            cout << "Valid parameters, continue..." << endl;
            cout << "/n" << ALU_opcode << endl;
            cout << "/n" << destination << endl;
            cout << "/n" << operation_1 << endl;
            cout << "/n" << operation_2 << endl;
            cout << "/n" << control << endl;
        }
    }

    return 0;
}