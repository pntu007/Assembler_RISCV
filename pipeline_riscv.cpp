#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include <bitset>
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
    bitset<32> registerLock;

    int programCounter = 0; // Points to the current line of execution
    int directProgramCounter;
    string instruction;

    // Stage status flags
    bool stage1Active = true;
    bool stage2Active = false;
    bool stage3Active = false;
    bool stage4Active = false;
    bool stage5Active = false;
    bool stopForOneTime = false;

    // Stage 3 variables
    int directProgramCounter_s3;
    string controlWord_s3;
    int registerSource1_s3;
    int registerSource2_s3;
    int immediate;
    string function3;
    int registerDestination_s3;

    // Stage 4 variables
    int arithmeticUnitOut_s4;
    string controlWord_s4;
    int registerSource2_s4;
    int registerDestination_s4;

    // Stage 5 variables
    int loadOut;
    int arithmeticUnitOut_s5;
    string controlWord_s5;
    int registerDestination_s5;

    bool bubbleInserted_data = false; 
    bool bubbleInserted_control = false;

    void openFile(string s){
        string myText;
        ifstream MyReadFile("./testing/" + s);
        while (getline(MyReadFile, myText)) {
            // cout<<"instruction : "<<myText<<endl;
            inputFile.push_back(myText);
        }
        MyReadFile.close();
    }

    int getImmdiate(char type,string s){
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

    // Function to detect hazards and insert a bubble if needed
    void detectHazard(){
        if(controlWord_s3 == "R" || controlWord_s3 == "B"){
            if (registerDestination_s4 == registerSource1_s3 || registerDestination_s4 == registerSource2_s3) {
                bubbleInserted_data = true;
                registerLock[registerDestination_s4]=1;
                // cout<<"data bubble inserted"<<endl;
            }
        }
        if(controlWord_s3 == "I" || controlWord_s3 == "L"){
            if (registerDestination_s4 == registerSource1_s3) {
                bubbleInserted_data = true;
                registerLock[registerDestination_s4]=1;
                // cout<<"data bubble inserted"<<endl;
            }
        }
        if(controlWord_s3 == "B" || controlWord_s3 == "J"){
            bubbleInserted_control = true;
            // cout<<"control bubble inserted"<<endl;
        } 
    }

    void stage1(); // Fetch stage (PC)
    void stage2(); // Decode and Immediate generation stage (DPC, NPC, I)
    void stage3(); // Execution stage (JPC, DPC, IMM, CW, RS1, RS2, IMM, F3, RD)
    void stage4(); // Memory access stage (CW, ALUOUT, RS2, RD)
    void stage5(); // Write-back stage (CW, LDOUT, ALUOUT, RD)

public:
    pipeline(string a){
        // cout<<"file name "<<a<<endl; 
        openFile(a);
        programCounter=0;

        while(programCounter < inputFile.size() || stage1Active || stage2Active || stage3Active || stage4Active || stage5Active){
                if(stage5Active) stage5();
                if(stage4Active) stage4();
                if(stage3Active) stage3();
                if(stage2Active) stage2();
                if(stage1Active) stage1();
                
            }
        }
};

void pipeline :: stage1() {
    // cout<<"stage1.. start"<<endl;

    if(bubbleInserted_data || bubbleInserted_control){
        // cout<<"stage1 stalled"<<endl;
        return;
    }

    // cout<<"pc : "<<programCounter<<endl;
    if(programCounter < inputFile.size()){
        instruction = inputFile[programCounter];
        directProgramCounter = programCounter;
        programCounter++;
        stage2Active = true; 
        // cout<<"pc Updated : "<<programCounter<<endl;
    }
    else{
        stage1Active = false; 
        // cout<<"stage 1 set to false"<<endl;
    }
    // cout<<"stage1.. exit"<<endl;
}

void pipeline :: stage2() {
    // if stage is not cleared than it will run again
    if(!stage1Active){
        stage2Active = false;
        // cout<<"stage 2 set to false"<<endl;
        return;
    }

    if(stopForOneTime == true){
        stopForOneTime = false;
        // cout<<"Stopping stage 2 due to unavoidable reason"<<endl;
        return;
    }

    if(bubbleInserted_data || bubbleInserted_control){
        // cout<<"stage2 stalled"<<endl;
        return;
    }

    // cout<<"stage2.. start"<<endl;
    char type = getType[instruction.substr(25, 7)]; // opcode for cw
    registerDestination_s3 = stoi(instruction.substr(20, 5), NULL, 2);
    function3 = instruction.substr(17, 3);
    registerSource1_s3 = stoi(instruction.substr(12, 5), NULL, 2);
    registerSource2_s3 = stoi(instruction.substr(7, 5), NULL, 2);
    immediate = getImmdiate(type,instruction);

    controlWord_s3 = string(1,type);
    directProgramCounter_s3 = directProgramCounter; 

    stage3Active = true;   

    // cout<<"Type "<<type<<endl;
    // cout<<"rd "<<registerDestination_s3<<endl;
    // cout<<"f3 "<<function3<<endl;
    // cout<<"rs2 "<<registerSource2_s3<<endl;
    // cout<<"rs1 "<<registerSource1_s3<<endl;
    // cout<<"imm "<<immediate<<endl;

    detectHazard();

    // cout<<"stage2.. exit"<<endl;

}

void pipeline :: stage3() {
    if(!stage2Active){
        stage3Active = false; // Deactivate if stage 2 is not active
        // cout<<"stage 3 set to false"<<endl;
        return;
    }

    if(bubbleInserted_data){
        // cout<<"stage3 stalled"<<endl;
        return;
    }

    // cout<<"stage3.. start"<<endl;
    if(controlWord_s3 == "R"){
        if(function3 == "000"){ // ADD operation
            arithmeticUnitOut_s4 = myRegister[registerSource1_s3] + myRegister[registerSource2_s3];
            // cout<<"Add performed : "<<arithmeticUnitOut_s4<<endl;
        } 
        else if(function3 == "001"){ // SLL operation
            arithmeticUnitOut_s4 = myRegister[registerSource1_s3] << (myRegister[registerSource2_s3] & 0x1F);
            // cout<<"SLL performed : "<<arithmeticUnitOut_s4<<endl;
        }
        else if(function3 == "010"){ // SLT operation
            arithmeticUnitOut_s4 = (myRegister[registerSource1_s3] * myRegister[registerSource2_s3]);
            // cout<<"MUL performed : "<<arithmeticUnitOut_s4<<endl;
        }
        else if(function3 == "011"){ // SUB operation
            arithmeticUnitOut_s4 = myRegister[registerSource1_s3] - myRegister[registerSource2_s3];
            // cout<<"SUB performed : "<<arithmeticUnitOut_s4<<endl;
        }
        else if(function3 == "100"){ // XOR operation
            arithmeticUnitOut_s4 = myRegister[registerSource1_s3] ^ myRegister[registerSource2_s3];
            // cout<<"XOR performed : "<<arithmeticUnitOut_s4<<endl;
        }
        else if(function3 == "101"){ // SRL operation
            arithmeticUnitOut_s4 = (unsigned)myRegister[registerSource1_s3] >> (myRegister[registerSource2_s3] & 0x1F);
            // cout<<"SRL performed : "<<arithmeticUnitOut_s4<<endl;
        }
        else if(function3 == "110"){ // OR operation
            arithmeticUnitOut_s4 = myRegister[registerSource1_s3] | myRegister[registerSource2_s3];
            // cout<<"OR performed : "<<arithmeticUnitOut_s4<<endl;
        }
        else if(function3 == "111"){ // AND operation
            arithmeticUnitOut_s4 = myRegister[registerSource1_s3] & myRegister[registerSource2_s3];
            // cout<<"AND performed : "<<arithmeticUnitOut_s4<<endl;
        }
    }

    
    else if(controlWord_s3 == "I"){ 
        if (function3 == "000") { // ADDI operation
            arithmeticUnitOut_s4 = myRegister[registerSource1_s3] + immediate;
            // cout<<"Addi performed : "<<arithmeticUnitOut_s4<<endl;
        } 
        else if(function3 == "001"){ // SLLI operation
            arithmeticUnitOut_s4 = myRegister[registerSource1_s3] << immediate;
            // cout<<"slli performed : "<<arithmeticUnitOut_s4<<endl;
        }
        else if(function3 == "100"){ // XORI operation
            arithmeticUnitOut_s4 = myRegister[registerSource1_s3] ^ immediate;
            // cout<<"xori performed : "<<arithmeticUnitOut_s4<<endl;
        }
        else if(function3 == "101"){ // SRLI operation
            arithmeticUnitOut_s4 = myRegister[registerSource1_s3] >> immediate;
            // cout<<"srli performed : "<<arithmeticUnitOut_s4<<endl;
        }
        else if(function3 == "110"){ // ORI operation
            arithmeticUnitOut_s4 = myRegister[registerSource1_s3] | immediate;
            // cout<<"ori performed : "<<arithmeticUnitOut_s4<<endl;
        }
        else if(function3 == "111"){ // ANDI operation
            arithmeticUnitOut_s4 = myRegister[registerSource1_s3] & immediate;
            // cout<<"Andi performed : "<<arithmeticUnitOut_s4<<endl;
        }
    } 

    else if(controlWord_s3 == "S" || controlWord_s3 == "L"){
        arithmeticUnitOut_s4 = myRegister[registerSource1_s3] + immediate;
        // cout<<"effective address calculated : "<<arithmeticUnitOut_s4<<endl;
    }

    else if(controlWord_s3 == "J"){ 

        arithmeticUnitOut_s4 = directProgramCounter_s3 + immediate;
        myRegister[registerDestination_s3] = programCounter;
        programCounter = arithmeticUnitOut_s4;
        if(bubbleInserted_control) stopForOneTime = true;
        bubbleInserted_control = false;
        // cout<<"control J bubble removed"<<endl;
        // cout<<"Jump executed.. pc : "<<programCounter<<endl;
    }

    else if(controlWord_s3 == "B") { 
        bool branchTaken = false;

        if(function3 == "000"){ // BEQ 
            branchTaken = (myRegister[registerSource1_s3]==myRegister[registerSource2_s3]);
        } 
        else if(function3 == "001"){ // BNE 
            branchTaken = (myRegister[registerSource1_s3]!=myRegister[registerSource2_s3]);
        } 
        else if(function3 == "100"){ // BLT 
            branchTaken = (myRegister[registerSource1_s3]<myRegister[registerSource2_s3]);
        } 
        else if(function3 == "101"){ // BGE 
            branchTaken = (myRegister[registerSource1_s3]>=myRegister[registerSource2_s3]);
        }
        else if(function3 == "110"){ //BLTU
            branchTaken = (myRegister[registerSource1_s3]<myRegister[registerSource2_s3]);
        }
        else if(function3 == "111"){ //BGEU
            branchTaken = (myRegister[registerSource1_s3]>=myRegister[registerSource2_s3]);
        }

        if(branchTaken){
            programCounter = directProgramCounter_s3 + immediate;
            // cout<<"branch taken .. pc : "<<programCounter<<endl;
        }
        if(bubbleInserted_control) stopForOneTime = true;
        bubbleInserted_control = false;
        // cout<<"control B bubble removed"<<endl;
    }

    controlWord_s4 = controlWord_s3;
    registerDestination_s4 = registerDestination_s3;
    registerSource2_s4 = registerSource2_s3;
    stage4Active = true;  

    // cout<<"stage3.. exit"<<endl;
}

void pipeline :: stage4() {
    
    if (!stage3Active) {
        stage4Active = false; // Deactivate if stage 3 is not active
        // cout<<"stage 4 set to false"<<endl;
        return;
    }
    // cout<<"stage4.. start"<<endl;
    if(controlWord_s4 == "L"){ 
        loadOut = myMemory[arithmeticUnitOut_s4];
        // cout<<"load performed : "<<loadOut<<endl;
    } 
    else if(controlWord_s4 == "S"){ 
        myMemory[arithmeticUnitOut_s4] = myRegister[registerSource2_s4];
        // cout<<"store performed : "<<myMemory[arithmeticUnitOut_s4] <<endl;
    }

    controlWord_s5 = controlWord_s4;
    arithmeticUnitOut_s5 = arithmeticUnitOut_s4;
    registerDestination_s5 = registerDestination_s4;
    stage5Active = true;  

    // cout<<"stage4.. exit"<<endl;
}

void pipeline :: stage5() {
    if (!stage4Active) {
        stage5Active = false; // Deactivate if stage 4 is not active
        // cout<<"stage 5 set to false"<<endl;
        return;
    }
    // cout<<"stage5.. start"<<endl;

    bool toUnlockRegister = false;
    if(controlWord_s5 == "R" || controlWord_s5 == "I"){ 
        myRegister[registerDestination_s5] = arithmeticUnitOut_s5;
        // cout<<"wb performed : "<<myRegister[registerDestination_s5]<<endl;

        if(registerLock[registerDestination_s5]==1) toUnlockRegister = true;
    } 
    else if(controlWord_s5 == "L"){ 
        myRegister[registerDestination_s5] = loadOut;
        // cout<<"wb performed : "<<myRegister[registerDestination_s5]<<endl;

        if(registerLock[registerDestination_s5]==1) toUnlockRegister = true;
    }

    if(toUnlockRegister) {
        bubbleInserted_data = false;
        stopForOneTime = true;
        registerLock[registerDestination_s5]=0;
        // cout<<"data bubble removed"<<endl;
    }

    // cout<<"stage5.. exit"<<endl;
    
}

int main(){
    string s1 = "setBit.s";
    string s2 = "setBit.o";

    myMemory[0]=5;

    assembler a(s1);
    pipeline p(s2);

    cout<<myMemory[1]<<endl;
}