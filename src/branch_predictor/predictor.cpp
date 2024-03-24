#include <bits\stdc++.h>

using namespace std;

static map<int, bool> branch_history_1bit; // PC with their correponding taken and not taken
static map<int, int> branch_history_2bit; // PC with their 2bit state value (SNT, NT, T, ST)
// 0 for SNT, 1 for NT, 2 for T and 3 for ST

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

bool onebit(int PC){
	auto it = search(&branch_history_1bit, PC);
	if (it->first == PC){
		return it->second;
	} else {
		return 0; // always not taken if coming on that branch first time
	}
}

void feedback_onebit(int PC, bool taken){
	insert(&branch_history_1bit, PC, taken); // update the PC
	auto it = search(&branch_history_1bit, PC);
}

bool twobit(int PC){
	auto it = search(&branch_history_2bit, PC);
	if (it->first == PC){
		int pred = it->second;
		if (pred == 0 || pred == 1) return 0; // SNT or NT
		else return 1; // T or ST
	} else {
		return 0; // always not taken if coming on that branch first time
	}
}

void feedback_twobit(int PC, bool taken){
	auto it = search(&branch_history_2bit, PC);
	if (it->first == PC){
		int pred = it->second;
		// decrease the state if not taken
		// increase the state if taken
		if (taken){
			pred++;
		} else {
			pred--;
		}
		
		// make sure that pred is in range [0,3]
		if (pred < 0) pred++; 
		else if (pred > 3) pred--;
		insert(&branch_history_2bit, PC, pred);
		
	} else {
		insert(&branch_history_2bit, PC, taken+1); // if (taken == 0) -> NT and if (taken == 1) -> T
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

void check_acc_bp(){
	// using vector<int> PCtrace
	int pred_static_taken = 0;
	int pred_static_nottaken = 0;
	int pred_onebit = 0;
	int pred_twobit = 0;
	int total_pred = 0;
	for (auto it = PCtrace.begin(); it != PCtrace.end(); ++it){
		if (branchinst(*it)){
			total_pred++;
			if (*(it+1) == *it + 4){
				// branch not taken
				pred_static_nottaken++;
				if (!onebit(*it)) pred_onebit++;
				feedback_onebit(*it, 0);
				if (!twobit(*it)) pred_twobit++;
				feedback_twobit(*it, 0);
			} else {
				// branch taken
				pred_static_taken++;
				if (onebit(*it)) pred_onebit++;
				feedback_onebit(*it, 1);
				if (twobit(*it)) pred_twobit++;
				feedback_twobit(*it, 1);
			}
		}
	}
	// printing the accuracy
	std::cout << "\nTotal Static Branch Instructions: " << totalstaticbranchinst() << "\nTotal Dynamic Branch Instructions: " << total_pred;
	std::cout << "\nAccuracy of the branch predictors are:\nStatic Taken: " << pred_static_taken * 100.0 / total_pred << "%\nStatic Not Taken: " << pred_static_nottaken * 100.0 / total_pred << "%\nDynamic One Bit: " << pred_onebit * 100.0 / total_pred << "%\nDynamic Two Bit: " << pred_twobit * 100.0 / total_pred << "%\n";
}