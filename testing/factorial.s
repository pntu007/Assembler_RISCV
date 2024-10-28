lw x1, 0(x0)
addi x1 , x1 , 1
addi x2, x0, 1
addi x3, x3, 1

for : 
    beq x2, x1, done
    mul x3, x3, x2
    addi x2, x2, 1
    jal x4, for

done : 
    sw x3, 1(x0)