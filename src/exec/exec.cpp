#include <bits/stdc++.h>

using namespace std;

long long PC = 0;

int x0 = 0;
int x1 = 0;
int x2 = 2147483612;
int x3 = 268435456;
int x4 = 0;
int x5 = 0;
int x6 = 1;
int x7 = 0;
int x8 = 0;
int x9 = 0;
int x10 = 1;
int x11 = 2147483612;
int x12 = 0;
int x13 = 0;
int x14 = 0;
int x15 = 0;
int x16 = 0;
int x17 = 0;
int x18 = 0;
int x19 = 0;
int x20 = 0;
int x21 = 0;
int x22 = 0;
int x23 = 0;
int x24 = 0;
int x25 = 0;
int x26 = 0;
int x27 = 0;
int x28 = 0;
int x29 = 0;
int x30 = 0;
int x31 = 0;

map<int, int> mod_mem; // all the memory having some non zero values
// modified_memory

std::map<int, int>::iterator bin_search_add(int address){
	
}

void bin_insert_add(int address, int val){
	
}

int memoryread(int address){
	// it reads 1 byte of val at the given memory
	// if the given memory is present in mod_mem then return it's val else return 0
	auto it = bin_search_add(address);
	if (it->first == address) return it->second;
	else return 0;
}

int memorywrite(int address, int val){
	// it writes 1 byte of val at the given memory
	auto it = bin_search_add(address);
	if (it->first == address){
		it->second = val;
	} else {
		bin_insert_add(address, val);
	}
}

int memory(int address, int size, bool read, int writedata = 0){
	// size is 1 for byte, 2 for half word, 4 for word and 8 for double word
	// double word is not supported!
	
	if (read){
		// we have to read from the memory
		if (size == 1){
			return memoryread(address);
		} else if (size == 2){
			return memoryread(address+1) << 8 + memoryread(address);
		} else if (size == 4){
			return memoryread(address+3) << 24 + memoryread(address+2) << 16 + memoryread(address+1) << 8 + memoryread(address);
		}
	} else {
		if (size == 1){
			// write 8 bits of writedata to memory
			int val = writedata;
			// performing bitwise and
			val = val & 0x000000ff; // read the last byte
			memorywrite(address, val);
		} else if (size == 2){
			int val = writedata;
			val = val & 0x000000ff; // last byte
			memorywrite(address, val);
			val = writedata;
			val = val & 0x0000ff00; // first byte
			memorywrite(address+1, val);
		} else if (size == 4){
			int val = writedata;
			val = val & 0x000000ff; // last byte
			memorywrite(address, val);
			val = writedata;
			val = val & 0x0000ff00; // second last byte
			memorywrite(address+1, val);
			val = writedata;
			val = val & 0x00ff0000; // second byte
			memorywrite(address+2, val);
			val = writedata;
			val = val & 0xff000000; // first byte
			memorywrite(address+3, val);
		}
	}
}

int binstr2dec(string binstr){
	// this function converts a binary string of a number to an integer
	int num = 0;
	
	for (int i=binstr.length(); i>=0; i--){
		if (binstr[i] == '-'){
			num = -1*num; // the number is negative
		} else {
			num = num*2 + (binstr[i]-'0'); // numstr[i] - '0' converts the char to digit
		}
	}
	return num;
}

int* idreg(string reg){
	// return the address of the register based upon the input string reg
	if (reg == "00000") {
        return &x0;
    } else if (reg == "00001") {
        return &x1;
    } else if (reg == "00010") {
        return &x2;
    } else if (reg == "00011") {
        return &x3;
    } else if (reg == "00100") {
        return &x4;
    } else if (reg == "00101") {
        return &x5;
    } else if (reg == "00110") {
        return &x6;
    } else if (reg == "00111") {
        return &x7;
    } else if (reg == "01000") {
        return &x8;
    } else if (reg == "01001") {
        return &x9;
    } else if (reg == "01010") {
        return &x10;
    } else if (reg == "01011") {
        return &x11;
    } else if (reg == "01100") {
        return &x12;
    } else if (reg == "01101") {
        return &x13;
    } else if (reg == "01110") {
        return &x14;
    } else if (reg == "01111") {
        return &x15;
    } else if (reg == "10000") {
        return &x16;
    } else if (reg == "10001") {
        return &x17;
    } else if (reg == "10010") {
        return &x18;
    } else if (reg == "10011") {
        return &x19;
    } else if (reg == "10100") {
        return &x20;
    } else if (reg == "10101") {
        return &x21;
    } else if (reg == "10110") {
        return &x22;
    } else if (reg == "10111") {
        return &x23;
    } else if (reg == "11000") {
        return &x24;
    } else if (reg == "11001") {
        return &x25;
    } else if (reg == "11010") {
        return &x26;
    } else if (reg == "11011") {
        return &x27;
    } else if (reg == "11100") {
        return &x28;
    } else if (reg == "11101") {
        return &x29;
    } else if (reg == "11110") {
        return &x30;
    } else if (reg == "11111") {
        return &x31;
    } else {
        return &x0; // no changes to be made
    }
}

