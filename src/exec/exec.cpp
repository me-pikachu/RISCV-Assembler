#include <bits/stdc++.h>

using namespace std;

static int x0 = 0;
static int x1 = 0;
static int x2 = 2147483616;
static int x3 = 268435456;
static int x4 = 0;
static int x5 = 0;
static int x6 = 1;
static int x7 = 0;
static int x8 = 0;
static int x9 = 0;
static int x10 = 1;
static int x11 = 2147483616;
static int x12 = 0;
static int x13 = 0;
static int x14 = 0;
static int x15 = 0;
static int x16 = 0;
static int x17 = 0;
static int x18 = 0;
static int x19 = 0;
static int x20 = 0;
static int x21 = 0;
static int x22 = 0;
static int x23 = 0;
static int x24 = 0;
static int x25 = 0;
static int x26 = 0;
static int x27 = 0;
static int x28 = 0;
static int x29 = 0;
static int x30 = 0;
static int x31 = 0;

static map<unsigned int, int> mod_mem; // all the memory having some non zero values
// modified_memory

string int2hex_8byte(int num){
	string hex = "";
	
	for (int i=0; i<8; i++){
		if (num % 16 == 0) hex += '0';
		else if (num % 16 == 1) hex += '1';
		else if (num % 16 == 2) hex += '2';
		else if (num % 16 == 3) hex += '3';
		else if (num % 16 == 4) hex += '4';
		else if (num % 16 == 5) hex += '5';
		else if (num % 16 == 6) hex += '6';
		else if (num % 16 == 7) hex += '7';
		else if (num % 16 == 8) hex += '8';
		else if (num % 16 == 9) hex += '9';
		else if (num % 16 == 10) hex += 'A';
		else if (num % 16 == 11) hex += 'B';
		else if (num % 16 == 12) hex += 'C';
		else if (num % 16 == 13) hex += 'D';
		else if (num % 16 == 14) hex += 'E';
		else if (num % 16 == 15) hex += 'F';		
		num = num / 16;
	}
	reverse(hex.begin(), hex.end());
	return hex;
}

string int2hex_2byte(int num){
	string hex = "";
	
	for (int i=0; i<2; i++){
		if (num % 16 == 0) hex += '0';
		else if (num % 16 == 1) hex += '1';
		else if (num % 16 == 2) hex += '2';
		else if (num % 16 == 3) hex += '3';
		else if (num % 16 == 4) hex += '4';
		else if (num % 16 == 5) hex += '5';
		else if (num % 16 == 6) hex += '6';
		else if (num % 16 == 7) hex += '7';
		else if (num % 16 == 8) hex += '8';
		else if (num % 16 == 9) hex += '9';
		else if (num % 16 == 10) hex += 'A';
		else if (num % 16 == 11) hex += 'B';
		else if (num % 16 == 12) hex += 'C';
		else if (num % 16 == 13) hex += 'D';
		else if (num % 16 == 14) hex += 'E';
		else if (num % 16 == 15) hex += 'F';		
		num = num / 16;
	}
	reverse(hex.begin(), hex.end());
	return hex;
}

std::map<unsigned int, int>::iterator bin_search_add(int address){
	// performs a search in the map for the address and return the corresponding iterator
	auto it = mod_mem.lower_bound(address);
    if (it != mod_mem.end() && it->first == address) {
        return it;
    }
    return mod_mem.end();
}

