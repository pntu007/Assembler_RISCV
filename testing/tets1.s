addi x18, x18,516
l:
andi x28,x18,1
add x19, x19, x28
srli x18,x18,1
beq x18,x0, r
jal x5, l
r: