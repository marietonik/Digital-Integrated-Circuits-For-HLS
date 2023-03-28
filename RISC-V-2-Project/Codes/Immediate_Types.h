#ifndef Immediate_Types_H
#define Immediate_Types_H

#include "ac_int.h"

class Immediate_Types
{
    public:

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
};
#endif