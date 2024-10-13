#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>

using namespace std;

vector<int> myMemory(20, 0);
vector<int> myRegister(32, 0);

unordered_map<string, char> m{
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
    int airthmeticUnitOut;

    // Stage 5 variables
    int loadOut;

    void openFile(string s){
        string myText;
        ifstream MyReadFile("./testing/" + s);
        while (getline(MyReadFile, myText)) {
            cout<<"instruction : "<<myText<<endl;
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
            if(temp[0]=='1') value-= (1 << 5);
            return value;
        }
        else{
            string temp = s.substr(0,20);
            int value = stoi(temp,NULL,2);
            if(temp[0]=='1') value-=(1 << 20);
            return value;
        }
    }

public:
    pipeline(string a){
        cout<<"file name "<<a<<endl; 
        openFile(a);
        while(programCounter < inputFile.size()){
            stage1();
            stage2();
            stage3();
            stage4();
            stage5();
        }

    }

    void stage1(); // Fetch stage (PC)
    void stage2(); // Decode and Immediate generation stage (DPC, NPC, I)
    void stage3(); // Execution stage (JPC, DPC, IMM, CW, RS1, RS2, IMM, F3, RD)
    void stage4(); // Memory access stage (CW, ALUOUT, RS2, RD)
    void stage5(); // Write-back stage (CW, LDOUT, ALUOUT, RD)
};

void pipeline :: stage1() {
    cout<<"stage1.. start"<<endl;
    instruction = inputFile[programCounter];
    cout<<"stage1.. exit"<<endl;
}

void pipeline :: stage2() {

    cout<<"stage2.. start"<<endl;
    char type = m[instruction.substr(25, 7)]; // opcode for cw
    registerDestination = stoi(instruction.substr(20, 5), NULL, 2);
    function3 = instruction.substr(17, 3);
    registerSource1 = stoi(instruction.substr(12, 5), NULL, 2);
    registerSource2 = stoi(instruction.substr(7, 5), NULL, 2);
    immediate = getImmdiate(type,instruction);

    controlWord = string(1,type);

    cout<<"Type "<<type<<endl;
    cout<<"rd "<<registerDestination<<endl;
    cout<<"f3 "<<function3<<endl;
    cout<<"rs2 "<<registerSource2<<endl;
    cout<<"rs1 "<<registerSource1<<endl;
    cout<<"imm "<<immediate<<endl;

    cout<<"stage2.. exit"<<endl;

}

