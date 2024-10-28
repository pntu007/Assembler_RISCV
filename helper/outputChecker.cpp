#include<bits/stdc++.h>
using namespace std;
#define ll long long

int main(){

    vector<string>correctOutput{
        // filename
        "00000000010100100000000110110011",
        "00000000101000011000000110010011",
        "00000000001000011001000101100011",
        "00000000101000011010000100000011",
        "11111111111111111111000101101111",
        "00000000000100011010011010100011"
    };

    string readFileName = "./../testing/filename.o";

    vector<string>obtainedOutput;
    string myText;
    ifstream MyReadFile(readFileName);
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