// instruction supported : 
// i format : lb, lh, lw, addi, ori, andi, xori, slti, sgti, slei, sgei, jalr
// r format :
// s format : sb, sh, sw
// sb format :
// u format : auipc
// uj format :
void exec_ext_bin_inst(string* ins){
	if (ins[0] == "sb"){
		// address is rs1 + immediate
		// value to store is in rs2
		int address = *idreg(ins[2]) + binstr2dec(ins[4]);
		memory(address, 1, false, *idreg(ins[3]));
		PC = PC + 4;
		
	} else if (ins[0] == "sh"){
		// address is rs2 + immediate
		// value to store is in rs1
		int address = *idreg(ins[2]) + binstr2dec(ins[4]);
		memory(address, 2, false, *idreg(ins[3]));
		PC = PC + 4;
		
	} else if (ins[0] == "sw"){
		// address is rs1 + immediate
		// value to store is in rs2
		int address = *idreg(ins[2]) + binstr2dec(ins[4]);
		memory(address, 4, false, *idreg(ins[3]));
		PC = PC + 4;
		
	} else if (ins[0] == "lb"){
		// address is rs1 + immediate
		int address = *idreg(ins[2]) + binstr2dec(ins[4]); // ins[2] is rs1 and ins[4] is imm
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = memory(address, 1, true); // ins[1] is rd
		PC = PC + 4;
		
	} else if (ins[0] == "lh"){
		// address is rs1 + immediate
		int address = *idreg(ins[2]) + binstr2dec(ins[4]); // ins[2] is rs1 and ins[4] is imm
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = memory(address, 2, true); // ins[1] is rd
		PC = PC + 4;
		
	} else if (ins[0] == "lw"){
		// address is rs1 + immediate
		int address = *idreg(ins[2]) + binstr2dec(ins[4]); // ins[2] is rs1 and ins[4] is imm
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = memory(address, 4, true); // ins[1] is rd
		PC = PC + 4;
		
	} else if (ins[0] == "addi"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) + binstr2dec(ins[4]);
		PC = PC + 4;
		
	} else if (ins[0] == "ori"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) | binstr2dec(ins[4]);
		PC = PC + 4;
		
	} else if (ins[0] == "andi"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) & binstr2dec(ins[4]);
		PC = PC + 4;
		
	} else if (ins[0] == "xori"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) ^ binstr2dec(ins[4]);
		PC = PC + 4;
		
	} else if (ins[0] == "slti"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) < binstr2dec(ins[4]) ? 1 : 0;
		PC = PC + 4;
		
	} else if (ins[0] == "sgti"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) > binstr2dec(ins[4]) ? 1 : 0;
		PC = PC + 4;
		
	} else if (ins[0] == "slei"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) <= binstr2dec(ins[4]) ? 1 : 0;
		PC = PC + 4;
		
	} else if (ins[0] == "sgei"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) >= binstr2dec(ins[4]) ? 1 : 0;
		PC = PC + 4;
	
	} else if (ins[0] == "jalr"){
		// the destination register cannot be x0
		// R[rd] = PC + 4
		// PC = R[rs1] + imm
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = PC + 4; // shift by 20 bits
		PC = *idreg(ins[2]) + binstr2dec(ins[4]);
		
	} else if (ins[0] == "auipc"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = binstr2dec(ins[4]) << 20 + PC; // shift by 20 bits and add current PC
		PC = PC + 4;
		
	} else if (ins[0] == "lui"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = binstr2dec(ins[4]) << 20; // shift by 20 bits
		PC = PC + 4;
		
	} else if (ins[0] == "jal"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = binstr2dec(ins[4]) << 20; // shift by 20 bits
		PC = PC + 4;
		
	}
}