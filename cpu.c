#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

char *reg_names[REG_NUM] = { // * because we have whole words not chars
    "zero",  // x0
    "ra",    // x1
    "sp",    // x2
    "gp",    // x3
    "tp",    // x4
    "t0",    // x5
    "t1",    // x6
    "t2",    // x7
    "s0",    // x8  (also called fp)
    "s1",    // x9
    "a0",    // x10
    "a1",    // x11
    "a2",    // x12
    "a3",    // x13
    "a4",    // x14
    "a5",    // x15
    "a6",    // x16
    "a7",    // x17
    "s2",    // x18
    "s3",    // x19
    "s4",    // x20
    "s5",    // x21
    "s6",    // x22
    "s7",    // x23
    "s8",    // x24
    "s9",    // x25
    "s10",   // x26
    "s11",   // x27
    "t3",    // x28
    "t4",    // x29
    "t5",    // x30
    "t6"     // x31
};

char* r_type_instr[R_TYPE_LEN] = {
    "add", 
    "sub",
    "and",
    "or",
    "xor"
};

char* i_type_instr[I_TYPE_LEN] = {
    "addi", 
    "andi",
    "ori",
    "lw"
};

char* s_type_instr[S_TYPE_LEN] = {
    "sw"
};

char* b_type_instr[B_TYPE_LEN] = {
    "beq", 
};

char* pseudo_instr[P_TYPE_LEN] = {
    "li", 
};


void cpu_init(struct CPU *cpu) {
    printf("Starting CPU...\n");
    cpu->pc = PC;
}