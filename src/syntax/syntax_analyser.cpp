#include <iostream>
#include<string>
#include<algorithm>
#include "../lexical/lexical_analyser.cpp"

using namespace std;
string s[6];
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


//function to convert string to int
int str2int(string c){
    long long n=0;
    int i;
    unsigned long k=c.length();
    if(c[0]!='-'){
        if(c[0]=='0'&&(c[1]=='x'|| c[1]=='X')){
            i=2;
            int loop=k-2;
            while(loop--){
                if(c[i]=='a'|| c[i]=='A') n=n+pow(16,loop)*10;
                else if(c[i]=='b'|| c[i]=='B') n=n+pow(16,loop)*11;
                else if(c[i]=='c'|| c[i]=='C') n=n+pow(16,loop)*12;
                else if(c[i]=='d'|| c[i]=='D') n=n+pow(16,loop)*13;
                else if(c[i]=='e'|| c[i]=='E') n=n+pow(16,loop)*14;
                else if(c[i]=='f'|| c[i]=='F') n=n+pow(16,loop)*15;
                else if(c[i]=='0'||c[i]=='1'||c[i]=='2'||c[i]=='3'||c[i]=='4'||c[i]=='5'||c[i]=='6'||c[i]=='7'||c[i]=='8'||c[i]=='9') n=n+pow(2,loop)*(c[i]-48);
                else error();
                i++;
            }
            if(n>2147483647) n=n-4294967296;
        }
        else if(c[0]=='0'&&(c[1]=='b'||c[1]=='B')){
            i=2;
            int loop=k-2;
            while(loop--){
                n=n+pow(2,loop)*(c[i]-48);
                i++;
            }
            if(n>2147483647) n=n-4294967296;
        }
        else if(c[0]=='0'&&(c[1]=='1'||c[1]=='2'||c[1]=='3'||c[1]=='4'||c[1]=='5'||c[1]=='6'||c[1]=='7'||c[1]=='8'||c[1]=='9')){
            i=1;
            k=k-1;
            while(k--){
                n=n+pow(8,k)*(c[i]-48);
                i++;
            }
            if(n>2147483647) n=n-4294967296;
        }
        else if(c[0]=='0'||c[0]=='1'||c[0]=='2'||c[0]=='3'||c[0]=='4'||c[0]=='5'||c[0]=='6'||c[0]=='7'||c[0]=='8'||c[0]=='9'){
            i=0;
            while(k--){
                n=n+pow(10,k)*(c[i]-48);
                i++;
            }
        }
        else error();
    }
    else if(c[0]=='-'){
        if(c[1]=='0'&&(c[2]=='x'|| c[2]=='X')){
            i=3;
            int loop=k-3;
            while(loop--){
                if(c[i]=='a'|| c[i]=='A') n=n+pow(16,loop)*10;
                else if(c[i]=='b'|| c[i]=='B') n=n+pow(16,loop)*11;
                else if(c[i]=='c'|| c[i]=='C') n=n+pow(16,loop)*12;
                else if(c[i]=='d'|| c[i]=='D') n=n+pow(16,loop)*13;
                else if(c[i]=='e'|| c[i]=='E') n=n+pow(16,loop)*14;
                else if(c[i]=='f'|| c[i]=='F') n=n+pow(16,loop)*15;
                else if(c[i]=='0'||c[i]=='1'||c[i]=='2'||c[i]=='3'||c[i]=='4'||c[i]=='5'||c[i]=='6'||c[i]=='7'||c[i]=='8'||c[i]=='9') n=n+pow(2,loop)*(c[i]-48);
                else error();
                i++;
            }
            if(n>2147483647) n=n-4294967296;
        }
        else if(c[1]=='0'&&(c[2]=='b'||c[2]=='B')){
            i=3;
            int loop=k-3;
            while(loop--){
                n=n+pow(2,loop)*(c[i]-48);
                i++;
            }
            if(n>2147483647) n=n-4294967296;
        }
        else if(c[1]=='0'&&(c[2]=='1'||c[2]=='2'||c[2]=='3'||c[2]=='4'||c[2]=='5'||c[2]=='6'||c[2]=='7'||c[2]=='8'||c[2]=='9')){
            i=2;
            k=k-2;
            while(k--){
                n=n+pow(8,k)*(c[i]-48);
                i++;
            }
            if(n>2147483647) n=n-4294967296;
        }
        else if(c[1]=='0'||c[1]=='1'||c[1]=='2'||c[1]=='3'||c[1]=='4'||c[1]=='5'||c[1]=='6'||c[1]=='7'||c[1]=='8'||c[1]=='9'){
            i=1;
            while(k--){
                n=n+pow(10,k)*(c[i]-48);
                i++;
            }
        }
        else error();
        n=n*(-1);
    }
    return n;
}

//function to convert the index number to its binary form
string bin_index(string c){
    string x;
    int n=0;
    if(c.length()==2){

        n=c[1]-48;
    }
    else if(c.length()==3){
        n=c[1]-48;
        n=n*10;
        n=n+c[2]-48;
    }
    int loop=5;
    while(loop--){
        int k=n%2;
        if(k==1) x=x+"1";
        else x=x+"0";
        n=n/2;
    }
    string a;
    reverse(x.begin(),x.end());
    return x;
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
    s[0]="";
    s[1]="";
    s[2]="";
    s[3]="";
    s[4]="";
    s[5]="";
    string c=lex(file_path);
    unsigned long len=c.length();
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
        else s[4]=to_string(n);
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
        else s[4]=to_string(n);
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
        else s[4]=to_string(n);
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
        else s[4]=to_string(m);
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
    else if(c[len-1]==':'){
        string c1;
		s[0] = "labelread";
        for (unsigned long i=0; i<len-1;i++) {
            c1=c1+c[i];
        }
        s[5]=c1;
    }
    else error();
    return s;
    
}


/*
int main(){
    getcmd("new.asm");
    cout <<s[0]<<" "<<s[1]<<" "<<s[2]<<" "<<s[3]<<" "<<s[4]<<" "<<s[5];
    getcmd("new.asm");
    cout <<s[0]<<" "<<s[1]<<" "<<s[2]<<" "<<s[3]<<" "<<s[4]<<" "<<s[5];
    getcmd("new.asm");
    cout <<s[0]<<" "<<s[1]<<" "<<s[2]<<" "<<s[3]<<" "<<s[4]<<" "<<s[5];
    getcmd("new.asm");
    cout <<s[0]<<" "<<s[1]<<" "<<s[2]<<" "<<s[3]<<" "<<s[4]<<" "<<s[5];
    getcmd("new.asm");
    cout <<s[0]<<" "<<s[1]<<" "<<s[2]<<" "<<s[3]<<" "<<s[4]<<" "<<s[5];
    return 0;
}
*/
