lw x1, 0(x0)
beq x1, x0, done

addi x3, x3, 1
beq x1, x3, done
addi x2, x0, 1
addi x4, x4, 1

for : 
    beq x2, x1, done
    add x3, x4, x5
    add x5, x4, x0
    add x4, x3, x0
    addi x2, x2, 1
    jal x6, for

done : 
    sw x3, 1(x0)