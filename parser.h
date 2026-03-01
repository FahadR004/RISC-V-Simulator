#ifndef PARSER_H
#define PARSER_H

#include "cpu.h"

int find_labels(FILE *fptr, char *line, char **label_arr, int *addr_arr, struct CPU cpu);

int get_label_address(char *label, char **label_arr, int *addr_arr, int label_count);

int tokenize(char *line, char **token_arr);

int get_register_number(char *reg_name);

Instruction parse(char **tokens, char **label_arr, int *addr_arr, int label_count);

#endif