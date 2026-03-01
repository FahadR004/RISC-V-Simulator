# RISC-V Simulator

A lightweight RISC-V 32-bit instruction set simulator written in C, built for the **Pulse Code Hackathon** by CIS Community. It simulates assembly execution step-by-step, displaying register and memory state after each instruction, similar to the Venus simulator (venus.cs61c.org).

---

## Supported Instructions (29 Total)

**R-Type (10):**
`add` `sub` `and` `or` `xor` `sll` `srl` `sra` `slt` `sltu`

**I-Type (10):**
`addi` `andi` `ori` `xori` `slti` `sltiu` `slli` `srli` `srai` `lw`

**S-Type (1):**
`sw`

**B-Type (4):**
`beq` `bne` `blt` `bge`

**Pseudo Instructions (4):**
`li` `mv` `nop` `j`

---

## How to Run

**1. Compile:**
```bash
make
```

**2. Write an assembly program** in `program.asm`. Example:
```asm
# Add two numbers
li t0, 5
li t1, 7
add t2, t0, t1
```

**3. Run the simulator:**
```bash
./risc_v_sim
```

---

## Example Output
```
> li t0, 5
PC: 0x00400004
Registers:
x0: 0 (zero)  x1: 0 (ra)  x2: 0 (sp)  x3: 0 (gp)
x4: 0 (tp)    x5: 5 (t0)  x6: 0 (t1)  x7: 0 (t2)
...

> li t1, 7
PC: 0x00400008
Registers:
x0: 0 (zero)  x1: 0 (ra)  x2: 0 (sp)  x3: 0 (gp)
x4: 0 (tp)    x5: 5 (t0)  x6: 7 (t1)  x7: 0 (t2)
...

> add t2, t0, t1
PC: 0x0040000c
Registers:
x0: 0 (zero)  x1: 0 (ra)  x2: 0 (sp)  x3: 0 (gp)
x4: 0 (tp)    x5: 5 (t0)  x6: 7 (t1)  x7: 12 (t2)
...

Execution completed. Result: t2 = 12
```

---

## Input Format

Assembly is written directly in `program.asm`. Supported syntax:
```asm
# This is a comment
li   t0, 10          # load immediate
addi t1, t0, -3      # immediate arithmetic
add  t2, t0, t1      # register arithmetic
sw   t2, 0(zero)     # store to memory
lw   t3, 0(zero)     # load from memory
beq  t0, t1, label   # conditional branch
label:
    blt t0, t2, label
```

---

## Project Structure
```
riscv-sim/
├── main.c       — entry point, file reading, execution loop
├── cpu.c        — instruction execution, CPU state
├── cpu.h        — CPU struct, instruction struct, constants
├── parser.c     — tokenizer, parser, label resolver
├── parser.h     — parser function declarations
├── utils.c      — print utilities
├── utils.h      — utility function declarations
├── Makefile     — build configuration
└── program.asm  — your assembly program goes here
```

---

## Architecture

The simulator follows a classic fetch-decode-execute pipeline:

1. **Two-pass parsing** — first pass scans for labels and records their addresses, second pass parses all instructions into an internal representation
2. **Array-based execution** — all instructions are loaded into an array and executed using the PC as an index, enabling correct branch and jump behavior
3. **Separate memory spaces** — instruction memory and data memory are kept separate, with data memory byte-addressed and little-endian
4. **Step-by-step output** — after every instruction, the full register file and any memory changes are displayed

---

## Limitations

- Memory is limited to 4096 bytes for data
- Maximum 256 instructions per program
- Maximum 256 labels per program
- Only a subset of the full RISC-V ISA is supported
- No floating point instructions

---

*Built for Pulse Code — CIS Community Hackathon*

