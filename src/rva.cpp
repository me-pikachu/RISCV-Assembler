#include <bits/stdc++.h>
#include <fstream>
#include "bincode/bincode.cpp"
#include "exec/exec.cpp"

using namespace std;

string int2hex(int num){
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

string bin2hex(string bin){
	string hex = "";
	
	for (int i=0; i<8; i++){
		if (bin[4*i] == '0' && bin[4*i+1] == '0' && bin[4*i+2] == '0' && bin[4*i+3] == '0') hex += '0';
		else if (bin[4*i] == '0' && bin[4*i+1] == '0' && bin[4*i+2] == '0' && bin[4*i+3] == '1') hex += '1';
		else if (bin[4*i] == '0' && bin[4*i+1] == '0' && bin[4*i+2] == '1' && bin[4*i+3] == '0') hex += '2';
		else if (bin[4*i] == '0' && bin[4*i+1] == '0' && bin[4*i+2] == '1' && bin[4*i+3] == '1') hex += '3';
		else if (bin[4*i] == '0' && bin[4*i+1] == '1' && bin[4*i+2] == '0' && bin[4*i+3] == '0') hex += '4';
		else if (bin[4*i] == '0' && bin[4*i+1] == '1' && bin[4*i+2] == '0' && bin[4*i+3] == '1') hex += '5';
		else if (bin[4*i] == '0' && bin[4*i+1] == '1' && bin[4*i+2] == '1' && bin[4*i+3] == '0') hex += '6';
		else if (bin[4*i] == '0' && bin[4*i+1] == '1' && bin[4*i+2] == '1' && bin[4*i+3] == '1') hex += '7';
		else if (bin[4*i] == '1' && bin[4*i+1] == '0' && bin[4*i+2] == '0' && bin[4*i+3] == '0') hex += '8';
		else if (bin[4*i] == '1' && bin[4*i+1] == '0' && bin[4*i+2] == '0' && bin[4*i+3] == '1') hex += '9';
		else if (bin[4*i] == '1' && bin[4*i+1] == '0' && bin[4*i+2] == '1' && bin[4*i+3] == '0') hex += 'A';
		else if (bin[4*i] == '1' && bin[4*i+1] == '0' && bin[4*i+2] == '1' && bin[4*i+3] == '1') hex += 'B';
		else if (bin[4*i] == '1' && bin[4*i+1] == '1' && bin[4*i+2] == '0' && bin[4*i+3] == '0') hex += 'C';
		else if (bin[4*i] == '1' && bin[4*i+1] == '1' && bin[4*i+2] == '0' && bin[4*i+3] == '1') hex += 'D';
		else if (bin[4*i] == '1' && bin[4*i+1] == '1' && bin[4*i+2] == '1' && bin[4*i+3] == '0') hex += 'E';
		else if (bin[4*i] == '1' && bin[4*i+1] == '1' && bin[4*i+2] == '1' && bin[4*i+3] == '1') hex += 'F';
	}
	
	return hex;
}

void write(string file_path){
	// we have to remove the .asm extension and add the .mc extension to it
	int ex = file_path.length();
	for (int i = file_path.length()-1; i >=0; --i){
		if (file_path[i] == '.'){
			ex = i;
			break;
		}
	}
	
	string filename = "";
	for (int i=0; i<ex; ++i){
		filename = filename + file_path[i];
	}
	
	string new_path = filename + ".mc";
	ofstream MyFile(new_path);
	for (auto it = PCbincmd.begin(); it != PCbincmd.end(); it++){
		MyFile << "0x" + int2hex(it->first) + " 0x" + bin2hex(it->second) + "\n";
	}
	MyFile.close();
}

int main(int argc, char* argv[]) {
	if (argc >= 2){
		for (int i=1; i<argc; i++){
			string file_path(argv[1]);
			std::cout << "File: " << file_path << std::endl;
			getBinCmd(file_path); // get the instruction in binary format
			write(file_path); // write to .mc file
			std::cout << "\nBefore execution:\n";
			dis_mem(); // display_memory
			exec(PCbincmd); // execute instruction in the binary format
			std::cout << "\n\nAfter execution:\n";
			dis_mem(); // display_memory
		}
	} else {
		std::cout << "Please enter a filename" << endl;
	}	
	
    return 0;
}
