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

        // tets1
        // "00100000010010010000100100010011",
        // "00000000000110010111111000010011",
        // "00000001110010011000100110110011",
        // "00000000000110010101100100010011",
        // "00000000000010010000000101100011",
        // "11111111110111111111001011101111"

        //test2
        // "01100100000101001100100100110111",
        // "01100100000011011111100110110111",
        // "00000000110010011101100110010011",
        // "00000000110010010101100100010011",
        // "00000001001110010000101110110011"
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