void bin_insert_add(int address, int val){
	// insert a value into the memory
	auto it = bin_search_add(address);
    if (it == mod_mem.end()) {
        mod_mem.insert(std::make_pair(address, val));
    } else {
        it->second = val;
    }
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
	// memory stage
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

int decstr2num(string numstr){
	// this function converts a string of a number to an integer
	int num = 0;
	for (int i=numstr.length(); i>=0; i--){
		num = num * 10 + (numstr[i] - '0') % 10; // numstr[i] - '0' converts the char to digit
	}
	
	return num;
}

long long decstr2longdec(string numstr){
	// this function converts a string of a number to an integer
	long long num = 0;
	for (int i=numstr.length(); i>=0; i--){
		num = num * 10 + (numstr[i] - '0') % 10; // numstr[i] - '0' converts the char to digit
	}
	
	return num;
}

int binstr2dec(string binstr){
	// this function converts a binary string of a number to an integer
	int num = 0;
	for (int i=0; i < binstr.length(); i++){
		num = num*2 + (binstr[i]-'0'); // numstr[i] - '0' converts the char to digit
	}
	if (num > pow(2, binstr.length()-1) - 1){
		// num > 2^31-1
		num = num - pow(2, binstr.length()); // num - 2^32
	}
	
	return num;
}

int logicalrightshift(int num, int n){
	// performs logical right shift
	if (num < 0){
		num = (num >> 1) + 0x80000000; // operator precedence is important as to first perform right shift and then add 
		if (n > 1) return num >> (n-1);
		else return num;
	} else {
		return num >> (n-1);
	}
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
// i format : lb, lh, lw, addi, ori, andi, xori, slti, sgti, slei, sgei, slli, srli, srai, jalr
// r format : add, sub, mul, div, rem, and, or, xor, slt, sgt, sle, sge	
// s format : sb, sh, sw
// sb format : beq, bne, blt, bgt, ble, bge
// u format : auipc, lui
// uj format : jal

int exec_ext_bin_inst(string* ins, int PC){
	// execute stage + memory stage + write_back stage
	// returns the new PC
	
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
	} else if (ins[0] == "sd"){
		// address is rs1 + immediate
		// value to store is in rs2
		std::cout << "sd command is not supported yet. Proceding with the corresponding sw instruction\n";
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
		
	} else if (ins[0] == "ld"){
		// address is rs1 + immediate
		std::cout << "ld command is not supported in 32bit RISCV. Proceding with the corresponding lw instruction\n";
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
	
	} else if (ins[0] == "slli"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) << binstr2dec(ins[4]);
		PC = PC + 4;
	
	} else if (ins[0] == "srai"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) >> binstr2dec(ins[4]);
		PC = PC + 4;
	
	} else if (ins[0] == "srli"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = logicalrightshift(*idreg(ins[2]), binstr2dec(ins[4]));
		PC = PC + 4;
	
	} else if (ins[0] == "jalr"){
		// the destination register cannot be x0
		// R[rd] = PC + 4
		// PC = R[rs1] + imm
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = PC + 4; // shift by 20 bits
		PC = *idreg(ins[2]) + binstr2dec(ins[4]);
		
	} else if (ins[0] == "auipc"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = binstr2dec(ins[4]) << 12 + PC; // shift by 12 bits and add current PC
		PC = PC + 4;
		
	} else if (ins[0] == "lui"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = binstr2dec(ins[4]) << 12; // shift by 12 bits
		PC = PC + 4;
		
	} else if (ins[0] == "jal"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = PC + 4; // shift by 20 bits
		PC = PC + binstr2dec(ins[4]);
		
	} else if (ins[0] == "beq"){
		if (idreg(ins[2]) == idreg(ins[3]))	PC = PC + binstr2dec(ins[4]);
		else PC = PC + 4;
		
	} else if (ins[0] == "bne"){
		if (idreg(ins[2]) != idreg(ins[3]))	PC = PC + binstr2dec(ins[4]);
		else PC = PC + 4;
		
	} else if (ins[0] == "blt"){
		if (idreg(ins[2]) < idreg(ins[3]))	PC = PC + binstr2dec(ins[4]);
		else PC = PC + 4;
		
	} else if (ins[0] == "bgt"){
		if (idreg(ins[2]) > idreg(ins[3]))	PC = PC + binstr2dec(ins[4]);
		else PC = PC + 4;
		
	} else if (ins[0] == "ble"){
		if (idreg(ins[2]) <= idreg(ins[3]))	PC = PC + binstr2dec(ins[4]);
		else PC = PC + 4;
		
	} else if (ins[0] == "bge"){
		if (idreg(ins[2]) >= idreg(ins[3]))	PC = PC + binstr2dec(ins[4]);
		else PC = PC + 4;
		
	} else if (ins[0] == "add"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) + *idreg(ins[3]);
		PC = PC + 4;
		
	} else if (ins[0] == "sub"){
		// the destination register cannot be x0
		// rs1 - rs2
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) - *idreg(ins[3]);
		PC = PC + 4;
		
	} else if (ins[0] == "mul"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) * *idreg(ins[3]);
		PC = PC + 4;
		
	} else if (ins[0] == "div"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = (int)(*idreg(ins[2]) / *idreg(ins[3]));
		PC = PC + 4;
		
	} else if (ins[0] == "rem"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) % *idreg(ins[3]);
		PC = PC + 4;
		
	} else if (ins[0] == "and"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) & *idreg(ins[3]);
		PC = PC + 4;
		
	} else if (ins[0] == "or"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) | *idreg(ins[3]);
		PC = PC + 4;
		
	} else if (ins[0] == "xor"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) ^ *idreg(ins[3]);
		PC = PC + 4;
		
	} else if (ins[0] == "slt"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) < *idreg(ins[3]) ? 1 : 0;
		PC = PC + 4;
		
	} else if (ins[0] == "sgt"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) > *idreg(ins[3]) ? 1 : 0;
		PC = PC + 4;
		
	} else if (ins[0] == "sle"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) <= *idreg(ins[3]) ? 1 : 0;
		PC = PC + 4;
		
	} else if (ins[0] == "sge"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) >= *idreg(ins[3]) ? 1 : 0;
		PC = PC + 4;
		
	} else if (ins[0] == "sll"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) << *idreg(ins[3]);
		PC = PC + 4;
	
	} else if (ins[0] == "sra"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = *idreg(ins[2]) >> *idreg(ins[3]);
		PC = PC + 4;
	
	} else if (ins[0] == "srl"){
		// the destination register cannot be x0
		if (idreg(ins[1]) != &x0) *idreg(ins[1]) = logicalrightshift(*idreg(ins[2]), *idreg(ins[3]));
		PC = PC + 4;
	
	}
	
	return PC;
}

