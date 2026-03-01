#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "cpu.h"
#include "utils.h"  

struct CPU cpu;

int main(int argc, char* argv[]) {

    // Initialize CPU. Initialize Registers and Memory with all 0s
    cpu_init(&cpu);

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-json")) {
            cpu.json_mode = 1;
        }
    }

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
    rewind(fptr);
    
    char *label_arr[256];
    int addr_arr[256];
    int label_count = find_labels(fptr, line, label_arr, addr_arr, cpu);

    // Assembler
    printf("\n");
    printf("\nPARSING...\n");
    rewind(fptr); // Resetting file pointer to the start of the file
    char *token_arr[4];
    Instruction instructions[256];
    int instr_count = 0;
    while (fgets(line, sizeof(line), fptr)) {
        line[strcspn(line, "\r\n")] = '\0'; // This will remove newline characters from the line
        if (line[strlen(line) - 1] == ':') continue;
        if (line[0] == '\0' || line[0] == '#') continue;
        
        tokenize(line, token_arr);
        instructions[instr_count] = parse(token_arr, label_arr, addr_arr, label_count); // Get instruction structure from tokens
        instr_count++;
    }
    printf("Parsing completed. Total instructions: %d\n\n", instr_count);
    printf("Initial PC set to 0x%08x\n", cpu.pc);

    char json_output[1024 * 1024]; // 1MB buffer — enough for any program
    json_output[0] = '[';
    printf("Starting execution...\n\n");
    int i = 0;
    while (i < instr_count) {
        execute(&cpu, instructions[i]);
         if (cpu.json_mode) {
            int is_last = ((int)(cpu.pc - BASE_PC) / 4 >= instr_count);
            print_json_step(instructions[i], &cpu, json_output, is_last);
        } else {
            print_execution_step(instructions[i], &cpu);
        }
        i = (cpu.pc - BASE_PC)/4; // CPU PC will always be bigger or equal to BASE_PC but never less
    }

    printf("Execution completed.\n");

    if (cpu.json_mode) {
    strcat(json_output, "]\n");
        
        FILE *out = fopen("output.json", "w");
        if (out) {
            fprintf(out, "%s", json_output);
            fclose(out);
            printf("JSON written to output.json\n");
        } else {
            perror("Could not write output.json");
        }
    }
    for (int i = 0; i < REG_NUM; i++) {
        if (cpu.regs[i] != 0)
            printf("Result: %s (x%d) = %d\n", reg_names[i], i, cpu.regs[i]);
    }

    for (int i = 0; i < label_count; i++) { // strdup uses malloc so we need to free that memory
        free(label_arr[i]);
    }
    fclose(fptr);


    return 0;
}