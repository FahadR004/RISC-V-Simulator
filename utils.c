#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "cpu.h"

void print_array(char **arr, int count, const char *label) {
    for (int i = 0; i < count; i++) {
        printf("%s[%d]: %s\n", label, i, arr[i]);
    }
}

void print_execution_step(Instruction instr, struct CPU *cpu) {

    // Print instruction
    printf("> %s ", instr.opcode);

   if (!strcmp(instr.opcode, "beq") || !strcmp(instr.opcode, "bne")) {
        printf("%s, %s, 0x%x", reg_names[instr.rs1], reg_names[instr.rs2], instr.target_addr);
    } else if (!strcmp(instr.opcode, "sw")) {
        printf("%s, %d(%s)", reg_names[instr.rs2], instr.imm, reg_names[instr.rs1]);
    } else if (!strcmp(instr.opcode, "lw")) {
        printf("%s, %d(%s)", reg_names[instr.rd], instr.imm, reg_names[instr.rs1]);
    } else if (!strcmp(instr.opcode, "li")) {
        printf("%s, %d", reg_names[instr.rd], instr.imm);
    } else if (instr.rs2 == -1) {
        // I-type
        printf("%s, %s, %d", reg_names[instr.rd], reg_names[instr.rs1], instr.imm);
    } else {
        // R-type
        printf("%s, %s, %s", reg_names[instr.rd], reg_names[instr.rs1], reg_names[instr.rs2]);
    }

    // Print PC
    printf("  PC: 0x%08x\n", cpu->pc);

    // Print non-zero registers
    printf("Registers: ");
   for (int i = 0; i < REG_NUM; i++) {
        printf("x%d(%s): %d  ", i, reg_names[i], cpu->regs[i]);
    }
    if (cpu->last_mem_addr != (uint32_t)-1) {
        printf("\nMemory[%d] = %d\n", cpu->last_mem_addr, cpu->last_mem_val);
    } 

    cpu->last_mem_addr = -1; // Reset
    printf("\n\n");
}