int assembler_dir(string* asscmd){
	// runs the assembler directives
	// returns the starting address where data is stored
	// asscmd = {"command_name", val, ascii_str} // ascii_str is non empty only when .asciiz is used
	static int address = 0x10000000;
	if (asscmd[0] == "byte"){
		memory(address, 1, 0, decstr2num(asscmd[1]));
		address = address + 1;
	} else if (asscmd[0] == "half"){
		memory(address, 2, 0, decstr2num(asscmd[1]));
		address = address + 2;
	} else if (asscmd[0] == "word"){
		memory(address, 4, 0, decstr2num(asscmd[1]));
		address = address + 4;
	} else if (asscmd[0] == "dword"){
		memory(address, 8, 0, decstr2longdec(asscmd[1]));
		address = address + 8;
	} else if (asscmd[0] == "asciiz"){
		for (int i=0; i<asscmd[2].length(); i++){
			memory(address, 1, 0, asscmd[2][i]);
			address = address + 1;
		}
	} else {
		std::cout << "Invalid assemble directive\n";
	}
	return address;
}

// instruction supported : 
// i format : lb, lh, lw, ld, lbu, lhu, lwu, fence, fence.i, addi, addiw, andi, ori, xori, slli, srli, srai, slliw, srliw, sraiw, slti, sltiu, jalr, ecall, ebreak, CSRRW, CSRRS, CSRRC, CSRRWT, CSRRST, CSRRCT
// r format : add, addw, sub, subw, mul, div, rem, sll, sllw, srl, srlw, sra, sraw, and, or, xor, slt, sltu	
// s format : sb, sh, sw, sd
// sb format : beq, bne, blt, bgt, ble, bge
// u format : auipc, lui
// uj format : jal

