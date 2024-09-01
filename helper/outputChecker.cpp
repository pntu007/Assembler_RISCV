#include<bits/stdc++.h>
using namespace std;
#define ll long long

int main(){
    // add x3, x4, x5
    // addi x3, x3, 10

    // for : 
    //     bne x3, x2 , done
    //     lw x2, 10(x3)
    //     jal x2, for

    // done : 
    //     sw x1, 13(x3)
    vector<string>correctOutput{
        "00000000010100100000000110110011",
        "00000000101000011000000110010011",
        "00000000001000011001000111100011",
        "00000000101000011010000100000011",
        "10000000000000000010000101101111",
        "00000000000100011010011010100011"
    };

    vector<string>obtainedOutput;
    string myText;
    ifstream MyReadFile("./../filename");
    while(getline(MyReadFile, myText)){
        obtainedOutput.push_back(myText);
    }
    MyReadFile.close();

    if(correctOutput.size()!=obtainedOutput.size()){
        cout<<"NUMBER OF INSTRUCTIONS NOT SAME"<<endl;
    }
    else{
        int n=obtainedOutput.size();
        for(int i=0;i<n;i++){
            if(obtainedOutput[i]==correctOutput[i]) cout<<i<<" is correct"<<endl;
            else {
                cout<<"***************************************************"<<endl;
                cout<<i<<" is not correct"<<endl;
                cout<<"WRONG   "<<obtainedOutput[i]<<endl;
                cout<<"CORRECT "<<correctOutput[i]<<endl;
                cout<<"***************************************************"<<endl;
            }
        }
    }


return 0;
}