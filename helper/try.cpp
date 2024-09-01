#include<bits/stdc++.h>
using namespace std;
#define ll long long

void checkInstructionFormat(string s,vector<string>&instructionSplit){
    string toCheck = "";
    for(int i = 1; i < instructionSplit.size(); i++) {
        toCheck += instructionSplit[i];
    }
    cout << toCheck << " ";
    
    int i = 0, j = 0;
    while (j < s.size() && i < toCheck.size()) {
        if (s[j] == 'x' && toCheck[i] == 'x') i++;
        else if (s[j] == '*') while (i < toCheck.size() && isdigit(toCheck[i])) i++;
        else if (s[j] == '#') while (i < toCheck.size() && isalpha(toCheck[i])) i++;
        else break;
        
        j++;
    }

    if (j != s.size() || i != toCheck.size()) cout << "Incorrect Instruction Format" << endl;
       
    else  cout << "Noice" << endl;
        
}


int main(){
    vector<string>vr{"add","x2","x3","x33"};
    vector<string>vi{"addi","x2","x3","34"};
    vector<string>vl{"lw","x2","100","x34"};
    vector<string>vs{"sw","x2","754","x34"};
    vector<string>vj{"jal","x2","jump"};
    vector<string>vb{"beq","x2","x3","branck"};
    vector<string>vu{"lui","x2","10"};

    checkInstructionFormat("x*x*x*",vr);
    checkInstructionFormat("x*x*",vi);
    checkInstructionFormat("x*x*",vl);
    checkInstructionFormat("x*x*",vs);
    checkInstructionFormat("x*#",vj);
    checkInstructionFormat("x*x*#",vb);
    checkInstructionFormat("x*",vu);


return 0;
}