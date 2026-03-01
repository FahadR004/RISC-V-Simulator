#ifndef UTILS_H
#define UTILS_H

#include "cpu.h"

void print_array(char **arr, int count, const char *label);
void print_execution_step(Instruction instr, struct CPU *cpu);

#endif