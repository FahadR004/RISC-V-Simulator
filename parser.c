#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "cpu.h"

int find_labels(FILE *fptr, char *line, char **label_arr, int *addr_arr, struct CPU cpu) {
    int addr_count = cpu.pc;
    int counter = 0;

    while (fgets(line, 256, fptr)) {
        line[strcspn(line, "\r\n")] = '\0'; // This will remove newline characters from the line
        // printf("PC %x,\n CHAR: %c,\n LINE: %s\n", addr_count, line[strlen(line)-1], line);
        if (line[strlen(line) - 1] == ':') {
            line[strlen(line) - 1] = '\0'; // Placing a null terminating char at : to signify ending of string
            label_arr[counter] = strdup(line); // Allocates new memory and copies the string into memory. Now, it will label_arr pointer will point to the correct value       
            addr_arr[counter] = addr_count;
            counter++;
        } else if (line[0] != '\0' && line[0] != '#') {
            addr_count += 4;
        }
    }

    return counter; 
}

int get_label_address(char *label, char **label_arr, int *addr_arr, int label_count) {
    for (int i = 0; i < label_count; i++) {
        if (!strcmp(label, label_arr[i])) {
            return addr_arr[i];
        }
    }
    printf("ERROR: Label '%s' not found\n", label);
    return -1;
}

int tokenize(char *line, char **token_arr) { // Pointer to an array of pointers
    char *outer_save_ptr;
    int token_count = 0;

    char *token = strtok_r(line, " ,()", &outer_save_ptr);
    while (token != NULL) {
        token_arr[token_count] = token;
        token_count++;
        token = strtok_r(NULL, " ,()", &outer_save_ptr);
    }

    return token_count; 
}

int get_register_number(char *reg_name) { // reg-names is the array
    int reg_names_len = REG_NUM;
    if (reg_name[0] == 'x') {
        return atoi(reg_name + 1); // Returning a pointer that starts one char later
    }
    if (!strcmp(reg_name, "fp")) return 8;
    for (int i = 0; i < reg_names_len; i++) {
        if (!strcmp(reg_name, reg_names[i])){ // returns 0 if identical
            return i;
        };
    }
    return -1;
}

Instruction parse(char **tokens, char **label_arr, int * addr_arr, int label_count) {
    Instruction instr = { 0 };
    strcpy(instr.opcode, tokens[0]);
    int found = 0;
    // R-Type Check
    for (int i = 0; i < R_TYPE_LEN; i++) {
        if (!strcmp(tokens[0], r_type_instr[i])){ // returns 0 if identical
            instr.rd = get_register_number(tokens[1]);
            instr.rs1 = get_register_number(tokens[2]); 
            instr.rs2 = get_register_number(tokens[3]);
            instr.imm = 0;
            found = 1;
            break;
        };
    }
    if (found) return instr;

     // I-Type: addi, andi, ori, lw
    // Format: opcode rd, rs1, imm
    for (int i = 0; i < I_TYPE_LEN; i++) {
        if (!strcmp(tokens[0], i_type_instr[i])) {
            if (!strcmp(tokens[0], "lw")) {
                // For lw -> lw rd, imm(rs1)
                instr.rd  = get_register_number(tokens[1]);
                instr.imm = atoi(tokens[2]);
                instr.rs1 = get_register_number(tokens[3]);
                instr.rs2 = -1; // no rs2 for lw
            } else {
                instr.rd  = get_register_number(tokens[1]);
                instr.rs1 = get_register_number(tokens[2]);
                instr.rs2 = -1; // no rs2 for I-type
                instr.imm = atoi(tokens[3]);
            }
            found = 1;
            break;
        }
    }
    if (found) return instr;

    // S-Type: sw
    // Format: sw rs2, imm(rs1)
    for (int i = 0; i < S_TYPE_LEN; i++) {
        if (!strcmp(tokens[0], s_type_instr[i])) {
            instr.rs2 = get_register_number(tokens[1]);
            instr.imm = atoi(tokens[2]);
            instr.rs1 = get_register_number(tokens[3]);
            instr.rd  = -1; // no rd for S-type
            found = 1;
            break;
        }
    }
    if (found) return instr;

    // B-Type: beq
    // Format: beq rs1, rs2, label/imm
    for (int i = 0; i < B_TYPE_LEN; i++) {
        if (!strcmp(tokens[0], b_type_instr[i])) {
            instr.rs1 = get_register_number(tokens[1]);
            instr.rs2 = get_register_number(tokens[2]);
            instr.target_addr = get_label_address(tokens[3], label_arr, addr_arr, label_count); 
            instr.rd  = -1; // no rd for B-type
            instr.imm = 0;
            found = 1;
            break;
        }
    }
    if (found) return instr;

    // Pseudo: li (load immediate)
    // Format: li rd, imm
    // Expands to addi rd, x0, imm
    for (int i = 0; i < P_TYPE_LEN; i++) {
        if (!strcmp(tokens[0], pseudo_instr[i])) {
            instr.rd  = get_register_number(tokens[1]);
            instr.rs1 = 0;  // x0
            instr.rs2 = -1;
            instr.imm = atoi(tokens[2]);
            found = 1;
            break;
        }
    }
    if (found) return instr;
   
    return instr;
}


// Original 
// char *token = strtok_r(line, ",", &outer_save_ptr);
// while (token != NULL) {
//     token_count++;
//     printf("TOKEN: %s\n", token); // Print the token
//     if (token_count == 1) {
//         char *inner = strtok_r(token, " ", &inner_save_ptr);
//         int inner_count = 0;
//         while (inner != NULL) { 
//             token_arr[inner_count] = inner;
//             inner_count++;
//             printf("INNER TOKEN: %s\n", inner);
//             inner = strtok_r(NULL, " ", &inner_save_ptr);
//         }
//         token_count = inner_count;
//         token = strtok_r(NULL, ",", &outer_save_ptr); // Advance to next outer token
//         continue;
//     }
//     token_arr[token_count-1] = token;
//     token = strtok_r(NULL, ",", &outer_save_ptr); // Continue from the last position
// }
// char *token = strtok_r(line, " ,", &outer_save_ptr); // This means comma OR space
// while (token != NULL) {
//     token_arr[token_count] = token;
//     token_count++;
//     token = strtok_r(NULL, " ,", &outer_save_ptr);
// }   
// for (int i = 0; i < 4; i++) {
//     printf("%s\n", token_arr[i]);
// }