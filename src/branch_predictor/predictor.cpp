#include <bits\stdc++.h>

using namespace std;

static map<int, bool> branch_history_1bit; // PC with their correponding taken and not taken
static map<int, int> branch_history_2bit_nt; // PC with their 2bit state value (SNT, NT, T, ST)
static map<int, int> branch_history_2bit_t; // PC with their 2bit state value (SNT, NT, T, ST)
// 0 for SNT, 1 for NT, 2 for T and 3 for ST
int state_2bit[4][2] = {{0, 1}, {0, 2}, {1, 3}, {2, 3}}; // 2D array graph for the states
static map<int, int> btb; // branch target buffer

std::map<int, bool>::iterator search(map<int, bool>* branch, int PC){
	// performs a search in the map for the address and return the corresponding iterator
	auto it = branch->lower_bound(PC);
    if (it != branch->end() && it->first == PC) {
        return it;
    }
    return branch->end();
}

std::map<int, int>::iterator search(map<int, int>* branch, int PC){
	// performs a search in the map for the address and return the corresponding iterator
	auto it = branch->lower_bound(PC);
    if (it != branch->end() && it->first == PC) {
        return it;
    }
    return branch->end();
}

void insert(map<int, bool>* branch, int PC, bool val){
	// insert a value into the memory
	auto it = search(branch, PC);
    if (it == branch->end()) {
        branch->insert(std::make_pair(PC, val));
    } else {
        it->second = val;
    }
}

void insert(map<int, int>* branch, int PC, int val){
	// insert a value into the memory
	auto it = search(branch, PC);
    if (it == branch->end()) {
        branch->insert(std::make_pair(PC, val));
    } else {
        it->second = val;
    }
}

bool static_taken(int PC){
	return 1; // always taken
}

bool static_nottaken(int PC){
	return 0; // always not taken
}

bool onebit(int PC, bool def){
	auto it = search(&branch_history_1bit, PC);
	if (it->first == PC){
		return it->second;
	} else {
		return def; // return default state
	}
}

void feedback_onebit(int PC, bool taken){
	insert(&branch_history_1bit, PC, taken); // update the PC
	auto it = search(&branch_history_1bit, PC);
}

bool twobit(int PC, int def){
	if (def == 0){
		auto it = search(&branch_history_2bit_nt, PC);
		if (it->first == PC){
			int pred = it->second;
			if (pred == 0 || pred == 1) return 0; // SNT or NT
			else return 1; // T or ST
		} else {
			return 0; // return default state
		}
	} else {
		auto it = search(&branch_history_2bit_t, PC);
		if (it->first == PC){
			int pred = it->second;
			if (pred == 0 || pred == 1) return 0; // SNT or NT
			else return 1; // T or ST
		} else {
			return 1; // return default state
		}
	}
}

void feedback_twobit_nt(int PC, bool taken){
	auto it = search(&branch_history_2bit_nt, PC);
	if (it->first == PC){
		int finalstate = state_2bit[it->second][taken]; // 2D array for calculating next state
		insert(&branch_history_2bit_nt, PC, finalstate);
	} else {
		int finalstate = state_2bit[1][taken]; // 2D array for calculating next state, current state is NT
		insert(&branch_history_2bit_nt, PC, finalstate);
	}
}

void feedback_twobit_t(int PC, bool taken){
	auto it = search(&branch_history_2bit_t, PC);
	if (it->first == PC){
		int finalstate = state_2bit[it->second][taken]; // 2D array for calculating next state
		insert(&branch_history_2bit_t, PC, finalstate);
	} else {
		int finalstate = state_2bit[2][taken]; // 2D array for calculating next state, current state is T
		insert(&branch_history_2bit_t, PC, finalstate);
	}
}

int totalstaticbranchinst(){
	int total_count = 0;
	for (auto it = PCbincmd.begin(); it != PCbincmd.end(); ++it){
		string bincmd = it->second;
	
		string opcode = "";
		opcode = opcode + bincmd[25] + bincmd[26] + bincmd[27] + bincmd[28] + bincmd[29] + bincmd[30] + bincmd[31];
		if (opcode == "1100011"){
			total_count++; // conditional branch
		} else if (opcode == "1101111"){
			total_count++; // jal instruction
		} else if (opcode == "1100111"){
			total_count++; // jalr instruction
		}
	}
	return total_count;
}