void pipeline :: stage3() {
    cout<<"stage3.. start"<<endl;
    if(controlWord == "R"){
        if(function3 == "000"){ // ADD operation
            airthmeticUnitOut = myRegister[registerSource1] + myRegister[registerSource2];
            cout<<"Add performed : "<<airthmeticUnitOut<<endl;
        } 
        else if(function3 == "001"){ // SLL operation
            airthmeticUnitOut = myRegister[registerSource1] << (myRegister[registerSource2] & 0x1F);
            cout<<"SLL performed : "<<airthmeticUnitOut<<endl;
        }
        else if(function3 == "010"){ // SLT operation
            airthmeticUnitOut = (myRegister[registerSource1] < myRegister[registerSource2]) ? 1 : 0;
            cout<<"SLT performed : "<<airthmeticUnitOut<<endl;
        }
        else if(function3 == "011"){ // SUB operation
            airthmeticUnitOut = myRegister[registerSource1] - myRegister[registerSource2];
            cout<<"SUB performed : "<<airthmeticUnitOut<<endl;
        }
        else if(function3 == "100"){ // XOR operation
            airthmeticUnitOut = myRegister[registerSource1] ^ myRegister[registerSource2];
            cout<<"XOR performed : "<<airthmeticUnitOut<<endl;
        }
        else if(function3 == "101"){ // SRL operation
            airthmeticUnitOut = (unsigned)myRegister[registerSource1] >> (myRegister[registerSource2] & 0x1F);
            cout<<"SRL performed : "<<airthmeticUnitOut<<endl;
        }
        else if(function3 == "110"){ // OR operation
            airthmeticUnitOut = myRegister[registerSource1] | myRegister[registerSource2];
            cout<<"OR performed : "<<airthmeticUnitOut<<endl;
        }
        else if(function3 == "111"){ // AND operation
            airthmeticUnitOut = myRegister[registerSource1] & myRegister[registerSource2];
            cout<<"AND performed : "<<airthmeticUnitOut<<endl;
        }
    }

    
    else if(controlWord == "I"){ 
        if (function3 == "000") { // ADDI operation
            airthmeticUnitOut = myRegister[registerSource1] + immediate;
            cout<<"Addi performed : "<<airthmeticUnitOut<<endl;
        } 
        else if(function3 == "001"){ // SLLI operation
            airthmeticUnitOut = myRegister[registerSource1] ^ immediate;
            cout<<"slli performed : "<<airthmeticUnitOut<<endl;
        }
        else if(function3 == "100"){ // XORI operation
            airthmeticUnitOut = myRegister[registerSource1] ^ immediate;
            cout<<"xori performed : "<<airthmeticUnitOut<<endl;
        }
        else if(function3 == "101"){ // SRLI operation
            airthmeticUnitOut = myRegister[registerSource1] ^ immediate;
            cout<<"srli performed : "<<airthmeticUnitOut<<endl;
        }
        else if(function3 == "110"){ // ORI operation
            airthmeticUnitOut = myRegister[registerSource1] ^ immediate;
            cout<<"ori performed : "<<airthmeticUnitOut<<endl;
        }
        else if(function3 == "111"){ // ANDI operation
            airthmeticUnitOut = myRegister[registerSource1] ^ immediate;
            cout<<"Andi performed : "<<airthmeticUnitOut<<endl;
        }
    } 

    else if(controlWord == "S" || controlWord == "L"){
        airthmeticUnitOut = myRegister[registerSource1] + immediate;
        cout<<"effective address calculated : "<<airthmeticUnitOut<<endl;
    }

    else if(controlWord == "J"){ 

        airthmeticUnitOut = programCounter + immediate;
        myRegister[registerDestination] = programCounter;
        programCounter = airthmeticUnitOut;
        isPcUpdated = true;
        cout<<"Jump executed.. pc : "<<programCounter<<endl;
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
            cout<<"branch taken .. pc : "<<programCounter<<endl;
        }
    }

    controlWord = controlWord;
    registerDestination = registerDestination;
    registerSource2 = registerSource2;
    cout<<"stage3.. exit"<<endl;
}

void pipeline :: stage4() {
    cout<<"stage4.. start"<<endl;
    if(controlWord == "L"){ 
        loadOut = myMemory[airthmeticUnitOut];
        cout<<"load performed : "<<loadOut<<endl;
    } 
    else if(controlWord == "S"){ 
        myMemory[airthmeticUnitOut] = myRegister[registerSource2];
        cout<<"store performed : "<<myMemory[airthmeticUnitOut] <<endl;
    }

    controlWord = controlWord;
    airthmeticUnitOut = airthmeticUnitOut;
    registerDestination = registerDestination;

    cout<<"stage4.. exit"<<endl;
}

void pipeline :: stage5() {
    cout<<"stage5.. start"<<endl;
    if(controlWord == "R" || controlWord == "I"){ 
        myRegister[registerDestination] = airthmeticUnitOut;
        cout<<"wb performed : "<<myRegister[registerDestination]<<endl;
    } 
    else if(controlWord == "L"){ 
        myRegister[registerDestination] = loadOut;
        cout<<"wb performed : "<<myRegister[registerDestination]<<endl;
    }

    if(!isPcUpdated) programCounter++;
    isPcUpdated = false;
    cout<<"stage5.. exit"<<endl;
    
}

int main(){
    string s = "p.o";
    myMemory[0]=11;
    pipeline p(s);
    cout<<myMemory[1]<<endl;
    
}