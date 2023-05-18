#include "ac_int.h"
#include "ALU.h"

//Control/Status Register Instructions: (Empty)

//Register-Register Arithmetic Instructions: (Arithemtic, Shift, Comparators)

//Arithmetic:

ac_int<32,true> ALU::add(ac_int<32,true> op1, ac_int<32,true> op2) {
    return op1 + op2;
}

ac_int<32,true> ALU::sub(ac_int<32,true> op1, ac_int<32,true> op2) {
    return op1 - op2;
}

ac_int<32,true> ALU::mul(ac_int<32,true> op1, ac_int<32,true> op2) {
    return op1 * op2;
}

//Logical:

ac_int<32,true> ALU::logical_and(ac_int<32,true> op1, ac_int<32,true> op2) {
    return op1 & op2;
}

ac_int<32,true> ALU::logical_or(ac_int<32,true> op1, ac_int<32,true> op2) {
    return op1 | op2;
}

ac_int<32,true> ALU::logical_xor(ac_int<32,true> op1, ac_int<32,true> op2) {
    return op1 ^ op2;
}

//Shift:

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

//Comparators:

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

//Operations

ac_int<32,true> ALU::operations(ac_int<4,false> ALU_opcode, ac_int<32,true> operation_1, ac_int<32,true> operation_2, ){

    switch(ALU_opcode){

        case 1:
            return ALU::add(operation_1, operation_2);
        case 2:
            return ALU::shift_left_logical(operation_1, operation_2);
        case 3:
            return ALU::signed_less_than(operation_1, operation_2);
        case 4:
            return ALU::unsigned_less_than(operation_1, operation_2);
        case 5:
            return ALU::logical_xor(operation_1, operation_2);
        case 6:
            return ALU::shift_right_logical(operation_1, operation_2);
        case 7:
            return ALU::logical_or(operation_1, operation_2);
        case 8:
            return ALU::logical_and(operation_1, operation_2);
        case 9:
            return ALU::mul(operation_1, operation_2);
        case 10:
            return ALU::sub(operation_1, operation_2);
        case 11:
            return ALU::shift_right_arithmetic(operation_1, operation_2);
        case 12:
            return ALU::comparator_equal(operation_1, operation_2);
        case 13:
            return ALU::not_equal(operation_1, operation_2);
        case 14:
            return ALU::signed_greater_than(operation_1, operation_2);
        case 15:
            return ALU::unsigned_greater_than(operation_1, operation_2);
        default:
            return 0;
    }
}