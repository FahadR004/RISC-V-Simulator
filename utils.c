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

    if (!strcmp(instr.opcode, "beq") || !strcmp(instr.opcode, "bne") ||
        !strcmp(instr.opcode, "blt") || !strcmp(instr.opcode, "bge")) {
        // B-type: only rs1, rs2, target — no rd
        printf("%s, %s, 0x%x", reg_names[instr.rs1], reg_names[instr.rs2], instr.target_addr);
    } else if (!strcmp(instr.opcode, "j")) {
        // j label — no registers at all
        printf("0x%x", instr.target_addr);
    } else if (!strcmp(instr.opcode, "sw")) {
        // sw rs2, imm(rs1)
        printf("%s, %d(%s)", reg_names[instr.rs2], instr.imm, reg_names[instr.rs1]);
    } else if (!strcmp(instr.opcode, "lw")) {
        // lw rd, imm(rs1)
        printf("%s, %d(%s)", reg_names[instr.rd], instr.imm, reg_names[instr.rs1]);
    } else if (!strcmp(instr.opcode, "li")) {
        // li rd, imm
        printf("%s, %d", reg_names[instr.rd], instr.imm);
    } else if (!strcmp(instr.opcode, "mv")) {
        // mv rd, rs1
        printf("%s, %s", reg_names[instr.rd], reg_names[instr.rs1]);
    } else if (!strcmp(instr.opcode, "nop")) {
        // no operands
    } else if (instr.rs2 == -1) {
        // I-type: addi, andi, ori, xori, slti, sltiu, slli, srli, srai
        printf("%s, %s, %d", reg_names[instr.rd], reg_names[instr.rs1], instr.imm);
    } else {
        // R-type: add, sub, and, or, xor, sll, srl, sra, slt, sltu
        printf("%s, %s, %s", reg_names[instr.rd], reg_names[instr.rs1], reg_names[instr.rs2]);
    }

    // Print PC
    printf("\nPC: 0x%08x\n", cpu->pc);

    // Print all 32 registers in required format: x0: 0 (zero)
    printf("Registers:\n");
    for (int i = 0; i < REG_NUM; i++) {
        printf("x%d: %d (%s)  ", i, cpu->regs[i], reg_names[i]);
        if ((i + 1) % 4 == 0) printf("\n"); // 4 registers per line for readability
    }

    // Print memory change if sw was executed
    if (cpu->last_mem_addr != (uint32_t)-1) {
        printf("Memory[%d] = %d\n", cpu->last_mem_addr, cpu->last_mem_val);
    }

    cpu->last_mem_addr = -1; // Reset after printing
    printf("\n");
}

void print_json_step(Instruction instr, struct CPU *cpu, 
                     char *json_output, int is_last) {
    char buf[4096];
    char instr_str[64];

    // For instruction string
    if (!strcmp(instr.opcode, "beq") || !strcmp(instr.opcode, "bne") ||
        !strcmp(instr.opcode, "blt") || !strcmp(instr.opcode, "bge")) {
        snprintf(instr_str, sizeof(instr_str), "%s %s, %s, 0x%x",
            instr.opcode, reg_names[instr.rs1], 
            reg_names[instr.rs2], instr.target_addr);
    } else if (!strcmp(instr.opcode, "sw")) {
        snprintf(instr_str, sizeof(instr_str), "%s %s, %d(%s)",
            instr.opcode, reg_names[instr.rs2], 
            instr.imm, reg_names[instr.rs1]);
    } else if (!strcmp(instr.opcode, "lw")) {
        snprintf(instr_str, sizeof(instr_str), "%s %s, %d(%s)",
            instr.opcode, reg_names[instr.rd], 
            instr.imm, reg_names[instr.rs1]);
    } else if (!strcmp(instr.opcode, "li")) {
        snprintf(instr_str, sizeof(instr_str), "%s %s, %d",
            instr.opcode, reg_names[instr.rd], instr.imm);
    } else if (!strcmp(instr.opcode, "mv")) {
        snprintf(instr_str, sizeof(instr_str), "%s %s, %s",
            instr.opcode, reg_names[instr.rd], reg_names[instr.rs1]);
    } else if (!strcmp(instr.opcode, "nop") || !strcmp(instr.opcode, "j")) {
        snprintf(instr_str, sizeof(instr_str), "%s", instr.opcode);
    } else if (instr.rs2 == -1) {
        snprintf(instr_str, sizeof(instr_str), "%s %s, %s, %d",
            instr.opcode, reg_names[instr.rd], 
            reg_names[instr.rs1], instr.imm);
    } else {
        snprintf(instr_str, sizeof(instr_str), "%s %s, %s, %s",
            instr.opcode, reg_names[instr.rd], 
            reg_names[instr.rs1], reg_names[instr.rs2]);
    }

    // For register array string
    char regs_str[512] = "[";
    char tmp[32];
    for (int i = 0; i < REG_NUM; i++) {
        snprintf(tmp, sizeof(tmp), "%d%s", 
                 cpu->regs[i], i < REG_NUM - 1 ? "," : "");
        strcat(regs_str, tmp);
    }
    strcat(regs_str, "]");

    // For memory string
    char mem_str[64] = "null";
    if (cpu->last_mem_addr != (uint32_t)-1) {
        snprintf(mem_str, sizeof(mem_str), 
                 "{\"addr\": %d, \"val\": %d}",
                 cpu->last_mem_addr, cpu->last_mem_val);
    }

    // Entire JSON object
    snprintf(buf, sizeof(buf),
        "  {\n"
        "    \"instr\": \"%s\",\n"
        "    \"pc\": \"0x%08x\",\n"
        "    \"regs\": %s,\n"
        "    \"mem\": %s\n"
        "  }%s\n",
        instr_str,
        cpu->pc,
        regs_str,
        mem_str,
        is_last ? "" : ","
    );

    strcat(json_output, buf);
}