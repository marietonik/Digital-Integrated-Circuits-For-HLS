#ifndef ALU_H
#define ALU_H

#include "ac_int.h"

class ALU
{
    public:

    ac_int<32,true> add(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 + op2;
    }

    ac_int<32,true> sub(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 - op2;
    }

    ac_int<32,true> ALU::mul(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 * op2;
    }

    ac_int<32,true> ALU::logical_and(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 & op2;
    }

    ac_int<32,true> ALU::logical_or(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 | op2;
    }   

    ac_int<32,true> ALU::logical_xor(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 ^ op2;
    }

    ac_int<32,true> ALU::comparator_equal(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 == op2;
    }

    ac_int<32,true> ALU::shift_right_arithmetic(ac_int<32,true> op1, ac_int<32,true> op2) {
        ac_int<5,false> op2_five_lsb = op2.slc<5>(0);
        return op1 >> op2_five_lsb;
    }

    ac_int<32, true> ALU::shift_right_logical(ac_int<32,true> op1, ac_int<32,true> op2) {
        ac_int<5,false> op2_five_lsb = op2.slc<5>(0);
        ac_int<32,false> unsigned_op1 = op1;
        return unsigned_op1 >> op2_five_lsb;
    }

    ac_int<32,true> ALU::shift_left_logical(ac_int<32,true> op1, ac_int<32,true> op2) {
        ac_int<5,false> op2_five_lsb = op2.slc<5>(0);
        ac_int<32,false> unsigned_op1 = op1;
        return unsigned_op1 << op2_five_lsb;
    }

    ac_int<32,true> ALU::comparator_equal(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 == op2;
    }   

    ac_int<32,true> ALU::not_equal(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 != op2;
    }

    ac_int<32, true> ALU::signed_less_than(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 < op2;
    }

    ac_int<32, true> ALU::unsigned_less_than(ac_int<32,true> op1, ac_int<32,true> op2) {
        ac_int<32,false> unsigned_op1 = op1;
        ac_int<32,false> unsigned_op2 = op2;
        return unsigned_op1 < unsigned_op2;
    }

    ac_int<32, true> ALU::signed_greater_than(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 >= op2;
    }

    ac_int<32, true> ALU::unsigned_greater_than(ac_int<32,true> op1, ac_int<32,true> op2) {
        ac_int<32,false> unsigned_op1 = op1;
        ac_int<32,false> unsigned_op2 = op2;
        return unsigned_op1 >= unsigned_op2;
    }
};
#endif
