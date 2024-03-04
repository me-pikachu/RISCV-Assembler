#include <iostream>

using namespace std;
string s[6];
string lex(){
    return s[0];
}

void error(){
    cout << "ERROR IN SYNTAX";
    s[0]='0';
    s[1]='0';
    s[2]='0';
    s[3]='0';
    s[4]='0';
    s[5]='0';
}

bool check_register(string c){
    if(c[0]=='x' && c.length()==2 && (c[1]=='0'||c[1]=='1' ||c[1]=='2' ||c[1]=='3' ||c[1]=='4' ||c[1]=='5' ||c[1]=='6' ||c[1]=='7' ||c[1]=='8' ||c[1]=='9')) return true;
    else if(c[0]=='x' && c.length()==3){
        if((c[1]=='1'||c[1]=='2') && (c[2]=='0'||c[2]=='1'||c[2]=='2'||c[2]=='3'||c[2]=='4'||c[2]=='5'||c[2]=='6'||c[2]=='7'||c[2]=='8'||c[2]=='9')){
            return true;
        }
        else if(c[1]=='3' && (c[2]=='0' || c[2]=='1')){
            return true;
        }
        else return false;
    }
    else return false;
}

string* getcmd(){
    string c=lex();
    if(c.length() ==2){
        if(c[0]=='o' && c[1]=='r'){
            s[0]=c;
            c=lex();
            if(check_register(c)) s[1]=c;
            else error();
            c=lex();
            if(c!=",") error();
            c=lex();
            if(check_register(c)) s[2]=c;
            else error();
            c=lex();
            if(c!=",") error();
            c=lex();
            if(check_register(c)) s[3]=c;
            else error();
        }
    }
    return s;
    
}

int main(){
    
    return 0;
}

