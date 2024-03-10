# merge sort

#void mergesort(int array[], int low, int high){
#    if (high-low == 0) return;
#    int mid = (high+low)/2;
#    mergesort(array, low, mid);
#    mergesort(array, mid+1, high);
#    # now merge the two arrays (low to mid) and (mid+1 to high)
#    int A[mid-low+1], B[high-mid];
#    for (int i=0; i<=mid-low; i++){
#        A[i] = array[low+i];
#    }
#    for (int i=0; i<=high-mid-1; i++){
#        B[i] = array[mid+1+i];
#    }
#    int i=0, j=0;
#    while (i <= mid-low && j <= high-mid-1){
#        if (A[i] < B[j]){
#            array[low+i+j] = A[i];
#            i++;
#        } else {
#            array[low+i+j] = B[j];
#            j++;
#        }
#    }
#    # either i == mid-low+1 or j == high-mid
#    if (i == mid-low){
#        while (j <= high-mid-1){
#            array[low+i+j] = B[j];
#            j++;
#        }
#    } else {
#        while (i <= mid-low){
#            array[low+i+j] = A[i];
#            i++;
#        }
#    }
#}

.data
size: .word 10
array: .word 5 1 2 3 10 4 6 6 6 6

.text
# x18 -> base address of the array
# x19 -> value of high
# x20 -> value of low
# x21 -> base address of A
# x22 -> base address of B
# x28 -> i
# x29 -> j
# x30 -> mid
# x31 -> value of index in array[index] and some temp values also
beq x0, x0, main
mergesort:
    # since the basecondition directly returns not making further recursive calls we don't need to update the stack pointer...
    # ... until we reach a recursive condition
    basecondition1: bne x19, x20, notbasecase1
    jalr x0, x1, 0
    notbasecase1: 
        add x30, x19, x20 # high+low
        srli x30, x30, 1 # (high+low)/2 # srli == shift right logical immediate
        # now we would make a recursive call hence we need to store current x1 in the stack
        # push the current x1 return address to the stack
        # stack is filled from top to bottom push operation and heap memory is filled from bottom to top
        addi x2, x2, -16 # allocating 12 bytes of space to the stack
        sw x1, 16(x2) # x2 is the stack pointer
        sw x19, 12(x2) # storing current value of high
        sw x20, 8(x2) # storing current value of low
        sw x30, 4(x2) # storing current value of mid
        # our stack pointer always points towards the empty memory location
        # checking the base condition
        # now changing the value of high and low for the recursive call
        addi x19, x30, 0
        jal x1, mergesort
        
        # in the second recursive call we have low as mid+1 and high as original high
        addi x20, x19, 1 # mid+1
        lw x19, 12(x2)
        jal x1, mergesort
        
        # now load back all the variables back
        lw x1, 16(x2) # no need to store x1 now as there are no further recursive calls
        lw x19, 12(x2)
        lw x20, 8(x2)
        lw x30, 4(x2)
        addi x2, x2, 16 # free up not required memory
        # now stack has only one value which is x1
        # now we will create the array A and array B in the stack memory
        # assigning a total of high-low+1 memory in the stack
        sub x31, x19, x20
        addi x31, x31, 1
        slli x31, x31, 2
        sub x2, x2, x31 # assigning the memory in the stack
        addi x28, x0, 0 # i=0
        # x5 = mid - low
        sub x5, x30, x20
        sub x31, x19, x30 # high-mid
        slli x31, x31, 2
        addi x31, x31, 4 # since x2 stack pointer points to an empty location
        # base address of A and B
        addi x21, x0, 0 # setting them to 0
        addi x22, x0, 0 # setting them to 0
        add x21, x2, x31
        addi x22, x2, 4 # since x2 stack pointer points to an empty location
        # 1st for loop
        for_loop1: 
            bgt x28, x5, for_loop1_exit
            # b[i] = array[mid+i+1]
            slli x6, x28, 2 # 4*i
            slli x7, x20, 2 # 4*low
            add x7, x7, x6 # x7 = 4*low + 4*i
            # using x23, x24, x25 as a temporary register
            addi x23, x0, 0 # x23 = 0
            add x23, x18, x7 # x23 is pointing to array[low+i]
            lw x24, 0(x23) # x24 = array[low+i]
            addi x25, x0, 0
            add x25, x21, x6 # x25 is poiting to a[i]
            sw x24, 0(x25)
            # now i++
            addi x28, x28, 1
            beq x0, x0, for_loop1
        for_loop1_exit:
        # now x5 = high - mid - 1
        addi x5, x0, 0
        sub x5, x19, x30 # x5 = high - mid
        addi x5, x5, -1 # x5 = high - mid - 1
        addi x28, x0, 0 # i=0
        # second for loop
        for_loop2: 
            bgt x28, x5, for_loop2_exit
            # a[i] = array[low+i]
            slli x6, x28, 2 # 4*i
            slli x7, x30, 2 # 4*mid
            add x7, x7, x6 # x7 = 4*mid + 4*i
            addi x7, x7, 4 # x7 = 4*(mid+i+1)
            # using x23, x24, x25 as a temporary register
            addi x23, x0, 0 # x23 = 0
            add x23, x18, x7 # x23 is pointing to array[mid+i+1]
            lw x24, 0(x23) # x24 = array[low+i]
            addi x25, x0, 0
            add x25, x22, x6 # x25 is pointing to b[i]
            sw x24, 0(x25)
            # now i++
            addi x28, x28, 1
            beq x0, x0, for_loop2
       for_loop2_exit: 
       addi x28, x0, 0 # i = 0
       addi x29, x0, 0 # j = 0
       # x5 = high-mid-1
       # x6 = mid-low
       # x5 is already having that value because of for_loop2
       # setting the value of x6 as mid-low
       sub x6, x30, x20
       while_loop1:
            cond1: bgt x28, x6, while_loop1_exit
            cond2: bgt x29, x5, while_loop1_exit
            # now we need to calculate a[i] and b[j]
            slli x7, x28, 2 # 4*i
            slli x8, x29, 2 # 4*j
            add x7, x7, x21 # a[i]
            add x8, x8, x22 # b[j]
            # now storing value of a[i] in x23 and value of b[j] in x24
            lw x23, 0(x7)
            lw x24, 0(x8)
            if_cond1: 
                bge x23, x24, else1 # a[i] >= b[j]
                add x31, x28, x29 # x31 = i+j
                add x31, x31, x20 # x31 = low+i+j
                slli x31, x31, 2 # x31 = 4*(low+i+j)
                add x31, x31, x18 # x31 = array[low+i+j]
                sw x23, 0(x31) # array[low+i+j] = a[i]
                addi x28, x28, 1 # i++
                beq x0, x0, if_cond1_end
            else1:
                add x31, x28, x29 # x31 = i+j
                add x31, x31, x20 # x31 = low+i+j
                slli x31, x31, 2 # x31 = 4*(low+i+j)
                add x31, x31, x18 # x31 = array[low+i+j]
                sw x24, 0(x31) # array[low+i+j] = b[j]
                addi x29, x29, 1 # j++
                beq x0, x0, if_cond1_end
            if_cond1_end:
            beq x0, x0, while_loop1
       while_loop1_exit:
       # check if i == x6+1 or j == x5+1
       if_cond2:
            ble x28, x6, else2
            # go here when i is already completed
            add x31, x20, x28
            slli x31, x31, 2 # x31 = 4*(low+i)
            while_loop2:
                bgt x29, x5, while_loop2_exit
                slli x8, x29, 2 # 4*j
                add x8, x8, x22 # b[j]
                lw x24, 0(x8) # x24 = b[j]
                slli x7, x29, 2 # x7 = 4*j
                add x7, x7, x31 # x7 = 4*(low+i+j)
                add x7, x7, x18 # x7 = array[low+i+j]
                sw x24, 0(x7) # array[low+i+j] = b[j]
                addi x29, x29, 1 # j++
                beq x0, x0, while_loop2
            while_loop2_exit:
            beq x0, x0, if_cond2_exit
       else2:
            # go here when j is already completed
            add x31, x20, x29
            slli x31, x31, 2 # x31 = 4*(low+j)
            while_loop3:
                bgt x28, x6, while_loop3_exit
                slli x8, x28, 2 # 4*i
                add x8, x8, x21 # a[i]
                lw x23, 0(x8) # x24 = a[i]
                slli x7, x28, 2 # x7 = 4*i
                add x7, x7, x31 # x7 = 4*(low+i+j)
                add x7, x7, x18 # x7 = array[low+i+j]
                sw x23, 0(x7) # array[low+i+j] = a[i]
                addi x28, x28, 1 # i++
                beq x0, x0, while_loop2
            while_loop3_exit:
            beq x0, x0, if_cond2_exit
       if_cond2_exit:
       # now free all the stack memory
       # total memory assigned was high-low+1
       sub x31, x19, x20
       addi x31, x31, 1 # x31 = high-low+1
       slli x31, x31, 2 # x31 = 4*(high-low+1)
       add x2, x2, x31 # x2 = x2 + x31
       jalr x0, x1, 0 # x1 was removed from the stack long time ago when there was no further recursive calls
main:
    lui x18, 0x10000
    lw x19, 0(x18)
    addi x18, x18, 4 # now x18 points towards base of array
    addi x19, x19, -1 # high = total_size_of_array - 1
    addi x20, x0, 0 # low = 0
    jal x1, mergesort # calling the mergesort function