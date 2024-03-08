#include <bits/stdc++.h>
#include "../syntax/syntax_analyser.hpp"

using namespace std;

long long PC = 0; // setting the Program Counter initially to 0

map<string, int> labelPC; // linking all the labels with their corresponding PC
map<int, string> PCbincmd; // all the binary commands with their corresponding PC
vector<int> unfinished_cmd; // storing all the unfinished commands

int decstr2dec(string numstr){
	// this function converts a string of a number to an integer
	int num = 0;
	for (int i=numstr.length(); i>=0; i--){
		num = num*10 + (numstr[i]-'0')%10; // numstr[i] - '0' converts the char to digit
	}
	return num;
}

bool* Dec2Bin(long long num, size_t size){
	bool* array = (bool*)malloc(size*sizeof(bool));
	
	return array;
}

bool generateBinCmd(){
	string* cmd = getcmd();
	// cmd = {"command_name","rd","rs1","rs2","immediate","label_name"}
	string bincmd = "";
	
	if (cmd[0] == "" && cmd[1] == "" && cmd[2] == "" && cmd[3] == "" && cmd[4] == "" && cmd[5] == "") return 0; // to stop iterating the function
	
	if (cmd[5] != "" && cmd[0] != "labelread"){
		long long PCoflabel = -1; // initially not found
		// this is a branch or a jal command
		// if the label_name exists in labelPC then we can assign the offset else we need to add this to unfinished_cmd
		for(auto i = labelPC.begin(); i != labelPC.end(); ++i){
			if (i->first == cmd[5]){
				// we have the PC of the label
				PCoflabel = i->second;
				break;
			}
		}
		
		if (PCoflabel >= 0){
			// we found the PC for the label
			long long offset = PCoflabel - PC;
			if (cmd[0] == "jal"){
				char digchar;
				bool* binOffset = Dec2Bin(offset, 21); // jal has a 21 bit offset field
				
				// 20|10:1|11|19:12
				digchar = '0' + binOffset[20];
				bincmd = bincmd + digchar;
				for (int j=10; j>=1; j--){
					digchar = '0' + binOffset[j];
					bincmd = bincmd + digchar;
				}
				digchar = '0' + binOffset[11];
				bincmd = bincmd + digchar;
				for (int j=19; j>=12; j--){
					digchar = '0' + binOffset[j];
					bincmd = bincmd + digchar;
				}
				free(binOffset);
				// free up the space used by the array
				
				// now we have the rd field
				bincmd = bincmd + cmd[1];
				// now we have the opcode of jal
				bincmd = bincmd + "1101111";
				
			} else {
				char digchar;
				bool* binOffset = Dec2Bin(offset, 13); // branch have 13 bit offset field
				
				// 12|10:5
				digchar = '0' + binOffset[12];
				bincmd = bincmd + digchar;
				for (int j=10; j>=5; j--){
					digchar = '0' + binOffset[j];
					bincmd = bincmd + digchar;
				}
				
				// now we have the rs2 field
				bincmd = bincmd + cmd[3];
				// now we have the rs1 field
				bincmd = bincmd + cmd[2];
				// now we have func3
				if (cmd[0] == "beq") bincmd = bincmd + "000";
				else if (cmd[0] == "bne") bincmd = bincmd + "001";
				else if (cmd[0] == "blt") bincmd = bincmd + "100";
				else if (cmd[0] == "bge") bincmd = bincmd + "101";
				else if (cmd[0] == "bltu") bincmd = bincmd + "110";
				else if (cmd[0] == "bgeu") bincmd = bincmd + "111";
				else {
					std::cout << "Cannot identify the type of branch statement\n";
					bincmd = bincmd + "000";
				}
				
				// 4:1|11
				for (int j=4; j>=1; j--){
					digchar = '0' + binOffset[j];
					bincmd = bincmd + digchar;
				}
				digchar = '0' + binOffset[11];
				bincmd = bincmd + digchar;
				free(binOffset);
				// free up the space used by the array
				
				// now we have opcode of conditional branch
				bincmd = bincmd + "1100011";
			}
			
			PCbincmd.insert(pair<int, string>(PC, bincmd));
			PC = PC + 4;
			
		} else {
			// add the current command to unfinished commands
			unfinished_cmd.push_back(PC);
			string uf_cmd = cmd[0] + "," + cmd[1] + "," + cmd[2] + "," + cmd[3] + "," + cmd[4] + "," + cmd[5];
			PCbincmd.insert(pair<int, string>(PC, uf_cmd)); // uf_cmd is of the format all the cmd fields combined with ',' as delimiter
			
		}
		return 1; // 1 means to keep on iterating the generateBinCmd
		
	} else if (cmd[5] != "" && cmd[0] == "labelread"){
		labelPC.insert(pair<string, int>(cmd[5], PC));
		return 1; // 1 means to keep on iterating the generateBinCmd
		
	} else {
		string func7;
		string func3;
		string opcode;
		bool insfmt;
		
		// instruction supported : 
		// i format : lb, lh, lw, ld, lbu, lhu, lwu, fence, fence.i, addi, addiw, andi, ori, xori, slli, srli, srai, slliw, srliw, sraiw, slti, sltiu, sgti, sgtiu, slei, sleiu, sgei, sgeiu, jalr
		// r format : add, sub, mul, div, rem, sll, srl, sra, and, or, xor, slt, sltu, sgt, sgtu, sle, sleu, sge, sgeu	
		// s format : sb, sh, sw, sd
		// sb format : beq, bne, blt, bgt, ble, bge (above)
		// u format : auipc, lui
		// uj format : jal (above)
		
		if (cmd[0] == "add"){
			insfmt = 'r';
			opcode = "0110011";
			func3 = "000";
			func7 = "0000000";
			
		} else if (cmd[0] == "sub"){
			insfmt = 'r';
			opcode = "0110011";
			func3 = "000";
			func7 = "0100000";
			
		} else if (cmd[0] == "mul"){
			insfmt = 'r';
			opcode = "";
			func3 = "";
			func7 = "";
			
		} else if (cmd[0] == "div"){
			insfmt = 'r';
			opcode = "";
			func3 = "";
			func7 = "";
			
		} else if (cmd[0] == "rem"){
			insfmt = 'r';
			opcode = "";
			func3 = "";
			func7 = "";
			
		} else if (cmd[0] == "sll"){
			insfmt = 'r';
			opcode = "0110011";
			func3 = "001";
			func7 = "0000000";
			
		} else if (cmd[0] == "srl"){
			insfmt = 'r';
			opcode = "0110011";
			func3 = "101";
			func7 = "0000000";
		} else if (cmd[0] == "sra"){
			insfmt = 'r';
			opcode = "0110011";
			func3 = "101";
			func7 = "0100000";
			
		 else if (cmd[0] == "and"){
			insfmt = 'r';
			opcode = "0110011";
			func3 = "111";
			func7 = "0000000";
			
		} else if (cmd[0] == "or"){
			insfmt = 'r';
			opcode = "0110011";
			func3 = "110";
			func7 = "0000000";
			
		} else if (cmd[0] == "xor"){
			insfmt = 'r';
			opcode = "0110011";
			func3 = "100";
			func7 = "0000000";
			
		} } else if (cmd[0] == "slt"){
			insfmt = 'r';
			opcode = "0110011";
			func3 = "010";
			func7 = "0000000";
			
		} else if (cmd[0] == "sltu"){
			insfmt = 'r';
			opcode = "0110011";
			func3 = "011";
			func7 = "0000000";
			
		} else if (cmd[0] == "sgt"){
			insfmt = 'r';
			opcode = "";
			func3 = "";
			func7 = "";
			
		} else if (cmd[0] == "sgtu"){
			insfmt = 'r';
			opcode = "";
			func3 = "";
			func7 = "";
			
		} else if (cmd[0] == "sle"){
			insfmt = 'r';
			opcode = "";
			func3 = "";
			func7 = "";
			
		} else if (cmd[0] == "sleu"){
			insfmt = 'r';
			opcode = "";
			func3 = "";
			func7 = "";
			
		} else if (cmd[0] == "sge"){
			insfmt = 'r';
			opcode = "";
			func3 = "";
			func7 = "";
			
		} else if (cmd[0] == "sgeu"){
			insfmt = 'r';
			opcode = "";
			func3 = "";
			func7 = "";
			
		} else if (cmd[0] == "sb"){
			insfmt = 's';
			opcode = "0100011";
			func3 = "000";
			func7 = "";
			
		} else if (cmd[0] == "sh"){
			insfmt = 's';
			opcode = "0100011";
			func3 = "001";
			func7 = "";
			
		} else if (cmd[0] == "sw"){
			insfmt = 's';
			opcode = "0100011";
			func3 = "010";
			func7 = "";
			
		} else if (cmd[0] == "sd"){
			insfmt = 's';
			opcode = "0100011";
			func3 = "011";
			func7 = "";
			
		} else if (cmd[0] == "lb"){
			insfmt = 'i';
			opcode = "0000011";
			func3 = "000";
			func7 = "";
			
		} else if (cmd[0] == "lh"){
			insfmt = 'i';
			opcode = "0000011";
			func3 = "001";
			func7 = "";
			
		} else if (cmd[0] == "lw"){
			insfmt = 'i';
			opcode = "0000011";
			func3 = "010";
			func7 = "";
			
		} else if (cmd[0] == "ld"){
			insfmt = 'i';
			opcode = "0000011";
			func3 = "011";
			func7 = "";
			
		} else if (cmd[0] == "lbu"){
			insfmt = 'i';
			opcode = "0000011";
			func3 = "100";
			func7 = "";
			
		} else if (cmd[0] == "lhu"){
			insfmt = 'i';
			opcode = "0000011";
			func3 = "101";
			func7 = "";
			
		} else if (cmd[0] == "lwu"){
			insfmt = 'i';
			opcode = "0000011";
			func3 = "110";
			func7 = "";
			
		} else if (cmd[0] == "fence"){
			insfmt = 'i';
			opcode = "0001111";
			func3 = "000";
			func7 = "";
			
		} else if (cmd[0] == "fence.i"){
			insfmt = 'i';
			opcode = "0001111";
			func3 = "001";
			func7 = "";
			
		} else if (cmd[0] == "addi"){
			insfmt = 'i';
			opcode = "0010011";
			func3 = "000";
			func7 = "";
			
		} else if (cmd[0] == "addiw"){
			insfmt = 'i';
			opcode = "0011011";
			func3 = "000";
			func7 = "";
			
		} else if (cmd[0] == "andi"){
			insfmt = 'r';
			opcode = "0010011";
			func3 = "111";
			func7 = "";
			
		} else if (cmd[0] == "ori"){
			insfmt = 'i';
			opcode = "0010011";
			func3 = "110";
			func7 = "";
			
		} else if (cmd[0] == "xori"){
			insfmt = 'i';
			opcode = "0010011";
			func3 = "100";
			func7 = "";
			
		} else if (cmd[0] == "slli"){
			insfmt = 'i';
			opcode = "0010011";
			func3 = "001";
			func7 = "0000000";
			
		} else if (cmd[0] == "srli"){
			insfmt = 'i';
			opcode = "0010011";
			func3 = "101";
			func7 = "0000000";
			
		} else if (cmd[0] == "srai"){
			insfmt = 'i';
			opcode = "0010011";
			func3 = "101";
			func7 = "0100000";
			
		} else if (cmd[0] == "slliw"){
			insfmt = 'i';
			opcode = "0011011";
			func3 = "001";
			func7 = "0000000";
			
		} else if (cmd[0] == "srliw"){
			insfmt = 'i';
			opcode = "0011011";
			func3 = "101";
			func7 = "0000000";
			
		} else if (cmd[0] == "sraiw"){
			insfmt = 'r';
			opcode = "0011011";
			func3 = "101";
			func7 = "0100000";
			
		} else if (cmd[0] == "slti"){
			insfmt = 'i';
			opcode = "0010011";
			func3 = "010";
			func7 = "";
			
		} else if (cmd[0] == "sltiu"){
			insfmt = 'i';
			opcode = "0010011";
			func3 = "011";
			func7 = "";
			
		} else if (cmd[0] == "sgti"){
			insfmt = 'i';
			opcode = "";
			func3 = "";
			func7 = "";
			
		} else if (cmd[0] == "sgtiu"){
			insfmt = 'i';
			opcode = "";
			func3 = "";
			func7 = "";
			
		} else if (cmd[0] == "slei"){
			insfmt = 'i';
			opcode = "";
			func3 = "";
			func7 = "";
			
		} else if (cmd[0] == "sleiu"){
			insfmt = 'i';
			opcode = "";
			func3 = "";
			func7 = "";
			
		} else if (cmd[0] == "sgei"){
			insfmt = 'i';
			opcode = "";
			func3 = "";
			func7 = "";
			
		} else if (cmd[0] == "sgeiu"){
			insfmt = 'i';
			opcode = "";
			func3 = "";
			func7 = "";
			
		} else if (cmd[0] == "jalr"){
			insfmt = 'i';
			opcode = "1100111";
			func3 = "000";
			func7 = "";
			
		} else if (cmd[0] == "ecall"){
			insfmt = 'i';
			opcode = "1110011";
			func3 = "000";
			func7 = "000000000000";
			
		} else if (cmd[0] == "ebreak"){
			insfmt = 'i';
			opcode = "1110011";
			func3 = "000";
			func7 = "000000000001";
			
		} else if (cmd[0] == "CSRRW"){
			insfmt = 'i';
			opcode = "1110011";
			func3 = "001";
			func7 = "";
			
		} else if (cmd[0] == "CSRRS"){
			insfmt = 'i';
			opcode = "1110011";
			func3 = "010";
			func7 = "";
			
		} else if (cmd[0] == "CSRRC"){
			insfmt = 'i';
			opcode = "1110011";
			func3 = "011";
			func7 = "";
			
		} else if (cmd[0] == "CSRRWT"){
			insfmt = 'i';
			opcode = "1110011";
			func3 = "101";
			func7 = "";
			
		} else if (cmd[0] == "CSRRST"){
			insfmt = 'i';
			opcode = "1110011";
			func3 = "110";
			func7 = "";
			
		} else if (cmd[0] == "CSRRCT"){
			insfmt = 'i';
			opcode = "1110011";
			func3 = "111";
			func7 = "";
			
		} else if (cmd[0] == "lui"){
			insfmt = 'u';
			opcode = "0110111";
			func3 = "";
			func7 = "";
			
		} else if (cmd[0] == "auipc"){
			insfmt = 'u';
			opcode = "0010111";
			func3 = "";
			func7 = "";
			
		} else {
			std::cout << "Cannot identify the type of the instruction\n";
		}
		
		if (insfmt == 'r'){
			// func7 rs2 rs1 func3 rd opcode
			bincmd = func7 + cmd[3] + cmd[2] + func3 + cmd[1] + opcode;
			
		} else if (insfmt == 'i'){
			if (func7.length() == 7){
				// we already have 7 bits of immediate
				long long num = decstr2dec(cmd[4]);
				if (num < 0){
					std::cout << "The value of immediate should be between 0 to 31\n";
				} else {
					bincmd = bincmd + func7;
					bool* binimd = Dec2Bin(num, 5); // immediate is of 5 bits
					// adding the 12 bits of immediate
					char digchar;
					for (int j=4; j>=0; j--){
						digchar = '0' + binimd[j];
						bincmd = bincmd + digchar;
					}
					
					bincmd = bincmd + cmd[2] + func3 + cmd[1] + opcode; // rs1 func3 rd opcode
				}
			} else if (func7.length() == 12){
				
				bincmd = func7 + bincmd + cmd[2] + func3 + cmd[1] + opcode; // imm[11:0] rs1 func3 rd opcode
				
			} else {
				
				bool* binimd = Dec2Bin(decstr2dec(cmd[4]), 12); // immediate is of 12 bits
			
				// adding the 12 bits of immediate
				char digchar;
				for (int j=11; j>=0; j--){
					digchar = '0' + binimd[j];
					bincmd = bincmd + digchar;
				}
				
				bincmd = bincmd + cmd[2] + func3 + cmd[1] + opcode; // rs1 func3 rd opcode
			}
			
			
		} else if (insfmt == 's'){
			bool* binimd = Dec2Bin(decstr2dec(cmd[4]), 12); // immediate is of 12 bits
			
			// imd[11:5]
			char digchar;
			for (int j=11; j>=5; j--){
				digchar = '0' + binimd[j];
				bincmd = bincmd + digchar;
			}
			
			bincmd = bincmd + cmd[3] + cmd[2] + func3;
			
			// imm[4:0]
			for (int j=4; j>=0; j--){
				digchar = '0' + binimd[j];
				bincmd = bincmd + digchar;
			}
			
			bincmd = bincmd + opcode; // opcode
			
		} else if (insfmt == 'u'){
			bool* binimd = Dec2Bin(decstr2dec(cmd[4]), 20); // immediate is of 20 bits
			
			// imm[31:12] all the lower bits would be 0
			char digchar;
			for (int j=19; j>=0; j--){
				digchar = '0' + binimd[j];
				bincmd = bincmd + digchar;
			}
			
			// rd opcode
			bincmd = bincmd + cmd[1] + opcode;
			
		} else {
			std::cout << "Cannot identify the type of the instruction\n";
		}
		
		PCbincmd.insert(pair<int, string>(PC, bincmd));
		PC = PC + 4;
		return 1;
	}
}

