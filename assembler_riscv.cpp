#include<iostream>
#include<vector>
#include<unordered_map>
// #include<algorithm>
// #include<bits/stdc++.h>
#include <fstream>
#include <string>

using namespace std;

unordered_map<string,char>m{
    {"add",'R'},{"sub",'R'},{"and",'R'},
    {"or",'R'},{"xor",'R'},{"sll",'R'},
    {"srl",'R'},

    {"addi",'I'},{"xori",'I'},{"ori",'I'},
    {"andi",'I'},{"slli",'I'},{"srli",'I'},

    {"lb",'L'},{"lh",'L'},{"lw",'L'},
    {"lbu",'L'},{"lhu",'L'},

    {"sb",'S'},{"sh",'S'},{"sw",'S'},

    {"jal",'J'},

    {"beq",'B'},{"bne",'B'},{"blt",'B'},
    {"bge",'B'},{"bltu",'B'},{"bgeu",'B'},

    {"lui",'U'}, {"auipc",'U'}
};


class assembler{
    private:
        string instruction; // current assembly instruction
        vector<string> instructionSplit; // current assembly instruction after splitting
        vector<string> inputFile; // all assembly code line by line in current file
        vector<string> binaryString; // store the assembly to binary
        unordered_map<string,int> label; // labels used in current assembly file

        void myError(string s){
            cout<<s<<endl;
            exit(0);
        }

        void openFile(string s){
            string myText;
            ifstream MyReadFile(s);
            int lineNumber =0;
            while(getline(MyReadFile, myText)){
                if(myText=="") continue;
                if(myText.find(':')==string::npos){
                    inputFile.push_back(myText);
                    lineNumber++;
                } 
                else {
                    while(myText.back()<'a' || myText.back()>'z') myText.pop_back();
                    label[myText] = lineNumber;
                }
            }
            MyReadFile.close();
        }

        string getBinary(string s,int length,bool flag = false,int line =-1){
            bool isRegister = false;
            if(!flag  && !isdigit(s[0])) {
                s.erase(0,1);
                isRegister = true;
            }

            if(isRegister && (stoi(s)>31 || stoi(s)<0) ){
                myError("Register "+ s +" not available!!");
            } 
            
            if(flag && label.find(s)==label.end()){
                myError("label "+ s +" not declared!!");
            }

            int num = flag ? (label[s]-line) : stoi(s);
            bool isNegative = (num < 0);
            num = abs(num);
            
            string inBinary(length, '0');
            
            int i = length - 1; 
            while(num!=0 && i >= 0){
                inBinary[i] = '0' + (num%2);
                num/=2;
                i--;
            }

            if(isNegative) inBinary[0] = '1';
            return inBinary;
            // string inBinary = "";
            // while(num!=0){
            //     inBinary += '0'+(num%2);
            //     num/=2;
            // }
            // int n = inBinary.size();
            // for(int i=0;i<length-n;i++) inBinary+='0';
            // reverse(inBinary.begin(),inBinary.end());
        }

        void checkInstructionFormat(string s){
            string toCheck = "";
            for(int i = 1; i < instructionSplit.size(); i++) {
                toCheck += instructionSplit[i];
            }
            
            int i=0, j=0;
            while(j<s.size() && i<toCheck.size()){
                if(s[j]=='x' && toCheck[i]=='x') i++;
                else if(s[j]=='*') while(i<toCheck.size() && isdigit(toCheck[i])) i++;
                else if(s[j] == '#') while(i<toCheck.size() && isalpha(toCheck[i])) i++;
                else break;

                j++;
            }

            if(j!=s.size() || i!=toCheck.size()){
                myError("Incorrect Instruction Format");
            } 

        }

        char splitTheInstruction(){

            string tempString="";
            for(auto x:instruction){

                if (!isalnum(x) && x!=' ' && x!=',' && x!='(' && x!=')') {
                    myError("Invalid character " + string(1,x) + " found in instruction.");
                }

                if(x!=' ' && x!=',' && x!='(' && x!=')'){
                    tempString.push_back(x);
                }
                else{
                    if(!tempString.empty()){
                        instructionSplit.push_back(tempString);
                        tempString = "";
                    } 
                }
            }
            if(!tempString.empty()) instructionSplit.push_back(tempString);

            if(m.find(instructionSplit[0])==m.end()){
                myError(instructionSplit[0]+" is not supported!!");
            } 

            char type = m[instructionSplit[0]];

            if(((type=='J' || type=='U') && instructionSplit.size()!=3) || (type!='J' && type!='U' && instructionSplit.size()!=4)){
                myError("Incorrect Instruction!!");
            }

            // check the format of instruction is correct or not
            if(type=='R') checkInstructionFormat("x*x*x*");
            else if(type=='I') checkInstructionFormat("x*x*");
            else if(type=='L') checkInstructionFormat("x*x*");
            else if(type=='S') checkInstructionFormat("x*x*");
            else if(type=='J') checkInstructionFormat("x*#");
            else if(type=='B') checkInstructionFormat("x*x*#");
            else if(type=='U') checkInstructionFormat("x*");

            return type;
       }

       void getBinaryFile(string name){
            string s="";
            for(auto x:name){
                if(x=='.') break;
                s+=x;
            }
            ofstream MyFile(s);
            for(auto line : binaryString){
                MyFile<<line<<"\n";
            }
            MyFile.close();
            
            inputFile.clear();
            label.clear();
            binaryString.clear();

            // cout<<binaryString<<endl;
            // ofstream MyFile("filename.txt", ios::app);
            // MyFile.seekp(0, ios::end);
            // streampos currentPosition = MyFile.tellp();

            // if(currentPosition != 0) MyFile<<"\n";
            
            // MyFile<<binaryString;
            // MyFile.close();
       }

