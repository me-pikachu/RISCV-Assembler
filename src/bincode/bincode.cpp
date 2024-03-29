#include <bits/stdc++.h>
#include "../syntax/syntax_analyser.cpp"

using namespace std;

int curPC = 0; // setting the Program Counter initially to 0

map<string, int> labelPC; // linking all the labels with their corresponding PC
map<int, string> PCbincmd; // all the binary commands with their corresponding PC
vector<int> unfinished_cmd; // storing all the unfinished commands

int decstr2dec(string numstr){
	// this function converts a string of a number to an integer
	int num = 0;
	if (numstr[0] == '-'){
		// the number is negative
		for (int i=1; i < numstr.length(); i++){
			num = num * 10 + (numstr[i] - '0') % 10; // numstr[i] - '0' converts the char to digit
		}
		num = -1*num;
	} else {
		for (int i=0; i < numstr.length(); i++){
			num = num * 10 + (numstr[i] - '0') % 10; // numstr[i] - '0' converts the char to digit
		}
	}	
	return num;
}

bool* Dec2Bin(int num, size_t size){
	bool* array = (bool*)malloc(size*sizeof(bool));
	
	if (num >= -1*pow(2, size-1) && num <= pow(2, size-1) - 1){
		if (num < 0){
			num = pow(2, size) + num;
		}
		
		for (int i=0; i<size; i++){
			array[i] = num % 2;
			num = num / 2;
		}		
	} else {
		std::cout << "Immediate out of range\n"; 
		exit(1);
	}
	
	return array;
}

bool generateBinCmd(string file_path){
	string* cmd = getcmd(file_path);
	// cmd = {"command_name","rd","rs1","rs2","immediate","label_name"}
	string bincmd = "";
	if (cmd[0] == "" && cmd[1] == "" && cmd[2] == "" && cmd[3] == "" && cmd[4] == "" && cmd[5] == ""){
		// to stop iterating the function and add the exit instruction
		// exit instruction in our case is all 1's
		PCbincmd.insert(pair<int, string>(curPC, "11111111111111111111111111111111"));
		curPC = curPC + 4;
		return 0;
	}
	
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
			long long offset = PCoflabel - curPC;
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
				
				if (cmd[0] == "bgt" || cmd[0] == "ble" || cmd[0] == "bgtu" || cmd[0] == "bleu"){
					// interchange rs1 and rs2
					bincmd = bincmd + cmd[2] + cmd[3];
				} else{
					// now we have the rs2 and rs1 field
					bincmd = bincmd + cmd[3] + cmd[2];
				}
				
				// now we have func3
				if (cmd[0] == "beq") bincmd = bincmd + "000";
				else if (cmd[0] == "bne") bincmd = bincmd + "001";
				else if (cmd[0] == "blt" || cmd[0] == "bgt") bincmd = bincmd + "100";
				else if (cmd[0] == "bge" || cmd[0] == "ble") bincmd = bincmd + "101";
				else if (cmd[0] == "bltu" || cmd[0] == "bgtu") bincmd = bincmd + "110";
				else if (cmd[0] == "bgeu" || cmd[0] == "bleu") bincmd = bincmd + "111";
				else {
					std::cout << "Branch statement opcode cannot be identified\n";
					exit(1);
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
			
			PCbincmd.insert(pair<int, string>(curPC, bincmd));
			curPC = curPC + 4;
			
		} else {
			// add the current command to unfinished commands
			unfinished_cmd.push_back(curPC);
			string uf_cmd = cmd[0] + "," + cmd[1] + "," + cmd[2] + "," + cmd[3] + "," + cmd[4] + "," + cmd[5];
			PCbincmd.insert(pair<int, string>(curPC, uf_cmd)); // uf_cmd is of the format all the cmd fields combined with ',' as delimiter
			curPC = curPC + 4;
		}
		return 1; // 1 means to keep on iterating the generateBinCmd
		
	} else if (cmd[5] != "" && cmd[0] == "labelread"){
		labelPC.insert(pair<string, int>(cmd[5], curPC));
		return 1; // 1 means to keep on iterating the generateBinCmd
		
	} else {
		string func7;
		string func3;
		string opcode;
		char insfmt = ' ';
		
		// instruction supported : 
		// i format : lb, lh, lw, ld, lbu, lhu, lwu, fence, fence.i, addi, addiw, andi, ori, xori, slli, srli, srai, slliw, srliw, sraiw, slti, sltiu, jalr, ecall, ebreak, CSRRW, CSRRS, CSRRC, CSRRWT, CSRRST, CSRRCT
		// r format : add, addw, sub, subw, mul, div, rem, sll, sllw, srl, srlw, sra, sraw, and, or, xor, slt, sltu, sgt, sgtu, sle, sleu, sge, sgeu	
		// s format : sb, sh, sw, sd
		// sb format : beq, bne, blt, bgt, ble, bge, bltu, bgtu, bleu, bgeu (above)
		// u format : auipc, lui
		// uj format : jal (above)
		
		if (cmd[0] == "add"){
			insfmt = 'r';
			opcode = "0110011";
			func3 = "000";
			func7 = "0000000";
			
		} else if (cmd[0] == "addw"){
			insfmt = 'r';
			opcode = "0111011";
			func3 = "000";
			func7 = "0000000";
			
		} else if (cmd[0] == "sub"){
			insfmt = 'r';
			opcode = "0110011";
			func3 = "000";
			func7 = "0100000";
			
		} else if (cmd[0] == "subw"){
			insfmt = 'r';
			opcode = "0111011";
			func3 = "000";
			func7 = "0100000";
			
		} else if (cmd[0] == "mul"){
			insfmt = 'r';
			opcode = "0110011";
			func3 = "000";
			func7 = "0000001";
			
		} else if (cmd[0] == "div"){
			insfmt = 'r';
			opcode = "0110011";
			func3 = "100";
			func7 = "0000001";
			
		} else if (cmd[0] == "rem"){
			insfmt = 'r';
			opcode = "0110011";
			func3 = "110";
			func7 = "0000001";
			
		} else if (cmd[0] == "sll"){
			insfmt = 'r';
			opcode = "0110011";
			func3 = "001";
			func7 = "0000000";
			
		} else if (cmd[0] == "sllw"){
			insfmt = 'r';
			opcode = "0111011";
			func3 = "001";
			func7 = "0000000";
			
		} else if (cmd[0] == "srl"){
			insfmt = 'r';
			opcode = "0110011";
			func3 = "101";
			func7 = "0000000";
			
		} else if (cmd[0] == "srlw"){
			insfmt = 'r';
			opcode = "0111011";
			func3 = "101";
			func7 = "0000000";
			
		} else if (cmd[0] == "sra"){
			insfmt = 'r';
			opcode = "0110011";
			func3 = "101";
			func7 = "0100000";
			
		} else if (cmd[0] == "sraw"){
			insfmt = 'r';
			opcode = "0111011";
			func3 = "101";
			func7 = "0100000";
			
		} else if (cmd[0] == "and"){
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
			
		} else if (cmd[0] == "slt"){
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
			// same as slt but rs1 and rs2 get interchanged
			insfmt = 'r';
			opcode = "0110011";
			func3 = "010";
			func7 = "0000000";
			string temp = cmd[2];
			cmd[2] = cmd[3];
			cmd[3] = temp;
			
		} else if (cmd[0] == "sgtu"){
			// same as sltu but rs1 and rs2 get interchanged
			insfmt = 'r';
			opcode = "0110011";
			func3 = "011";
			func7 = "0000000";
			string temp = cmd[2];
			cmd[2] = cmd[3];
			cmd[3] = temp;
			
		} else if (cmd[0] == "sle"){
			// slt rd, rs2, rs1
			// xori rd, rd, 1
			/*
			insfmt = 'r';
			opcode = "0110011";
			func3 = "010";
			func7 = "0000000";
			*/
			// slt rd, rs2, rs1
			bincmd = "0000000" + cmd[2] + cmd[3] + "010" + cmd[1] + "0110011";
			PCbincmd.insert(pair<int, string>(curPC, bincmd));
			curPC = curPC + 4;
			bincmd = "";
			
			// xori rd, rd, 1
			cmd[0] = "xori";
			cmd[2] = cmd[1]; // rd
			cmd[4] = "1";
			
			insfmt = 'i';
			opcode = "0010011";
			func3 = "100";
			func7 = "";
			
		} else if (cmd[0] == "sge"){
			// slt rd, rs1, rs2
			// xori rd, rd, 1
			/*
			insfmt = 'r';
			opcode = "0110011";
			func3 = "010";
			func7 = "0000000";
			*/
			// slt rd, rs1, rs2
			bincmd = "0000000" + cmd[3] + cmd[2] + "010" + cmd[1] + "0110011";
			PCbincmd.insert(pair<int, string>(curPC, bincmd));
			curPC = curPC + 4;
			bincmd = "";
			
			// xori rd, rd, 1
			cmd[0] = "xori";
			cmd[2] = cmd[1]; // rd
			cmd[4] = "1";
			
			insfmt = 'i';
			opcode = "0010011";
			func3 = "100";
			func7 = "";
			
		} else if (cmd[0] == "sleu"){
			// sltu rd, rs2, rs1
			// xori rd, rd, 1
			/*
			insfmt = 'r';
			opcode = "0110011";
			func3 = "011";
			func7 = "0000000";
			*/
			// slt rd, rs2, rs1
			bincmd = "0000000" + cmd[2] + cmd[3] + "011" + cmd[1] + "0110011";
			PCbincmd.insert(pair<int, string>(curPC, bincmd));
			curPC = curPC + 4;
			bincmd = "";
			
			// xori rd, rd, 1
			cmd[0] = "xori";
			cmd[2] = cmd[1]; // rd
			cmd[4] = "1";
			
			insfmt = 'i';
			opcode = "0010011";
			func3 = "100";
			func7 = "";
			
		} else if (cmd[0] == "sgeu"){
			// sltu rd, rs1, rs2
			// xori rd, rd, 1
			/*
			insfmt = 'r';
			opcode = "0110011";
			func3 = "011";
			func7 = "0000000";
			*/
			// slt rd, rs1, rs2
			bincmd = "0000000" + cmd[3] + cmd[2] + "011" + cmd[1] + "0110011";
			PCbincmd.insert(pair<int, string>(curPC, bincmd));
			curPC = curPC + 4;
			bincmd = "";
			
			// xori rd, rd, 1
			cmd[0] = "xori";
			cmd[2] = cmd[1]; // rd
			cmd[4] = "1";
			
			insfmt = 'i';
			opcode = "0010011";
			func3 = "100";
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
					
					bool* binimd = Dec2Bin(num, 6); // immediate is of 5 bits (but all 5 bits can be 1)
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
		
		PCbincmd.insert(pair<int, string>(curPC, bincmd));
		curPC = curPC + 4;
		return 1;
	}
}

