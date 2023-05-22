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

    ac_int<32,true> mul(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 * op2;
    }

    ac_int<32,true> logical_and(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 & op2;
    }

    ac_int<32,true> logical_or(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 | op2;
    }   

    ac_int<32,true> logical_xor(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 ^ op2;
    }

    ac_int<32,true> shift_right_arithmetic(ac_int<32,true> op1, ac_int<32,true> op2) {
        ac_int<5,false> op2_five_lsb = op2.slc<5>(0);
        return op1 >> op2_five_lsb;
    }

    ac_int<32, true> shift_right_logical(ac_int<32,true> op1, ac_int<32,true> op2) {
        ac_int<5,false> op2_five_lsb = op2.slc<5>(0);
        ac_int<32,false> unsigned_op1 = op1;
        return unsigned_op1 >> op2_five_lsb;
    }

    ac_int<32,true> shift_left_logical(ac_int<32,true> op1, ac_int<32,true> op2) {
        ac_int<5,false> op2_five_lsb = op2.slc<5>(0);
        ac_int<32,false> unsigned_op1 = op1;
        return unsigned_op1 << op2_five_lsb;
    }

    ac_int<32,true> comparator_equal(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 == op2;
    }   

    ac_int<32,true> not_equal(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 != op2;
    }

    ac_int<32, true> signed_less_than(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 < op2;
    }

    ac_int<32, true> unsigned_less_than(ac_int<32,true> op1, ac_int<32,true> op2) {
        ac_int<32,false> unsigned_op1 = op1;
        ac_int<32,false> unsigned_op2 = op2;
        return unsigned_op1 < unsigned_op2;
    }

    ac_int<32, true> signed_greater_than(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 >= op2;
    }

    ac_int<32, true> unsigned_greater_than(ac_int<32,true> op1, ac_int<32,true> op2) {
        ac_int<32,false> unsigned_op1 = op1;
        ac_int<32,false> unsigned_op2 = op2;
        return unsigned_op1 >= unsigned_op2;
    }

    ac_int<32,true> operations(ac_int<4,false> ALU_opcode, ac_int<32,true> operation_1, ac_int<32,true> operation_2){

        switch(ALU_opcode){

            case 1:
                return add(operation_1, operation_2);
            case 2:
                return shift_left_logical(operation_1, operation_2);
            case 3:
                return signed_less_than(operation_1, operation_2);
            case 4:
                return unsigned_less_than(operation_1, operation_2);
            case 5:
                return logical_xor(operation_1, operation_2);
            case 6:
                return shift_right_logical(operation_1, operation_2);
            case 7:
                return logical_or(operation_1, operation_2);
            case 8:
                return logical_and(operation_1, operation_2);
            case 9:
                return mul(operation_1, operation_2);
            case 10:
                return sub(operation_1, operation_2);
            case 11:
                return shift_right_arithmetic(operation_1, operation_2);
            case 12:
                return comparator_equal(operation_1, operation_2);
            case 13:
                return not_equal(operation_1, operation_2);
            case 14:
                return signed_greater_than(operation_1, operation_2);
            case 15:
                return unsigned_greater_than(operation_1, operation_2);
            default:
                return 0;
        }
    }
};
#endif
