#include "ALU.h"
#include "Processor.h"
#include "ac_int.h"

#include <iostream>

using namespace std;

//Read 32-bit instruction:

/*
ac_int<32,true> Processor::read_instruction(ac_int<32,true> instruction_memory[256]) {
    return ;
}
*/

//Decode 32-bit instruction:

ac_int<32,true> Processor::decode_instruction(ac_int<32,true> instruction) {

    //The type of instruction according to opcode:
    ac_int<4,false> ALU_opcode;
    ac_int<1,false> invalid_instruction;
    ac_int<7,false> opcode = instruction.slc<7>(0);

    switch(opcode) {

        case 51: //(0b0110011)
        //Register-Register Arithmetic Instructions 
        //R-Type:
        ac_int<5,false> rd = instruction.slc<5>(7);
        ac_int<3,false> func3 = instruction.slc<3>(12);
        ac_int<5,false> rs1 = instruction.slc<5>(15);
        ac_int<5,false> rs2 = instruction.slc<5>(20);
        ac_int<7,false> func7 = instruction.slc<7>(25);


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

        case 19: //(0b0010011)
        //Register-Immediate Arithmetic Instructions
        //I-Type-I-Immediate:
        ac_int<5,false> rd = instruction.slc<5>(7);
        ac_int<3,false> func3 = instruction.slc<3>(12);
        ac_int<5,false> rs1 = instruction.slc<5>(15);
        ac_int<12,true> imm = instruction.slc<12>(20);

        if (func3 == 0){
            std::cout << "ADDI Instruction" << endl;
            ALU_opcode = 12;
        }

        else if(func3 == 1){
            std::cout << "SLLI Instruction" << endl;
            ALU_opcode = 13;
            //more 31        25 24     20
        }

        else if(func3 == 2){
            std::cout << "SLTI Instruction" << endl;
            ALU_opcode = 14;
        }

        else if(func3 == 3){
            std::cout << "SLTIU Instruction" << endl;
            ALU_opcode = 15;
        }        

        else if(func3 == 4){
            std::cout << "XORI Instruction" << endl;
            ALU_opcode = 16;
        }

        else if(func3 == 5){
            
            if(imm[10] == 1){
                std::cout << "SRAI instruction" << std::endl;
                ALU_opcode = 17;
                //more 31        25 24     20
            }

            else if(imm.slc<7>(5) == 0){
                std::cout << "SRLI instruction" << std::endl;
                ALU_opcode = 18;
                //more 31        25 24     20
            }

            else{
                std::cout << "Invalid func3" << endl;
                invalid_instruction = 1;
                ALU_opcode = 0;
            }
        }

        else if(func3 == 6){
            std::cout << "ORI Instruction" << endl;
            ALU_opcode = 19;
        }

        else if(func3 == 7){
            std::cout << "ANDI Instruction" << endl;
            ALU_opcode = 20;
        }

        else{
            std::cout << "Invalid func3" << endl;
            invalid_instruction = 1;
            ALU_opcode = 0;
        }

        break;

        case 3: //(0b0000011)
        //Memory Instructions
        //I-Type-I-Immediate:
        ac_int<5,false> rd = instruction.slc<5>(7);
        ac_int<3,false> func3 = instruction.slc<3>(12);
        ac_int<5,false> rs1 = instruction.slc<5>(15);
        ac_int<12,true> imm = instruction.slc<12>(20);

        if(func3 == 2){
            std::cout << "LW instruction" << std::endl;
            ALU_opcode = 21;
        }

        else{
            std::cout << "Invalid func3" << endl;
            invalid_instruction = 1;
            ALU_opcode = 0;
        }

        break;

        case 35: //(0b0100011)
        //Memory Instructions
        //S-Type-S-immediate:
        ac_int<5,false> imm_part_1 = instruction.slc<5>(7);
        ac_int<3,false> func3 = instruction.slc<3>(12);
        ac_int<5,false> rs1 = instruction.slc<5>(15);
        ac_int<5,false> rs2 = instruction.slc<5>(20);
        ac_int<7,false> imm_part_2 = instruction.slc<7>(25);

        if(func3 == 2){
            std::cout << "SW instruction" << std::endl;
            ALU_opcode = 22;
        }
        else{
            std::cout << "Invalid func3" << endl;
            invalid_instruction = 1;
            ALU_opcode = 0;
        }

        break;

        case 55: //(0b0110111)
        //Register-Immediate Arithmetic Instructions
        //I-Type-U-immediate: (using U-type)
        ac_int<5,false> rd = instruction.slc<5>(7);
        ac_int<20,false> imm = instruction.slc<20>(12);
        
        std::cout << "LUI instruction" << std::endl;
        invalid_instruction = 0;
        ALU_opcode = 23;

        break;

        case 23: //(0b0010111)
        //Register-Immediate Arithmetic Instructions
        //I-Type-U-immediate: (using U-type)
        ac_int<5,false> rd = instruction.slc<5>(7);
        ac_int<20,false> imm = instruction.slc<20>(12);
        
        std::cout << "AUIPC instruction" << std::endl;
        invalid_instruction = 0;
        ALU_opcode = 24;
        
        break;

        case 111: //(0b1101111)
        //Unconditional Jump Instructions
        //U-type, J-immediate: (using U-type)
        ac_int<5,false> rd = instruction.slc<5>(7);
        ac_int<20,false> imm = instruction.slc<20>(12);
        
        std::cout << "JAL instruction" << std::endl;
        invalid_instruction = 0;
        ALU_opcode = 25;
        
        break;

        case 103: //(0b1101111)
        //Unconditional Jump Instructions
        //I-Type, I-immediate: (using I-type)
        ac_int<5,false> rd = instruction.slc<5>(7);
        ac_int<3,false> func3 = instruction.slc<3>(12);
        ac_int<5,false> rs1 = instruction.slc<5>(15);
        ac_int<12,true> imm = instruction.slc<12>(20);
        
        if((imm == 0)&&(rd == 0)&&(func3 == 0)){
            std::cout << "JR instruction" << std::endl;
            ALU_opcode = 26;
        }
        
        else if(func3 == 0){
            std::cout << "JALR instruction" << std::endl;
            ALU_opcode = 27;
        }
        else{
            std::cout << "Invalid instruction" << endl;
            invalid_instruction = 1;
            ALU_opcode = 0;
        }

        break;

        default:
            
            if(ALU_opcode == 0){
                cout << "Break due to invalid parameters..." << endl;
            }

            else{
                invalid_instruction = 0;
                cout << "Valid parameters, continue..." << endl;
            }
        }
}


        //I-immediate:
        ac_int<32,true> imm = 0;
        ac_int<1,false> imm_part_1 = instruction[20];
        ac_int<4,false> imm_part_2 = instruction.slc<4>(21);
        ac_int<6,false> imm_part_3 = instruction.slc<6>(25);
        ac_int<1,false> imm_part_4 = instruction[31];
        imm[0] = imm_part_1;
        imm.set_slc(1,imm_part_2);
        imm.set_slc(5,imm_part_3);
        imm.set_slc(11,imm_part_4);

        //S-immediate:
        ac_int<32,true> imm = 0;
        ac_int<1,false> imm_part_1 = instruction[7];
        ac_int<4,false> imm_part_2 = instruction.slc<4>(8);
        ac_int<6,false> imm_part_3 = instruction.slc<6>(25);
        ac_int<1,false> imm_part_4 = instruction[31];
        imm[0] = imm_part_1;
        imm.set_slc(1,imm_part_2);
        imm.set_slc(5,imm_part_3);
        imm.set_slc(11,imm_part_4);

        //B-immediate:
        ac_int<32,true> imm = 0;
        ac_int<4,false> imm_part_1 = instruction.slc<4>(8);
        ac_int<6,false> imm_part_2 = instruction.slc<6>(25);
        ac_int<1,false> imm_part_3 = instruction[7];
        ac_int<1,false> imm_part_4 = instruction[31];
        imm.set_slc(1,imm_part_1);
        imm.set_slc(5,imm_part_2);
        imm[11] = imm_part_3;
        imm.set_slc(12,imm_part_4);

        //U-immediate:
        ac_int<32,true> imm = 0;
        ac_int<8,false> imm_part_1 = instruction.slc<8>(12);
        ac_int<11,false> imm_part_2 = instruction.slc<11>(20);
        ac_int<1,false> imm_part_3 = instruction[31];
        imm.set_slc(12,imm_part_1);
        imm.set_slc(20,imm_part_2);
        imm[31] = imm_part_3;

        //J-immediate:
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