# this is some random code
.data
hello: .half 45 10
str: .asciiz "hello world"

.text:
addi x1, x2, -0b11111111111111111111111111111111
bne x3, x4, label1
lw x5,-0xfffffffe(x6)
sub x7,x8,x9
label1: sw x10,0(x11)