void update_unfinished_cmd(){
	
	// iterate over every unfinished command
	for (int i : unfinished_cmd){
		string cmd;
		string bincmd = "";
		auto it = PCbincmd.begin();
		int t = i/4;
		while(t--) it++; // finding the right iterator
		cmd = it->second;
		
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
		bool found = 0;
		for(auto j = labelPC.begin(); j != labelPC.end(); ++j){
			if (j->first == label_name){
				found = 1;
				// we have the PC of the label
				offset = j->second - i; // i is the current PC
				break;
			}
		}
		
		if (found == 0){
			std::cout << "Label: " << label_name << " is not found\n"; 
			exit(1);
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
			char digchar;
			bool* binOffset = Dec2Bin(offset, 13); // branch have 13 bit offset field
			
			// 12|10:5
			digchar = '0' + binOffset[12];
			bincmd = bincmd + digchar;
			for (int j=10; j>=5; j--){
				digchar = '0' + binOffset[j];
				bincmd = bincmd + digchar;
			}
			
			if (cmd_name == "bgt" || cmd_name == "ble" || cmd_name == "bgtu" || cmd_name == "bleu"){
				// interchange rs1 and rs2
				bincmd = bincmd + rs1 + rs2;
			} else {
				// now we have the rs2 and rs1 field
				bincmd = bincmd + rs2 + rs1;
			}
			
			// now we have func3
			if (cmd_name == "beq") bincmd = bincmd + "000";
			else if (cmd_name == "bne") bincmd = bincmd + "001";
			else if (cmd_name == "blt" || cmd_name == "bgt") bincmd = bincmd + "100";
			else if (cmd_name == "bge" || cmd_name == "ble") bincmd = bincmd + "101";
			else if (cmd_name == "bltu" || cmd_name == "bgtu") bincmd = bincmd + "110";
			else if (cmd_name == "bgeu" || cmd_name == "bleu") bincmd = bincmd + "111";
			else {
				std::cout << "Branch statement opcode cannot be identified\n";
				exit(1);
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
		it->second = bincmd; // update the PCbincmd
	}
}

void getBinCmd(string file_path){
	while(generateBinCmd(file_path)){}
	update_unfinished_cmd();
}

/*
int main(){
	getBinCmd("new.asm");
	for (auto it = PCbincmd.begin(); it != PCbincmd.end(); it++){
		cout << it->first << " " << it->second << endl;
	}
}
*/