string* ext_bin_inst(string bincmd){
	// decode stage
	
	string* ins = new string[6];
	ins[0] = ""; // command_name
	ins[1] = ""; // rd
	ins[2] = ""; // rs1
	ins[3] = ""; // rs2
	ins[4] = ""; // imm
	ins[5] = ""; // label_name;
	
	if (bincmd == "11111111111111111111111111111111"){
		// exit command
		return ins;
	}
	
	string opcode = "";
	opcode = opcode + bincmd[25] + bincmd[26] + bincmd[27] + bincmd[28] + bincmd[29] + bincmd[30] + bincmd[31];
	
	char insfmt = ' '; // b for sb format and j for uj format
	if (opcode == "0000011"){
		// lb lh lw ld lbu lhu lwu
		insfmt = 'i';
		string func3 = "";
		func3 = func3 + bincmd[17] + bincmd[18] + bincmd[19]; // func3
		
		if (func3 == "000") ins[0] = "lb";
		else if (func3 == "001") ins[0] = "lh";
		else if (func3 == "010") ins[0] = "lw";
		else if (func3 == "011") ins[0] = "ld";
		else if (func3 == "100") ins[0] = "lbu";
		else if (func3 == "101") ins[0] = "lhu";
		else if (func3 == "110") ins[0] = "lwu";
		
	} else if (opcode == "0001111"){
		// fence fence.i
		insfmt = 'i';
		string func3 = "";
		func3 = func3 + bincmd[17] + bincmd[18] + bincmd[19]; // func3
		
		if (func3 == "000") ins[0] = "fence";
		else if (func3 == "001") ins[0] = "fence.i";
		
	} else if (opcode == "0010011"){
		// addi andi ori xori slli srli srai slti sltiu
		insfmt = 'i';
		string func3 = "";
		func3 = func3 + bincmd[17] + bincmd[18] + bincmd[19]; // func3
		
		if (func3 == "000") ins[0] = "addi";
		else if (func3 == "111") ins[0] = "andi";
		else if (func3 == "110") ins[0] = "ori";
		else if (func3 == "100") ins[0] = "xori";
		else if (func3 == "001") ins[0] = "slli";
		else if (func3 == "101"){
			if (bincmd[1] == '0'){
				ins[0] = "srli";
			} else {
				ins[0] = "srai";
			}
		}
		else if (func3 == "010") ins[0] = "slti";
		else if (func3 == "011") ins[0] = "sltiu";
		
	} else if (opcode == "0011011"){
		// addiw slliw srliw sraiw
		insfmt = 'i';
		string func3 = "";
		func3 = func3 + bincmd[17] + bincmd[18] + bincmd[19]; // func3
		
		if (func3 == "000") ins[0] = "addiw";
		else if (func3 == "001") ins[0] = "slliw";
		else if (func3 == "101"){
			if (bincmd[1] == '0'){
				ins[0] = "srliw";
			} else {
				ins[0] = "sraiw";
			}
		}
		
	} else if (opcode == "1100111"){
		// jalr
		insfmt = 'i';
		ins[0] = "jalr";
		
	} else if (opcode == "1110011"){
		// ecall, ebreak, CSRRW, CSRRS, CSRRC, CSRRWT, CSRRST, CSRRCT
		insfmt = 'i';
		string func3 = "";
		func3 = func3 + bincmd[17] + bincmd[18] + bincmd[19]; // func3
		
		if (func3 == "000"){
			if (bincmd[11] == '0'){
				ins[0] = "ecall";
			} else {
				ins[0] = "ebreak";
			}
		} else if (func3 == "001") ins[0] = "CSRRW";
		else if (func3 == "010") ins[0] = "CSRRS";
		else if (func3 == "011") ins[0] = "CSRRC";
		else if (func3 == "101") ins[0] = "CSRRWT";
		else if (func3 == "110") ins[0] = "CSRRST";
		else if (func3 == "111") ins[0] = "CSRRCT";
		
	} else if (opcode == "0110011"){
		// add sub mul div rem sll srl sra and or xor slt sltu
		insfmt = 'r';
		string func3 = "";
		func3 = func3 + bincmd[17] + bincmd[18] + bincmd[19]; // func3
		
		if (func3 == "000"){
			if (bincmd[1] == '0'){
				if (bincmd[6] == '0'){
					ins[0] = "add";
				} else {
					ins[0] = "mul";
				}
			} else {
				ins[0] = "sub";
			}
		} else if (func3 == "100"){
			if (bincmd[6] == '0'){
				ins[0] = "xor";
			} else {
				ins[0] = "div"; // integer divide
			}
		} else if (func3 == "110"){
			if (bincmd[6] == '0'){
				ins[0] = "or";
			} else {
				ins[0] = "rem";
			}
		} else if (func3 == "001") ins[0] = "sll";
		  else if (func3 == "101"){
			if (bincmd[1] == '0'){
				ins[0] = "srl";
			} else {
				ins[0] = "sra";
			}
		} else if (func3 == "111") ins[0] = "and";
		  else if (func3 == "010") ins[0] = "slt";
		  else if (func3 == "011") ins[0] = "sltu";
		
	} else if (opcode == "0111011"){
		// addw subw sllw srlw sraw
		insfmt = 'r';
		string func3 = "";
		func3 = func3 + bincmd[17] + bincmd[18] + bincmd[19]; // func3
		
		if (func3 == "000"){
			if (bincmd[1] == '0'){
				ins[0] = "addw";
			} else {
				ins[0] = "subw";
			}
		}
		else if (func3 == "001") ins[0] = "sllw";
		else if (func3 == "101"){
			if (bincmd[1] == '0'){
				ins[0] = "srlw";
			} else {
				ins[0] = "sraw";
			}
		}
		
	} else if (opcode == "0110111"){
		// lui
		insfmt = 'u';
		ins[0] = "lui";
		
	} else if (opcode == "0010111"){
		// auipc
		insfmt = 'u';
		ins[0] = "auipc";
		
	} else if (opcode == "0100011"){
		// sb sh sw sd
		insfmt = 's';
		string func3 = "";
		func3 = func3 + bincmd[17] + bincmd[18] + bincmd[19]; // func3
		
		if (func3 == "000") ins[0] = "sb";
		else if (func3 == "001") ins[0] = "sh";
		else if (func3 == "010") ins[0] = "sw";
		else if (func3 == "011") ins[0] = "sd";
		
	} else if (opcode == "1101111"){
		// jal
		insfmt = 'j'; // uj format
		ins[0] = "jal";
		
	} else if (opcode == "1100011"){
		// beq bne blt bge bltu bgeu
		insfmt = 'b'; // sb format
		string func3 = "";
		func3 = func3 + bincmd[17] + bincmd[18] + bincmd[19]; // func3
		
		if (func3 == "000") ins[0] = "beq";
		else if (func3 == "001") ins[0] = "bne";
		else if (func3 == "100") ins[0] = "blt";
		else if (func3 == "101") ins[0] = "bge";
		else if (func3 == "110") ins[0] = "bltu";
		else if (func3 == "111") ins[0] = "bgeu";
		
	} else {
		std::cout << "Invalid binary operation\n";
	}
	
	if (insfmt == 'r'){
		ins[3] = ins[3] + bincmd[7] + bincmd[8] + bincmd[9] + bincmd[10] + bincmd[11]; // rs2
		ins[2] = ins[2] + bincmd[12] + bincmd[13] + bincmd[14] + bincmd[15] + bincmd[16]; // rs1
		ins[1] = ins[1] + bincmd[20] + bincmd[21] + bincmd[22] + bincmd[23] + bincmd[24]; // rd
		
	} else if (insfmt == 'i'){
		ins[4] = ins[4] + bincmd[0] + bincmd[1] + bincmd[2] + bincmd[3] + bincmd[4] + bincmd[5] + bincmd[6] + bincmd[7] + bincmd[8] + bincmd[9] + bincmd[10] + bincmd[11]; // immediate
		ins[2] = ins[2] + bincmd[12] + bincmd[13] + bincmd[14] + bincmd[15] + bincmd[16]; // rs1
		ins[1] = ins[1] + bincmd[20] + bincmd[21] + bincmd[22] + bincmd[23] + bincmd[24]; // rd
		
	} else if (insfmt == 's'){
		ins[4] = ins[4] + bincmd[0] + bincmd[1] + bincmd[2] + bincmd[3] + bincmd[4] + bincmd[5] + bincmd[6] + bincmd[20] + bincmd[21] + bincmd[22] + bincmd[23] + bincmd[24]; // immediate
		ins[3] = ins[3] + bincmd[7] + bincmd[8] + bincmd[9] + bincmd[10] + bincmd[11]; // rs2
		ins[2] = ins[2] + bincmd[12] + bincmd[13] + bincmd[14] + bincmd[15] + bincmd[16]; // rs1
		
	} else if (insfmt == 'u'){
		ins[4] = ins[4] + bincmd[0] + bincmd[1] + bincmd[2] + bincmd[3] + bincmd[4] + bincmd[5] + bincmd[6] + bincmd[7] + bincmd[8] + bincmd[9] + bincmd[10] + bincmd[11] + bincmd[12] + bincmd[13] + bincmd[14] + bincmd[15] + bincmd[16] + bincmd[17] + bincmd[18] + bincmd[19]; // immediate
		ins[1] = ins[1] + bincmd[20] + bincmd[21] + bincmd[22] + bincmd[23] + bincmd[24]; // rd
		
	} else if (insfmt == 'b'){
		// sb format
		ins[4] = ins[4] + bincmd[0] + bincmd[24] + bincmd[1] + bincmd[2] + bincmd[3] + bincmd[4] + bincmd[5] + bincmd[6] + bincmd[20] + bincmd[21] + bincmd[22] + bincmd[23] + '0'; // immediate ( + '0' as the 0th bit is always considered to be 0)
		ins[3] = ins[3] + bincmd[7] + bincmd[8] + bincmd[9] + bincmd[10] + bincmd[11]; // rs2
		ins[2] = ins[2] + bincmd[12] + bincmd[13] + bincmd[14] + bincmd[15] + bincmd[16]; // rs1
		
	} else if (insfmt == 'j'){
		// uj format
		ins[4] = ins[4] + bincmd[0] + bincmd[12] + bincmd[13] + bincmd[14] + bincmd[15] + bincmd[16] + bincmd[17] + bincmd[18] + bincmd[19] + bincmd[11] + bincmd[1] + bincmd[2] + bincmd[3] + bincmd[4] + bincmd[5] + bincmd[6] + bincmd[7] + bincmd[8] + bincmd[9] + bincmd[10] + '0'; // immediate ( + '0' as the 0th bit is always considered to be 0)
		ins[1] = ins[1] + bincmd[20] + bincmd[21] + bincmd[22] + bincmd[23] + bincmd[24]; // rd
	}
	
	return ins;
}

