# RISCV-Assembler
An assembler for RISC
This generates a corresponding RISCV Machine Level code for a RISCV Assembler
To use the assembler in cmd type: ./rva.exe $filename.asm
The rva.exe generates two files $filename.mc and $filename.trace
$filename.mc: It contains all the machine level instruction corresponding to a PC value
$filename.trace: It contains the dynamic PC of instruction that are executed in that order in the hardware