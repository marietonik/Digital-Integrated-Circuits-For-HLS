#ifndef Processor_H
#define Processor_H

#include "ALU.h"
#include "Processor.h"
#include "ac_int.h"
#include <iostream>
#include <bitset>
using namespace std;

class Processor
{
    public:
    ac_int<32,false> PC;
    ac_int<32,true> R[32];
    ac_int<1,false> invalid_instruction;
    ALU alu;

    Processor()
    {
        PC = 200;
        R[0] = 0;
        invalid_instruction = 0;
        cout << "Initial PC is: " << PC << "\n" << endl;
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
    
    ac_int<32,false> read_instruction_memory(ac_int<32,false> instruction_mem[256]){

        return instruction_mem[PC.slc<30>(2)];
    }

    ac_int<1,false> run(ac_int<32,false> instruction_mem[256], ac_int<32,true> data_mem[256]){

        decode_instruction(read_instruction_memory(instruction_mem));
        return 0;
    }

    ac_int<32,true> memory_read(ac_int<32,true> memory[256], ac_int<32,false> address){

        return memory[address.slc<30>(2)];
    }
    
    void memory_write(ac_int<32,true> memory[256], ac_int<32,false> address, ac_int<32,true> value){
        
        memory[address.slc<30>(2)] = value;
    }

    void register_write(ac_int<32,false> register_address, ac_int<32,true> value){
        
        R[register_address] = value;
    }

    ac_int<32,true> execute(ac_int<4,false> ALU_opcode, ac_int<32,false> operation_1, ac_int<32,false> operation_2, ac_int<32,false> destination, ac_int<3,false> control){
        
        ac_int<32,true> result;

        if(control == 1){
            
            result = alu.operations(ALU_opcode, operation_1, operation_2);
            cout << "Result: "<< result << "\n" << endl;
            register_write(destination, result);
        }
        else if(control == 2){
            //LW
            //register_write();
        }
        else if(control == 3){
            //SW
            //memory_write();
        }
        else{
            cout << "Continue..." << "\n" << endl;
        }
        return 0;
    }

    ac_int<1,false> check_decode(ac_int<1,false> invalid_instruction, ac_int<7,false> opcode, ac_int<4,false> ALU_opcode, ac_int<3,false> control){

        if(invalid_instruction == 0){
            cout << "Opcode: " << opcode << endl;
            cout << "ALU opcode: " << ALU_opcode << endl;
            cout << "Control: " << control << endl;
        }

        else{
            cout << "Break due to invalid parameters..." << endl;
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
                
                destination = rd;
                operation_1 = R[rs1];
                operation_2 = R[rs2];
                control = 1;

                cout << "\n" << "Operant 1: R[" << rs1 << "]" << endl;
                cout << "Operant 2: R[" << rs2 << "]" << endl;
                cout << "Destination: R[" << destination << "]" << "\n" << endl;
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

                destination = rd;
                operation_1 = R[rs1];
                operation_2 = sext_imm;
                control = 1;

                cout << "\n" << "Operant 1: R[" << rs1 << "]" << endl;
                cout << "Operant 2: " << sext_imm << endl;
                cout << "Destination: R[" << destination << "]" << "\n" << endl;
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
                std::cout << "Invalid func3" << endl;
                invalid_instruction = 1;
                ALU_opcode = 0;
            }

            destination = rd;
            operation_1 = sext_imm;
            operation_2 = R[rs1];
            control = 2;

            cout << "\n" << "Operant 1: " << sext_imm << endl;
            cout << "Operant 2: R[" << rs1 << "]" << endl;
            cout << "Destination: R[" << destination << "]" << "\n" << endl;
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
                std::cout << "Invalid func3" << endl;
                invalid_instruction = 1;
                ALU_opcode = 0;
            }

            destination = R[rs2];
            operation_1 = R[rs1];
            operation_2 = sext_imm;
            control = 3;

            cout << "\n" << "Operant 1: R[" << rs1 << "]" << endl;
            cout << "Operant 2: " << sext_imm << endl;
            cout << "Destination: R[" << rs2 << "]" << "\n" << endl;
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

            cout << "\n" << "Operant 1: " << sext_imm << endl;
            cout << "Operant 2: " << operation_2 << endl;
            cout << "Destination: R[" << destination << "]" << "\n" << endl;
            cout << "Result: "<< bitset<32>(sext_imm) << "\n";

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
            
            cout << "\n" << "Operant 1: " << sext_imm << endl;
            cout << "Operant 2: " << operation_2 << endl;
            cout << "Destination: R[" << destination << "]" << "\n" << endl;
            cout << "Result: "<< bitset<32>(sext_imm) << "\n";
            
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
            
            cout << "\n" << "Operant 1: " << operation_1 << endl;
            cout << "Operant 2: " << sext_imm << endl;
            cout << "Destination: R[" << destination << "]" << "\n" << endl;

            R[rd] = PC + 4;
            PC = operation_1 + operation_2;
        
            break;
            }
            case 103: 
            {
            ac_int<5,false> rd = instruction.slc<5>(7);
            ac_int<3,false> func3 = instruction.slc<3>(12);
            ac_int<5,false> rs1 = instruction.slc<5>(15);

            ac_int<20,false> sign_imm = -1;
            ac_int<32,true> sext_imm = i_immediate(instruction);
            
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

            destination = rd;
            operation_1 = R[rs1];
            operation_2 = sext_imm;
            control = 4;

            cout << "\n" << "Operant 1: R[" << operation_1 << "]" << endl;
            cout << "Operant 2: " << sext_imm << endl;
            cout << "Destination: R[" << destination << "]" << "\n" << endl;

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
            
            if(sext_imm[11] == 1){
                sext_imm.set_slc(12,sign_imm);
            }

            if(func3 == 0){
                std::cout << "BEQ instruction" << std::endl;
                ALU_opcode = 12;
                result = (operation_1 == operation_2);
            }

            else if(func3 == 1){
                std::cout << "BNE instruction" << std::endl;
                ALU_opcode = 13;
                result = (operation_1 != operation_2);
            }

            else if(func3 == 4){
                std::cout << "BLT instruction" << std::endl;
                ALU_opcode = 3;
                result = (operation_1 < operation_2);
            }

            else if(func3 == 5){
                std::cout << "BGE instruction" << std::endl;
                ALU_opcode = 14;
                result = (operation_1 >= operation_2);
            }

            else if(func3 == 6){
                std::cout << "BLTU instruction" << std::endl;
                ALU_opcode = 4;
                result = (operation_1 < operation_2);
            }

            else if(func3 == 7){
                std::cout << "BGEU instruction" << std::endl;
                ALU_opcode = 15;
                result = (operation_1 >= operation_2);
            }

            else{
                std::cout << "Invalid func3" << endl;
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

            destination = sext_imm;
            operation_1 = R[rs1];
            operation_2 = R[rs2];
            control = 0;

            cout << "Result: "<< result << "\n";
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
#endif
