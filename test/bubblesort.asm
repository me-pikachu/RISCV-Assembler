# code for bubble sort
#for (int i=0; i<n-1; i++){
#    for (int j=0; j<n-1; j++){
#        if (a[j] > a[j+1]){
#            swap(a[j], a[j+1])
#        }
#    }
#}

.data
size: .word 5 # size of the array
array: .word 10, 6, 7, 5, 1 # the array itself

.text
# base address of the array -> x18
# n -> x19
# i -> x28
# j -> x29
# load the size of the array in a variable
lui x18, 0x10000
lw x19, 0(x18)
addi x19, x19, -1 # since we don't need n we actually need n-1
addi x18, x18, 4 # now x18 points to the base address of the array
slt x28, x0, x0 # i = 0
slt x29, x0, x0 # j = 0
for_loop1: bge x28, x19, for_loop_exit1
    slt x29, x0, x0 # set j = 0
    for_loop2: bge x29, x19, for_loop_exit2
        # load array[j] in x5 and load array[j+1] in x6
        slt x31, x0, x0 # initialising x31 to 0
        addi x30, x29, 0 # x30 = j
        slli x30, x30, 2 # x30 = 4*j
        add x31, x31, x18 # x31 = base address
        add x31, x31, x30 # x31 = base address + offset
        # we have address of array[j] in x31 now
        lw x5, 0(x31) # array[j]
        lw x6, 4(x31) # array[j+1]
        # now compare x5 and x6
        ble x5, x6, if_exit1
        # now swap x5 and x6
        sw x5, 4(x31)
        sw x6, 0(x31)
        # if loop ends
        if_exit1: addi x29, x29, 1
        beq x0, x0, for_loop2
    for_loop_exit2: addi x28, x28, 1
    beq x0, x0, for_loop1
for_loop_exit1:
