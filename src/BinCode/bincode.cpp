#include <bits/stdc++.h>

using namespace std;

long long PC = 0; // setting the Program Counter initially to 0

map<string, int> labelPC; // linking all the labels with their corresponding PC
map<int, string> PCbincmd; // all the binary commands with their corresponding PC
vector<int> unfinished_cmd; // storing all the unfinished commands

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
		
	}
}

void update_unfinished_cmd(){
	
}

void getBinCmd(){
	while(generateBinCmd()){}
	update_unfinished_cmd();
}

