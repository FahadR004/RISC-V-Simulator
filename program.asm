# Fibonacci Sequence
li t0, 0
li t1, 1
li t2, 10
li t3, 0
loop:
add t4, t0, t1
mv t0, t1
mv t1, t4
addi t3, t3, 1
blt t3, t2, loop