void update_unfinished_cmd(){
	
	// iterate over every unfinished command
	for (int i : unfinished_cmd){
		string cmd;
		string bincmd = "";
		for (auto it = PCbincmd.begin(); it != PCbincmd.end(); ++it){
			if (it->first == i){
				cmd = it->second;
			}
		}
		
		string cmd_name = "";
		int j = 0;
		for (j=0; j<cmd.length(); j++){
			if (cmd[j] == ',') break;
			cmd_name = cmd_name + cmd[j];
		}
		
		string rd = "";
		for (j=j+1; j<cmd.length(); j++){
			if (cmd[j] == ',') break;
			rd = rd + cmd[j];
		}
		
		string rs1 = "";
		for (j=j+1; j<cmd.length(); j++){
			if (cmd[j] == ',') break;
			rs1 = rs1 + cmd[j];
		}
		
		string rs2 = "";
		for (j=j+1; j<cmd.length(); j++){
			if (cmd[j] == ',') break;
			rs2 = rs2 + cmd[j];
		}
		
		string imm = "";
		for (j=j+1; j<cmd.length(); j++){
			if (cmd[j] == ',') break;
			imm = imm + cmd[j];
		}
		
		string label_name = "";
		for (j=j+1; j<cmd.length(); j++){
			label_name = label_name + cmd[j];
		}
		
		int offset = 0;
		for(auto j = labelPC.begin(); j != labelPC.end(); ++j){
			if (j->first == label_name){
				// we have the PC of the label
				offset = j->second - i; // i is the current PC
				break;
			}
		}
		
		
		
		
		// now cmd_name is having the command_name
		if (cmd_name == "jal"){
			bool* binoffset = Dec2Bin(offset, 21); // 21 bit immediate
			char digchar;
			
			// imm[20]
			digchar = '0' + binoffset[20];
			bincmd = bincmd + digchar;
			
			// imm[10:1]
			for (int k=10; k>=1; k--){
				digchar = '0' + binoffset[k];
				bincmd = bincmd + digchar;
			}
			
			// imm[11]
			digchar = '0' + binoffset[11];
			bincmd = bincmd + digchar;
			
			// imm[19:12]
			for (int k=19; k>=12; k--){
				digchar = '0' + binoffset[k];
				bincmd = bincmd + digchar;
			}
			
			// rd + opcode
			bincmd = bincmd + rd + "1101111";
			
		} else {
			bool* binoffset = Dec2Bin(offset, 13); // 13 bit immediate
			
			// imm[12]
			char digchar;
			digchar = '0' + binoffset[12];
			bincmd = bincmd + digchar;
			
			// imm[10:5]
			for (int k=10; k>=5; k--){
				digchar = '0' + binoffset[k];
				bincmd = bincmd + digchar;
			}
			
			// rs2 rs1
			bincmd = bincmd + rs2 + rs1;
			
			// func3
			if (cmd_name == "beq") bincmd = bincmd + "000";
			else if (cmd_name == "bne") bincmd = bincmd + "001";
			else if (cmd_name == "blt") bincmd = bincmd + "100";
			else if (cmd_name == "bge") bincmd = bincmd + "101";
			else if (cmd_name == "bltu") bincmd = bincmd + "110";
			else if (cmd_name == "bgeu") bincmd = bincmd + "111";
			else {
				std::cout << "Cannot identify the type of branch statement\n";
				bincmd = bincmd + "000";
			}
			
			// imm[4:1]
			for (int k=4; k>=1; k--){
				digchar = '0' + binoffset[k];
				bincmd = bincmd + digchar;
			}
			
			// imm[11]
			digchar = '0' + binoffset[11];
			bincmd = bincmd + digchar;
			
			bincmd = bincmd + "1100011";
		}
	}
}

void getBinCmd(){
	while(generateBinCmd()){}
	update_unfinished_cmd();
}

