#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include "assembler_riscv.cpp"

using namespace std;

vector<int> myMemory(20, 0);
vector<int> myRegister(32, 0);

unordered_map<string, char> getType{
    {"0110011",'R'}, {"0010011",'I'}, {"0000011",'L'},
    {"0100011",'S'}, {"1101111",'J'}, {"1100011",'B'},
    {"0110111",'U'}
};

class pipeline {
private:
    vector<string> inputFile; // Store all the binary instructions

    int programCounter = 0; // Points to the current line of execution
    bool isPcUpdated = false;
    string instruction;

    // Stage 3 variables
    string controlWord;
    int registerSource1;
    int registerSource2;
    int immediate;
    string function3;
    int registerDestination;

    // Stage 4 variables
    int arithmeticUnitOut;

    // Stage 5 variables
    int loadOut;

    void openFile(string s){
        string myText;
        ifstream MyReadFile("./testing/" + s);
        while (getline(MyReadFile, myText)) {
            inputFile.push_back(myText);
        }
        MyReadFile.close();
    }

    int getImmediate(char type,string s){
        if(type == 'I' || type == 'L') return stoi(s.substr(0,12),NULL,2);
        else if(type == 'U') return stoi(s.substr(0,20),NULL,2);
        else if(type == 'S'){
            string temp = s.substr(0,7)+s.substr(20,5);
            return stoi(temp,NULL,2);
        }
        else if(type == 'B'){
            string temp = s.substr(0,7)+s.substr(20,5);
            int value = stoi(temp,NULL,2);
            if(temp[0]=='1') value-= (1 << 12);
            return value;
        }
        else{
            string temp = s.substr(0,20);
            int value = stoi(temp,NULL,2);
            if(temp[0]=='1') value-=(1 << 20);
            return value;
        }
    }

    void stage1(); // Fetch stage (PC)
    void stage2(); // Decode and Immediate generation stage (DPC, NPC, I)
    void stage3(); // Execution stage (JPC, DPC, IMM, CW, RS1, RS2, IMM, F3, RD)
    void stage4(); // Memory access stage (CW, ALUOUT, RS2, RD)
    void stage5(); // Write-back stage (CW, LDOUT, ALUOUT, RD)

public:
    pipeline(string a){
        openFile(a);
        while(programCounter < inputFile.size()){
            stage1();
            stage2();
            stage3();
            stage4();
            stage5();
        }

    }

};

void pipeline :: stage1() {
    instruction = inputFile[programCounter];
}

void pipeline :: stage2() {
    char type = getType[instruction.substr(25, 7)]; // opcode for cw
    registerDestination = stoi(instruction.substr(20, 5), NULL, 2);
    function3 = instruction.substr(17, 3);
    registerSource1 = stoi(instruction.substr(12, 5), NULL, 2);
    registerSource2 = stoi(instruction.substr(7, 5), NULL, 2);
    immediate = getImmediate(type,instruction);

    controlWord = string(1,type);

}

void pipeline :: stage3() {
    if(controlWord == "R"){
        if(function3 == "000"){ // ADD operation
            arithmeticUnitOut = myRegister[registerSource1] + myRegister[registerSource2];
        } 
        else if(function3 == "001"){ // SLL operation
            arithmeticUnitOut = myRegister[registerSource1] << (myRegister[registerSource2] & 0x1F);
        }
        else if(function3 == "010"){ // SLT operation
            arithmeticUnitOut = (myRegister[registerSource1] * myRegister[registerSource2]);
        }
        else if(function3 == "011"){ // SUB operation
            arithmeticUnitOut = myRegister[registerSource1] - myRegister[registerSource2];
        }
        else if(function3 == "100"){ // XOR operation
            arithmeticUnitOut = myRegister[registerSource1] ^ myRegister[registerSource2];
        }
        else if(function3 == "101"){ // SRL operation
            arithmeticUnitOut = (unsigned)myRegister[registerSource1] >> (myRegister[registerSource2] & 0x1F);
        }
        else if(function3 == "110"){ // OR operation
            arithmeticUnitOut = myRegister[registerSource1] | myRegister[registerSource2];
        }
        else if(function3 == "111"){ // AND operation
            arithmeticUnitOut = myRegister[registerSource1] & myRegister[registerSource2];
        }
    }

    
    else if(controlWord == "I"){ 
        if (function3 == "000") { // ADDI operation
            arithmeticUnitOut = myRegister[registerSource1] + immediate;
        } 
        else if(function3 == "001"){ // SLLI operation
            arithmeticUnitOut = myRegister[registerSource1] << immediate;
        }
        else if(function3 == "100"){ // XORI operation
            arithmeticUnitOut = myRegister[registerSource1] ^ immediate;
        }
        else if(function3 == "101"){ // SRLI operation
            arithmeticUnitOut = myRegister[registerSource1] >> immediate;
        }
        else if(function3 == "110"){ // ORI operation
            arithmeticUnitOut = myRegister[registerSource1] | immediate;
        }
        else if(function3 == "111"){ // ANDI operation
            arithmeticUnitOut = myRegister[registerSource1] & immediate;
        }
    } 

    else if(controlWord == "S" || controlWord == "L"){
        arithmeticUnitOut = myRegister[registerSource1] + immediate;
    }

    else if(controlWord == "J"){ 

        arithmeticUnitOut = programCounter + immediate;
        myRegister[registerDestination] = programCounter;
        programCounter = arithmeticUnitOut;
        isPcUpdated = true;
    }

    else if(controlWord == "B") { 
        bool branchTaken = false;

        if(function3 == "000"){ // BEQ 
            branchTaken = (myRegister[registerSource1]==myRegister[registerSource2]);
        } 
        else if(function3 == "001"){ // BNE 
            branchTaken = (myRegister[registerSource1]!=myRegister[registerSource2]);
        } 
        else if(function3 == "100"){ // BLT 
            branchTaken = (myRegister[registerSource1]<myRegister[registerSource2]);
        } 
        else if(function3 == "101"){ // BGE 
            branchTaken = (myRegister[registerSource1]>=myRegister[registerSource2]);
        }
        else if(function3 == "110"){ //BLTU
            branchTaken = (myRegister[registerSource1]<myRegister[registerSource2]);
        }
        else if(function3 == "111"){ //BGEU
            branchTaken = (myRegister[registerSource1]>=myRegister[registerSource2]);
        }

        if(branchTaken){
            programCounter += immediate;
            isPcUpdated = true;
        }
    }

    controlWord = controlWord;
    registerDestination = registerDestination;
    registerSource2 = registerSource2;
}

void pipeline :: stage4() {
    if(controlWord == "L"){ 
        loadOut = myMemory[arithmeticUnitOut];
    } 
    else if(controlWord == "S"){ 
        myMemory[arithmeticUnitOut] = myRegister[registerSource2];
    }

    controlWord = controlWord;
    arithmeticUnitOut = arithmeticUnitOut;
    registerDestination = registerDestination;

}

void pipeline :: stage5() {
    if(controlWord == "R" || controlWord == "I"){ 
        myRegister[registerDestination] = arithmeticUnitOut;
    } 
    else if(controlWord == "L"){ 
        myRegister[registerDestination] = loadOut;
    }

    if(!isPcUpdated) programCounter++;
    isPcUpdated = false;
    
}

int main(){
    string s1 = "setBit.s";
    string s2 = "setBit.o";

    myMemory[0]=8;

    assembler a(s1);
    pipeline p(s2);

    cout<<myRegister[19]<<endl;
    cout<<myMemory[1]<<endl;
    
}