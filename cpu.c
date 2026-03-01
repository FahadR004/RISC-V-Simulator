#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    "bne"
};

char* pseudo_instr[P_TYPE_LEN] = {
    "li", 
};

void execute(struct CPU *cpu, Instruction instr) {
    cpu->pc += 4;
   if (!strcmp(instr.opcode, "addi") || !strcmp(instr.opcode, "li")) {
        cpu->regs[instr.rd] = cpu->regs[instr.rs1] + instr.imm;
   } else if (!strcmp(instr.opcode, "add")) {
        cpu->regs[instr.rd] = cpu->regs[instr.rs1] + cpu->regs[instr.rs2];
    } else if (!strcmp(instr.opcode, "sub")) {
        cpu->regs[instr.rd] = cpu->regs[instr.rs1] - cpu->regs[instr.rs2];
   } else if (!strcmp(instr.opcode, "and")) {
        cpu->regs[instr.rd] = cpu->regs[instr.rs1] & cpu->regs[instr.rs2];
   } else if (!strcmp(instr.opcode, "or")) {
        cpu->regs[instr.rd] = cpu->regs[instr.rs1] | cpu->regs[instr.rs2];
   } else if (!strcmp(instr.opcode, "xor")) {
        cpu->regs[instr.rd] = cpu->regs[instr.rs1] ^ cpu->regs[instr.rs2];
   } else if (!strcmp(instr.opcode, "andi")) {
        cpu->regs[instr.rd] = cpu->regs[instr.rs1] & instr.imm;
   } else if (!strcmp(instr.opcode, "ori")) {
        cpu->regs[instr.rd] = cpu->regs[instr.rs1] | instr.imm;
   } else if (!strcmp(instr.opcode, "lw")) {
        // We are doing little endian
        uint32_t addr = cpu->regs[instr.rs1] + instr.imm; // Calculate address
        if (addr + 3 >= MEM_SIZE) {
            printf("ERROR: Memory access out of bounds at 0x%x\n", addr);
            return;
        }
        cpu->regs[instr.rd] = cpu->data_mem[addr] // Byte 0
                | (cpu->data_mem[addr+1] << 8)    // Byte 1
                | (cpu->data_mem[addr+2] << 16)   // Byte 2
                | (cpu->data_mem[addr+3] << 24);  // Byte 3
        // So, if data is: 0xAABBCCDD (Byte 3,2,1,0)
        // And I get address: 40
        // Then, starting at 40, I get DD
        // Then, I do 40 + 1, that gives me CC and then I shift it and make it CC00
        // Then, I do 40 + 2, that gives me BB and then I shift it and make it BB0000
        // Then, I do 40 + 3, that gives me AA and then I shift it and make it AA000000
        // Then, add all
   } else if (!strcmp(instr.opcode, "sw")) {
        uint32_t addr = cpu->regs[instr.rs1] + instr.imm; // Calculate address
        if (addr + 3 >= MEM_SIZE) {
            printf("ERROR: Memory access out of bounds at 0x%x\n", addr);
            return;
        }
        // 0xFF -> 0000 0000  0000 0000  0000 0000  1111 1111
        cpu->data_mem[addr]   =  cpu->regs[instr.rs2]        & 0xFF;
        cpu->data_mem[addr+1] = (cpu->regs[instr.rs2] >> 8)  & 0xFF; // Taking the value and shifting it so that second byte is now under FF
        cpu->data_mem[addr+2] = (cpu->regs[instr.rs2] >> 16) & 0xFF; // Taking the value and shifting it so that third byte is now under FF
        cpu->data_mem[addr+3] = (cpu->regs[instr.rs2] >> 24) & 0xFF; // Taking the value and shifting it so that fourth byte is now under FF
        cpu->last_mem_addr = addr;
        cpu->last_mem_val  = cpu->regs[instr.rs2];
   } else if (!strcmp(instr.opcode, "beq")) {
        // beq rs1, rs2, label/imm
        if (cpu->regs[instr.rs1] == cpu->regs[instr.rs2]) {
            cpu->pc = instr.target_addr; // Label address from get_label_address() is in imm
        }
   } else if (!strcmp(instr.opcode, "bne")) {
        if (cpu->regs[instr.rs1] != cpu->regs[instr.rs2]) {
            cpu->pc = instr.target_addr; // Label address from get_label_address() is in imm
        }
   }
   cpu->regs[0] = 0;
}

void cpu_init(struct CPU *cpu) {
    printf("Starting CPU...\n");
    memset(cpu, 0, sizeof(struct CPU));
    cpu->pc = BASE_PC;
    cpu->last_mem_addr = -1;
}