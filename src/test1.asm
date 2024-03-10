.data
n: .word 3
A: .byte 65
B: .byte 66
C: .byte 67

.text
addi x10,x0,1
slli x10,x10,16
addi x10,x10,1
slli x10,x10,12

addi x9,x0,1
auipc x5,65536
lw x5, -20(x5)
auipc x6, 65536
lb x6, -24(x6)
auipc x7, 65536
lb x7 ,-30(x7)
auipc x8, 65536
lb x8, -39(x8)
jal x1,toh
beq x0,x0,exit

toh:
    addi x2,x2,-20
    sw x5,0(x2)
    sw x6,4(x2)
    sw x7,8(x2)
    sw x8,12(x2)
    sw x1,16(x2)
    
    beq x5,x9,l1
    addi x5,x5,-1
    lw x6,4(x2)
    lw x7,12(x2)
    lw x8,8(x2)
    jal x1,toh
    
    lw x5,0(x2)
    lw x6,4(x2)
    lw x7,8(x2)
    lw x8,12(x2)
    
    add x11,x0,x5
    slli x11,x11,8
    add x11,x11,x6
    slli x11,x11,8
    add x11,x11,x7
    sw x11,0(x10)
    addi x10,x10,4
    
    addi x5,x5,-1
    lw x6,12(x2)
    lw x7,8(x2)
    lw x8,4(x2)
    jal x1,toh
    
    lw x1,16(x2)
    addi x2,x2,20
    jalr x0,0(x1)
    
    l1:
        add x11,x0,x5
        slli x11,x11,8
        add x11,x11,x6
        slli x11,x11,8
        add x11,x11,x7
        sw x11,0(x10)
        addi x10,x10,4
        lw x1,16(x2)
        addi x2,x2,20
        jalr x0,0(x1)
    
exit: