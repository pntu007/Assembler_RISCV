#include<iostream>
#include<vector>
#include<unordered_map>
// #include<algorithm>
// #include<bits/stdc++.h>
#include <fstream>
#include <string>

using namespace std;

vector<int>mymemory(20,0);
vector<int>myRegister(32,0);

unordered_map<string,char>m{
    {"0110011",'R'},{"0010011",'I'},{"0000011",'L'},
    {"0100011",'S'},{"1101111",'J'},{"1100011",'B'},
    {"0110111",'U'}
};

class pipeline{
    private:
        vector<string>inputFile; // store all the binary instruction

        int programCounter=0; // points to current line of execution

        int directProgramCounter;
        int nextProgramCounter;
        string instruction;

        int instructionImmediate;
        string controlWord_s3;
        int registerSource1;
        int registerSource2_s3;
        int immediate;
        string function3;
        int registerDestination_s3;

        int airthmeticUnitOut;
        string controlWord_s4;
        int registerSource2_s4;
        int registerDestination_s4;

        int loadOut;
        string controlWord_s5;
        int registerDestination_s5;

        void openFile(string s){
            string myText;
            ifstream MyReadFile("./testing/" + s);
            while(getline(MyReadFile, myText)){
                inputFile.push_back(myText);
            }
            MyReadFile.close();
        }

    public : 
        pipeline(string a){
            openFile(a);
        }

        void stage1(); //PC
        void stage2(); //DPC,NPC,I
        void stage3(); //JPC,DPC,IMM,CW,RS1,RS2,IMM,F3,RD
        void stage4(); //CW,ALUOUT,RS2,RD
        void stage5(); //CW,LDOUT,ALUOUT,RD

};

void pipeline :: stage1(){
    instruction = inputFile[programCounter];
    directProgramCounter = programCounter;
    nextProgramCounter = programCounter+1;
}

void pipeline :: stage2(){
    char type = m[instruction.substr(25,7)]; // opcode for cw
    registerDestination_s3 = stoi(instruction.substr(20,5));
    function3 = instruction.substr(17,3);
    registerSource2_s3 = stoi(instruction.substr(12,5));
    registerSource1 = stoi(instruction.substr(7,5));
    string f7 = instruction.substr(20,5); // no use in my implimentaion 
    immediate = stoi(instruction.substr(0,12));

    // impliment cw && jpc using imm && imm for branching 
}

void pipeline :: stage3(){

}

void pipeline :: stage4(){

}

void pipeline :: stage5(){

}