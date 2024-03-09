#include <iostream>
#include "../lexical/lexical_analyser.cpp"

using namespace std;
string s[6];

//function to convert string to int
int str2int(string c){
    int n=0;
    int i=1;
    unsigned long k=c.length();
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
    cout << "ERROR IN SYNTAX\n";
    s[0]='0';
    s[1]='0';
    s[2]='0';
    s[3]='0';
    s[4]='0';
    s[5]='0';
}

//function to check if the given register is in range of register (ie 0-31) or not
bool check_register(string c){
    if(c[0]=='x' && c.length()==2 && (c[1]=='0'||c[1]=='1'||c[1]=='2'||c[1]=='3'||c[1]=='4'||c[1]=='5'||c[1]=='6'||c[1]=='7'||c[1]=='8'||c[1]=='9')) return true;
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
string* getcmd(string file_path){
    string c=lex(file_path);
    unsigned long length=c.length();
    while(c=="\n") c=lex(file_path);
    if(c=="add"||c=="and"||c=="sll"||c=="slt"||c=="sra"||c=="srl"||c=="sub"||c=="xor"||c=="mul"||c=="div"||c=="rem"||c=="or"){
        s[0]=c;
        c=lex(file_path);
        if(check_register(c)) {
            s[1]=bin_index(c);
        }
        else error();
        c=lex(file_path);
        if(c!=",") error();
        c=lex(file_path);
        if(check_register(c)) s[2]=bin_index(c);
        else error();
        c=lex(file_path);
        if(c!=",") error();
        c=lex(file_path);
        if(check_register(c)) s[3]=bin_index(c);
        else error();
        c=lex(file_path);
        if(c!="\n") error();
    }
    else if(c=="ori"||c=="addi"||c=="andi"){
        s[0]=c;
        c=lex(file_path);
        if(check_register(c)) {
            s[1]=bin_index(c);
        }
        else error();
        c=lex(file_path);
        if(c!=",") error();
        c=lex(file_path);
        if(check_register(c)) s[2]=bin_index(c);
        else error();
        c=lex(file_path);
        if(c!=",") error();
        c=lex(file_path);
        int n=str2int(c);
        if(n<-2048 || n>2047) error();
        else s[4]=c;
        c=lex(file_path);
        if(c!="\n") error();
    }
    else if(c=="lb"||c=="ld"||c=="lw"||c=="lh"||c=="jalr"){       //checking if it is a load command
        s[0]=c;
        c=lex(file_path);
        if(check_register(c)) {
            s[1]=bin_index(c);
        }
        else error();
        c=lex(file_path);
        if(c!=",") error();
        c=lex(file_path);
        int n=str2int(c);
        if(n<-2048 || n>2047) error();
        else s[4]=c;
        c=lex(file_path);
        if(c!="(") error();
        c=lex(file_path);
        if(check_register(c)) s[2]=bin_index(c);
        else error();
        c=lex(file_path);
        if(c!=")") error();
        c=lex(file_path);
        if(c!="\n") error();
    }
    else if(c=="sb"||c=="sd"||c=="sw"||c=="sh"){         //checking if it is a store command
        s[0]=c;
        c=lex(file_path);
        if(check_register(c)) {
            s[3]=bin_index(c);
        }
        else error();
        c=lex(file_path);
        if(c!=",") error();
        c=lex(file_path);
        int n=str2int(c);
        if(n<-2048 || n>2047) error();
        else s[4]=c;
        c=lex(file_path);
        if(c!="(") error();
        c=lex(file_path);
        if(check_register(c)) s[2]=bin_index(c);
        else error();
        c=lex(file_path);
        if(c!=")") error();
        c=lex(file_path);
        if(c!="\n") error();
    }
    else if(c=="beq"||c=="bne"||c=="bge"||c=="blt"){
        s[0]=c;
        c=lex(file_path);
        if(check_register(c)) s[2]=bin_index(c);
        else error();
        c=lex(file_path);
        if(c!=",") error();
        c=lex(file_path);
        if(check_register(c)) s[3]=bin_index(c);
        else error();
        c=lex(file_path);
        if(c!=",") error();
        c=lex(file_path);
        s[5]=c;
        c=lex(file_path);
        if(c!="\n") error();
    }
    else if(c=="lui" || c=="auipc"){
        s[0]=c;
        c=lex(file_path);
        if(check_register(c)) {
            s[1]=bin_index(c);
        }
        else error();
        c=lex(file_path);
        if(c!=",") error();
        c=lex(file_path);
        int m=str2int(c);
        if(m>1048575 || m<0) error();
        else s[4]=c;
        c=lex(file_path);
        if(c!="\n") error();
    }
    else if(c=="jal"){
        s[0]=c;
        c=lex(file_path);
        if(check_register(c)) {
            s[1]=bin_index(c);
        }
        else error();
        c=lex(file_path);
        if(c!=",") error();
        c=lex(file_path);
        s[5]=c;
        c=lex(file_path);
        if(c!="\n") error();
    }
    else if(c[length-1]==':'){
        string c1;
        for (unsigned long i=0; i<length-1;i++) {
            c1=c1+c[i];
        }
        s[5]=c1;
    }
    else error();
    return s;
    
}



int main(){
    getcmd("new.asm");
    return 0;
}