void exec(map<int, string> PCbincmd){
	// this command execute the code stored in PCbincmd
	static int PC = 0;
	auto it = PCbincmd.begin();
	
	if (PC > (PCbincmd.size()-1)*4 || PC < 0){
		// if PC is negative then break or if PC increase the map size PC counter then break (the binary code does not have exit code)
		if (PC < 0) std::cout << "Warning! The PC reached a -ve value\n";
		return;
	}
	
	int t = PC/4;
	while (t--) it++;
	
	string* ins = ext_bin_inst(it->second);
	
	if (ins[0] == ""){
		// empty no command
		return;
	} else{
		PC = exec_ext_bin_inst(ins, PC);
		exec(PCbincmd);
	}
}

void dis_mem(){
	std::cout << "x0 : 0x" << int2hex_8byte(x0) << "\nx1 : 0x" << int2hex_8byte(x1) << "\nx2 : 0x" << int2hex_8byte(x2) << "\nx3 : 0x" << int2hex_8byte(x3) << "\nx4 : 0x" << int2hex_8byte(x4) << "\nx5 : 0x" << int2hex_8byte(x5) << "\nx6 : 0x" << int2hex_8byte(x6) << "\nx7 : 0x" << int2hex_8byte(x7) << "\nx8 : 0x" << int2hex_8byte(x8) << "\nx9 : 0x" << int2hex_8byte(x9) << "\nx10 : 0x" << int2hex_8byte(x10) << "\nx11 : 0x" << int2hex_8byte(x11) << "\nx12 : 0x" << int2hex_8byte(x12) << "\nx13 : 0x" << int2hex_8byte(x13) << "\nx14 : 0x" << int2hex_8byte(x14) << "\nx15 : 0x" << int2hex_8byte(x15) << "\nx16 : 0x" << int2hex_8byte(x16) << "\nx17 : 0x" << int2hex_8byte(x17) << "\nx18 : 0x" << int2hex_8byte(x18) << "\nx19 : 0x" << int2hex_8byte(x19) << "\nx20 : 0x" << int2hex_8byte(x20) << "\nx21 : 0x" << int2hex_8byte(x21) << "\nx22 : 0x" << int2hex_8byte(x22) << "\nx23 : 0x" << int2hex_8byte(x23) << "\nx24 : 0x" << int2hex_8byte(x24) << "\nx25 : 0x" << int2hex_8byte(x25) << "\nx26 : 0x" << int2hex_8byte(x26) << "\nx27 : 0x" << int2hex_8byte(x27) << "\nx28 : 0x" << int2hex_8byte(x28) << "\nx29 : 0x" << int2hex_8byte(x29) << "\nx30 : 0x" << int2hex_8byte(x30) << "\nx31 : 0x" << int2hex_8byte(x31);
	if (mod_mem.size() != 0) std::cout << "\n           +3 +2 +1 +0" << endl;
	int prev_add = -8; // used to print " .\n .\n .\n"
	int address;
	for (auto rev_it = mod_mem.rbegin(); rev_it != mod_mem.rend(); ++rev_it) {
		if (rev_it->first % 4 == 0){
			address = rev_it->first;
		} else if ((rev_it->first-1) % 4 == 0){
			address = rev_it->first - 1;
		} else if ((rev_it->first-2) % 4 == 0){
			address = rev_it->first - 2;
		} else if ((rev_it->first-3) % 4 == 0){
			address = rev_it->first - 3;
		} else {
			std::cout << "Warning! Memory display error\n";
			address = rev_it->first;
		}
		
		if (prev_add == address) continue;
		if (prev_add != -8 && (prev_add - address) != 4) std::cout << " .\n .\n .\n";
		prev_add = address;
		
		cout << "0x" << int2hex_8byte(address) << " " << int2hex_2byte(memoryread(address + 3)) << " " << int2hex_2byte(memoryread(address + 2)) << " " << int2hex_2byte(memoryread(address + 1)) << " " << int2hex_2byte(memoryread(address)) << endl;
	}
}

