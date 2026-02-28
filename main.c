#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "cpu.h"
#include "utils.h"  

struct CPU cpu;

int main() {

    // Initialize CPU. Initialize Registers and Memory with all 0s
    cpu_init(&cpu);
    
    // Read Asm File
    printf("Reading Assembly Program...\n");
    FILE *fptr;
    fptr = fopen("program.asm", "r");

    if(fptr == NULL) {
        perror("Unable to open the file");
        exit(1);
    }
    
    char line[256];
    while (fgets(line, sizeof(line), fptr)) {
        printf("%s", line);
    }

    // Assembler
    printf("\n");
    printf("PARSING...\n");
    rewind(fptr); // Resetting file pointer to the start of the file

    char *token_arr[4];
    while (fgets(line, sizeof(line), fptr)) {
        
        line[strcspn(line, "\r\n")] = '\0'; // This will remove newline characters from the line
        
        int token_count = tokenize(line, token_arr);
        print_array(token_arr, token_count, "TOKEN");
        
        Instruction instr = parse(token_arr, token_count); // Get instruction structure from tokens
        
        printf("Instruction: %s, rd: %d, rs1: %d, rs2: %d, imm: %d\n", instr.opcode, instr.rd, instr.rs1, instr.rs2, instr.imm);
    }


    // Decode the hex code

    // Execute the instructions

    // Store in memory

    // Write to registers

    fclose(fptr);


    return 0;
}