bool branchinst(int PC){
	// search the PC in PCbincmd and returns true if it's a branch instruction (conditional and unconditional)
	auto it = PCbincmd.begin();
	
	if (PC > (PCbincmd.size()-1)*4 || PC < 0){
		// if PC is negative then break or if PC increase the map size PC counter then break (the binary code does not have exit code)
		if (PC < 0) std::cout << "Warning! The PC reached a -ve value\n";
		return 0;
	}
	
	int t = PC/4;
	while (t--) it++;
	
	string bincmd = it->second;
	
	string opcode = "";
	opcode = opcode + bincmd[25] + bincmd[26] + bincmd[27] + bincmd[28] + bincmd[29] + bincmd[30] + bincmd[31];
	if (opcode == "1100011"){
		return 1; // conditional branch
	} else if (opcode == "1101111"){
		return 1; // jal instruction
	} else if (opcode == "1100111"){
		return 1; // jalr instruction
	} else {
		return 0;
	}
}

void generate_btb(){
	// generate branch target buffer
	for (auto it = PCbincmd.begin(); it != PCbincmd.end(); ++it){
		string bincmd = it->second;
	
		string opcode = "";
		opcode = opcode + bincmd[25] + bincmd[26] + bincmd[27] + bincmd[28] + bincmd[29] + bincmd[30] + bincmd[31];
		if (opcode == "1100011"){
			// conditional branch
			string imm = "";
			imm = imm + bincmd[0] + bincmd[24] + bincmd[1] + bincmd[2] + bincmd[3] + bincmd[4] + bincmd[5] + bincmd[6] + bincmd[20] + bincmd[21] + bincmd[22] + bincmd[23] + '0';
			btb.insert(std::make_pair(it->first, it->first + binstr2dec(imm)));
			
		} else if (opcode == "1101111"){
			// jal instruction
			string imm = "";
			imm = imm + bincmd[0] + bincmd[12] + bincmd[13] + bincmd[14] + bincmd[15] + bincmd[16] + bincmd[17] + bincmd[18] + bincmd[19] + bincmd[11] + bincmd[1] + bincmd[2] + bincmd[3] + bincmd[4] + bincmd[5] + bincmd[6] + bincmd[7] + bincmd[8] + bincmd[9] + bincmd[10] + '0';
			btb.insert(std::make_pair(it->first, it->first + binstr2dec(imm)));
			
		}
	}
}

void print_btb(){
	std::cout << "  Look-Up  ->   Branch" << std::endl;
	for (auto it = btb.begin(); it != btb.end(); ++it){
		std::cout << "0x" << int2hex_8byte(it->first) << " -> 0x" << int2hex_8byte(it->second) << std::endl; 
	}
}

void check_acc_bp(){
	// using vector<int> PCtrace
	int pred_static_taken = 0;
	int pred_static_nottaken = 0;
	int pred_onebit_nt = 0;
	int pred_onebit_t = 0;
	int pred_twobit_nt = 0;
	int pred_twobit_t = 0;
	int total_pred = 0;
	for (auto it = PCtrace.begin(); it != PCtrace.end(); ++it){
		if (branchinst(*it)){
			total_pred++;
			if (*(it+1) == *it + 4){
				// branch not taken
				pred_static_nottaken++;
				if (!onebit(*it, 0)) pred_onebit_nt++;
				if (!onebit(*it, 1)) pred_onebit_t++;
				feedback_onebit(*it, 0);
				if (!twobit(*it, 0)) pred_twobit_nt++;
				if (!twobit(*it, 1)) pred_twobit_t++;
				feedback_twobit_nt(*it, 0);
				feedback_twobit_t(*it, 0);
				
			} else {
				// branch taken
				pred_static_taken++;
				if (onebit(*it, 0)) pred_onebit_nt++;
				if (onebit(*it, 1)) pred_onebit_t++;
				feedback_onebit(*it, 1);
				if (twobit(*it, 0)) pred_twobit_nt++;
				if (twobit(*it, 1)) pred_twobit_t++;
				feedback_twobit_nt(*it, 1);
				feedback_twobit_t(*it, 1);
				
			}
		}
	}
	// printing the accuracy
	std::cout << "\nBranch Target Buffer (BTB):\n";
	generate_btb();
	print_btb();
	std::cout << "\nTotal Static Branch Instructions: " << totalstaticbranchinst() << "\nTotal Dynamic Branch Instructions: " << total_pred;
	std::cout << "\nAccuracy of the branch predictors are:\nStatic Taken: " << pred_static_taken * 100.0 / total_pred << "%\nStatic Not Taken: " << pred_static_nottaken * 100.0 / total_pred << "%\nDynamic One Bit (Taken): " << pred_onebit_t * 100.0 / total_pred << "%\nDynamic One Bit (Not Taken): " << pred_onebit_nt * 100.0 / total_pred << "%\nDynamic Two Bit (Taken): " << pred_twobit_t * 100.0 / total_pred << "%\nDynamic Two Bit (Not Taken): " << pred_twobit_nt * 100.0 / total_pred << "%\n";
}