    public:
       assembler(string a){
            openFile(a);
            for(int line=0;line<inputFile.size();line++){
                instruction=inputFile[line];
                char type = splitTheInstruction();
                if(type=='R') rtype();
                else if(type=='I') itype();
                else if(type=='L') iltype();
                else if(type=='S') stype();
                else if(type=='J') jtype(line);
                else if(type=='B') btype(line);
                else if(type=='U') utype();
                instructionSplit.clear();
            }
            getBinaryFile(a);
            cout<<"TASK COMPLETE!!"<<endl;
       }  
       
       void rtype();
       void itype();
       void iltype();
       void stype();
       void jtype(int);
       void btype(int);
       void utype();
};

void assembler :: rtype(){
    string opcode = "0110011";
    string f7 = "0000000";
    string f3="";

    if(instructionSplit[0]=="add") f3 = "000";
    else if(instructionSplit[0]=="or") f3 = "001";
    else if(instructionSplit[0]=="xor") f3 = "010";
    else if(instructionSplit[0]=="sub") f3 = "011";
    else if(instructionSplit[0]=="and") f3 = "100";
    else if(instructionSplit[0]=="srl") f3 = "101";
    else if(instructionSplit[0]=="sll") f3 = "110";
    
    string rd = getBinary(instructionSplit[1],5);
    string rs1 = getBinary(instructionSplit[2],5);
    string rs2 = getBinary(instructionSplit[3],5);

    binaryString.push_back(f7 + rs2 + rs1 + f3 + rd + opcode);
}

void assembler :: itype(){
    string opcode = "0010011";
    string f3="";

    if(instructionSplit[0]=="addi") f3 = "000";
    else if(instructionSplit[0]=="ori") f3 = "001";
    else if(instructionSplit[0]=="xori") f3 = "010";
    else if(instructionSplit[0]=="andi") f3 = "011";
    else if(instructionSplit[0]=="srli") f3 = "100";
    else if(instructionSplit[0]=="slli") f3 = "101";
    
    string rd = getBinary(instructionSplit[1],5);
    string rs1 = getBinary(instructionSplit[2],5);
    string imm = getBinary(instructionSplit[3],12);

    binaryString.push_back(imm + rs1 + f3 + rd + opcode);
}

void assembler :: iltype(){
    string opcode = "0000011";
    string f3="";

    if(instructionSplit[0]=="lb") f3 = "000";
    else if(instructionSplit[0]=="lh") f3 = "001";
    else if(instructionSplit[0]=="lw") f3 = "010";
    else if(instructionSplit[0]=="lbu") f3 = "011";
    else if(instructionSplit[0]=="lhu") f3 = "100";
    
    string rd = getBinary(instructionSplit[1],5);
    string imm = getBinary(instructionSplit[2],12);
    string rs1 = getBinary(instructionSplit[3],5);

    binaryString.push_back(imm + rs1 + f3 + rd + opcode);
}

void assembler :: stype(){
    string opcode = "0100011";
    string f3="";

    if(instructionSplit[0]=="sb") f3 = "000";
    else if(instructionSplit[0]=="sh") f3 = "001";
    else if(instructionSplit[0]=="sw") f3 = "010";
    
    string rs2 = getBinary(instructionSplit[1],5); // src
    string imm = getBinary(instructionSplit[2],12);
    string rs1 = getBinary(instructionSplit[3],5); // base

    string imm2 = imm.substr(0,7); 
    string imm1 = imm.substr(7,5);

    binaryString.push_back(imm2 + rs2 + rs1 + f3 + imm1 + opcode);
    
}

void assembler :: jtype(int line){
    string opcode = "1101111";
    string rd = getBinary(instructionSplit[1],5);
    string imm = getBinary(instructionSplit[2],20,true,line);

    binaryString.push_back(imm + rd + opcode);
}

void assembler :: btype(int line){
    string opcode = "1100011";
    string f3="";

    if(instructionSplit[0]=="beq") f3 = "000";
    else if(instructionSplit[0]=="bne") f3 = "001";
    else if(instructionSplit[0]=="blt") f3 = "010";
    else if(instructionSplit[0]=="bge") f3 = "011";
    else if(instructionSplit[0]=="bltu") f3 = "100";
    else if(instructionSplit[0]=="bgeu") f3 = "101";

    string rs1 = getBinary(instructionSplit[1],5);
    string rs2 = getBinary(instructionSplit[2],5);
    string imm = getBinary(instructionSplit[3],12,true,line); 

    string imm2 = imm.substr(0,7); 
    string imm1 = imm.substr(7,5);

    binaryString.push_back(imm2 + rs2 + rs1 + f3 + imm1 + opcode);
}
 
void assembler :: utype(){
    string opcode = "0000110";

    string rd = getBinary(instructionSplit[1],5);
    string imm = getBinary(instructionSplit[2],20);

    binaryString.push_back(imm + rd + opcode);
}

int main(){
    string s="filename.s";
    // cout<<"ENTER THE STRING: ";
    // getline(cin, s);
    assembler a(s);

    // string myText;
    // ifstream MyReadFile("filename.txt");
    // while(getline(MyReadFile, myText)){
    //      cout << myText<<endl;
    // }
    // MyReadFile.close();

// 00000000010100100000000110110011
// 00000000101000011000000110010011
// 00000000001000011001000111100011
// 00000000101000011010000100000011
// 10000000000000000010000101101111
// 00000000000100011010011010100011
}