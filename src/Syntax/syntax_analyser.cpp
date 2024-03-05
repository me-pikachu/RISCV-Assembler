#include <iostream>

using namespace std;
string s[6];
string lex(){
    return s[0];
}

//function to convert string to int
int str2int(string c){
    int n=0;
    int i=1;
    int k=c.length();
    while(k--){
        n=n*10;
        n=n+(int)c[i];
        i++;
    }
    return n;
}

//function to convert the index number to its binary form
string bin_index(string c){
    char x[5];
    int i=0,n=0;
    if(c.length()==2){
        n=(int)c[1];
    }
    else if(c.length()==3){
        n=(int)c[1];
        n=n*10;
        n=n+(int)c[2];
    }
    while(n>0){
        x[4-i]=(n%2);
        n=n/2;
        i++;
    }
    string a;
    a=c[0]+x[0]+x[1]+x[2]+x[3]+x[4];
    return a;
}

//function to return error if the syntax is not correct
void error(){
    cout << "ERROR IN SYNTAX";
    s[0]='0';
    s[1]='0';
    s[2]='0';
    s[3]='0';
    s[4]='0';
    s[5]='0';
}

//function to check if the given register is in range of register (ie 0-31) or not
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

//function to check syntax and return the tokens in form of a string array
string* getcmd(){
    string c=lex();
    while(c=="\n") c=lex();
    if(c.length() ==2){
        if(c[0]=='o' && c[1]=='r'){             //checking if the command is or
            s[0]=c;
            c=lex();
            if(check_register(c)) {
                s[1]=bin_index(c);
            }
            else error();
            c=lex();
            if(c!=",") error();
            c=lex();
            if(check_register(c)) s[2]=bin_index(c);
            else error();
            c=lex();
            if(c!=",") error();
            c=lex();
            if(check_register(c)) s[3]=bin_index(c);
            else error();
            c=lex();
            if(c!="\n") error();
        }
        else if(c[0]=='l' && (c[1]=='b'|| c[1]=='w'|| c[1]=='h'||c[1]=='d')){       //checking if it is a load command
            s[0]=c;
            c=lex();
            if(check_register(c)) {
                s[1]=bin_index(c);
            }
            else error();
            c=lex();
            if(c!=",") error();
            c=lex();
            int n=str2int(c);
            if(n<-2048 || n>2047) error();
            else s[4]=c;
            c=lex();
            if(c!="(") error();
            c=lex();
            if(check_register(c)) s[2]=bin_index(c);
            else error();
            c=lex();
            if(c!=")") error();
            c=lex();
            if(c!="\n") error();
        }
        else if(c[0]=='s'&& (c[1]=='b'||c[1]=='w'||c[1]=='h'||c[1]=='d')){         //checking if it is a store command
            s[0]=c;
            c=lex();
            if(check_register(c)) {
                s[2]=bin_index(c);
            }
            else error();
            c=lex();
            if(c!=",") error();
            c=lex();
            int n=str2int(c);
            if(n<-2048 || n>2047) error();
            else s[4]=c;
            c=lex();
            if(c!="(") error();
            c=lex();
            if(check_register(c)) s[1]=bin_index(c);
            else error();
            c=lex();
            if(c!=")") error();
            c=lex();
            if(c!="\n") error();
        }
        else error();
    }
    else if(c.length()==3){
        if(c=="add"||c=="and"||c=="sll"||c=="slt"||c=="sra"||c=="srl"|| c=="sub"||c=="xor"||c=="mul"||c=="div"||c=="rem"){
            s[0]=c;
            c=lex();
            if(check_register(c)) {
                s[1]=bin_index(c);
            }
            else error();
            c=lex();
            if(c!=",") error();
            c=lex();
            if(check_register(c)) s[2]=bin_index(c);
            else error();
            c=lex();
            if(c!=",") error();
            c=lex();
            if(check_register(c)) s[3]=bin_index(c);
            else error();
            c=lex();
            if(c!="\n") error();
        }
        else if(c=="beq"||c=="bne"||c=="bge"||c=="blt"){
            s[0]=c;
            c=lex();
            if(check_register(c)) s[2]=bin_index(c);
            else error();
            c=lex();
            if(c!=",") error();
            c=lex();
            if(check_register(c)) s[3]=bin_index(c);
            else error();
            c=lex();
            if(c!=",") error();
            c=lex();
            s[5]=c;
            c=lex();
            if(c!="\n") error();
        }
    }
    return s;
    
}

int main(){
    
    return 0;
}
