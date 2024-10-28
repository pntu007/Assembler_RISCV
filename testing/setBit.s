lw x18, 0(x0)

l:
    andi x28, x18, 1
    add x19, x19, x28
    srli x18, x18, 1
    beq x18, x0, r
    jal x5, l
r:
    sw x19, 1(x0)