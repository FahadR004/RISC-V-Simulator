#include <stdio.h>
#include "utils.h"
#include "cpu.h"

void print_array(char **arr, int count, const char *label) {
    for (int i = 0; i < count; i++) {
        printf("%s[%d]: %s\n", label, i, arr[i]);
    }
}

void print_execution_step(Instruction instr, struct CPU *cpu) {
    // Reconstruct instruction string based on opcode
    char instr_str[64];

    if (instr.rs2 != -1 && instr.imm == 0) {
        // R-type: add t0, t1, t2
        snprintf(instr_str, sizeof(instr_str), "%s %s, %s, %s",
            instr.opcode,
            reg_names[instr.rd],
            reg_names[instr.rs1],
            reg_names[instr.rs2]);
    } else if (instr.rs2 == -1 && instr.rd != -1 && instr.rs1 == 0) {
        // Pseudo: li t0, 5
        snprintf(instr_str, sizeof(instr_str), "%s %s, %d",
            instr.opcode,
            reg_names[instr.rd],
            instr.imm);
    } else if (instr.rs2 == -1) {
        // I-type: addi t0, t1, 5
        snprintf(instr_str, sizeof(instr_str), "%s %s, %s, %d",
            instr.opcode,
            reg_names[instr.rd],
            reg_names[instr.rs1],
            instr.imm);
    } else {
        // B-type: beq t0, t1, label
        snprintf(instr_str, sizeof(instr_str), "%s %s, %s, 0x%x",
            instr.opcode,
            reg_names[instr.rs1],
            reg_names[instr.rs2],
            instr.imm);
    }

    // Print instruction and PC
    printf("> %s  PC: 0x%08x\n", instr_str, cpu->pc);

    // Print registers — only non zero ones plus always print x0
    printf("Registers: ");
    for (int i = 0; i < REG_NUM; i++) {
        if (cpu->regs[i] != 0 || i == 0) {
            printf("x%d: %d (%s)  ", i, cpu->regs[i], reg_names[i]);
        }
    }
    printf("\n\n");
}