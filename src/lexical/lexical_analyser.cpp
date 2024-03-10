#include <iostream>
#include <cctype>
#include <string>
#include <fstream>

using namespace std;


int column = 0;  // character that the lex is currently analyzing
int word_count = -1;  // token that the lex is on
int row = 0;  // row that the lex is currrently on
bool eof = false;

string lex(string file_path){
    word_count++;
    if(eof){
        word_count = 0;
        eof = false;
    }

    ifstream MyFile(file_path);
    string s="";
    for(int i = 0; i<=row; i++){
        if(MyFile.eof()){
            return "EOF";
        }
        getline(MyFile, s);
    }

    string token="";
    while(1){
        while(s[column] == ' '){
            column++;
        }
        
        if(s[column] == '\0'){
            token += "\n";
            column = 0;
            row++;
            eof = true;
            return token;
        }
        
        if(s[column] == ','){
            token += ",";
            column++;
            return token;
        }
        
        if(s[column] == '(' || s[column] == ')'){
            token += s[column];
            column++;
            return token;
        }
        
        // if(s[column] == ':'){
        //     token += s[column];
        //     column++;
        //     return token;
        // }
        
        if(s[column] == '+' || s[column] == '-'){
            token += s[column];
            column++;
        }
        
        while(isdigit(s[column])){
            token += s[column];
            column++;
        }
        
        if(s[column] == '.'){
            token += s[column];
            column++;
        }

        if(s[column] == '#'){
            do{
                token += s[column];
                column++;
            }while(s[column] != '\0');
        }
        
        if(s[column] == '\"'){
            do{
                token += s[column];
                column++;
            }while(s[column] != '\"');
            token += s[column];
            column++;
        }
        
        while(isalnum(s[column]) || s[column] == ':'){
            token += s[column];
            column++;
        }
        
        // if(s[column] == '\"'){
        //     token += s[column];
        //     column++;
        // }
        
        return token;
    }
}
