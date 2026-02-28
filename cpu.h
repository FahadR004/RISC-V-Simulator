
#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#define REG_NUM 32 // Or register numbers
#define MEM_SIZE 4096
#define PC 0x00400000  

#define R_TYPE_LEN 5
#define I_TYPE_LEN 4
#define S_TYPE_LEN 1
#define B_TYPE_LEN 1
#define P_TYPE_LEN 1

struct CPU {
   int32_t regs[REG_NUM];
   uint32_t pc;
   int8_t mem[MEM_SIZE];
};

typedef struct {
   char opcode[16];
   int  rd;
   int  rs1;
   int  rs2;
   int  imm;
} Instruction;

void cpu_init(struct CPU *cpu);

extern char *reg_names[REG_NUM];

extern char *r_type_instr[R_TYPE_LEN];
extern char *i_type_instr[I_TYPE_LEN];
extern char *s_type_instr[S_TYPE_LEN];
extern char *b_type_instr[B_TYPE_LEN];
extern char *pseudo_instr[P_TYPE_LEN];

#endif