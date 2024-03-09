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
		else if (num % 16 == 10) hex += 'a';
		else if (num % 16 == 11) hex += 'b';
		else if (num % 16 == 12) hex += 'c';
		else if (num % 16 == 13) hex += 'd';
		else if (num % 16 == 14) hex += 'e';
		else if (num % 16 == 15) hex += 'f';		
		num = num / 16;
	}
	
	return hex;
}

string bin2hex(string bin){
	string hex = "";
	
	for (int i=0; i<8; i++){
		if (bin[4*i+3] == '0' && bin[4*i+2] == '0' && bin[4*i+1] == '0' && bin[4*i] == '0') hex += '0';
		else if (bin[4*i+3] == '0' && bin[4*i+2] == '0' && bin[4*i+1] == '0' && bin[4*i] == '1') hex += '1';
		else if (bin[4*i+3] == '0' && bin[4*i+2] == '0' && bin[4*i+1] == '1' && bin[4*i] == '0') hex += '2';
		else if (bin[4*i+3] == '0' && bin[4*i+2] == '0' && bin[4*i+1] == '1' && bin[4*i] == '1') hex += '3';
		else if (bin[4*i+3] == '0' && bin[4*i+2] == '1' && bin[4*i+1] == '0' && bin[4*i] == '0') hex += '4';
		else if (bin[4*i+3] == '0' && bin[4*i+2] == '1' && bin[4*i+1] == '0' && bin[4*i] == '1') hex += '5';
		else if (bin[4*i+3] == '0' && bin[4*i+2] == '1' && bin[4*i+1] == '1' && bin[4*i] == '0') hex += '6';
		else if (bin[4*i+3] == '0' && bin[4*i+2] == '1' && bin[4*i+1] == '1' && bin[4*i] == '1') hex += '7';
		else if (bin[4*i+3] == '1' && bin[4*i+2] == '0' && bin[4*i+1] == '0' && bin[4*i] == '0') hex += '8';
		else if (bin[4*i+3] == '1' && bin[4*i+2] == '0' && bin[4*i+1] == '0' && bin[4*i] == '1') hex += '9';
		else if (bin[4*i+3] == '1' && bin[4*i+2] == '0' && bin[4*i+1] == '1' && bin[4*i] == '0') hex += 'a';
		else if (bin[4*i+3] == '1' && bin[4*i+2] == '0' && bin[4*i+1] == '1' && bin[4*i] == '1') hex += 'b';
		else if (bin[4*i+3] == '1' && bin[4*i+2] == '1' && bin[4*i+1] == '0' && bin[4*i] == '0') hex += 'c';
		else if (bin[4*i+3] == '1' && bin[4*i+2] == '1' && bin[4*i+1] == '0' && bin[4*i] == '1') hex += 'd';
		else if (bin[4*i+3] == '1' && bin[4*i+2] == '1' && bin[4*i+1] == '1' && bin[4*i] == '0') hex += 'e';
		else if (bin[4*i+3] == '1' && bin[4*i+2] == '1' && bin[4*i+1] == '1' && bin[4*i] == '1') hex += 'f';
	}
	
	return hex;
}

void write(string filename){
	string file_path = filename + ".mc";
	ofstream MyFile(file_path);
	for (auto it = PCbincmd.begin(); it != PCbincmd.end(); it++){
		MyFile<<int2hex(it->first) + " " + bin2hex(it->second) + "\n";
	}
	MyFile.close();
}

int main(int argc, char* argv[]) {
	
    return 0;
}
