#ifndef PARSER_H
#define PARSER_H

#include "cpu.h"

int tokenize(char *line, char **token_arr);

int get_register_number(char *reg_name);

Instruction parse(char **tokens, int token_count);

#endif