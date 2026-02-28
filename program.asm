add t2, t0, t1
addi t0, x0, 5
li t1, 7
sw t0, 8(sp)
beq t0, t1, loop
