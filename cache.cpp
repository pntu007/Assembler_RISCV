#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <ctime>
#include <random>

using namespace std;

class Cache {
private:
    enum State { INVALID, VALID, MISS_PENDING };

    struct Block {
        long long tag = -1;
        State state = INVALID;
        long long data[16];
    };

    struct addressLocation {
        long long tag;
        long long set;
        long long offset;
    };

    long long cacheSize = 32768; // 32KB
    long long blockSize = 64; // 64 bytes
    long long associativity = 8; // 8-way associative
    long long numSets = cacheSize / (blockSize * associativity); // 64 sets
    vector<vector<Block>> sets; // 2D vector for each set with 8 blocks

    // Persistent random generator and distribution setup
    random_device rd;
    mt19937 gen;
    normal_distribution<double> dist;

    addressLocation getLocationInfo(long long address) {
        string inBinary(40, '0');
        long long i = 40 - 1;
        long long num = address;
        while (num != 0 && i >= 0) {
            inBinary[i] = '0' + (num % 2);
            num /= 2;
            i--;
        }
        addressLocation temp;
        temp.tag = stoi(inBinary.substr(0, 28), nullptr, 2);
        temp.set = stoi(inBinary.substr(28, 6), nullptr, 2);
        temp.offset = stoi(inBinary.substr(34, 6), nullptr, 2);

        return temp;
    }

    long long getNewLocation(long long set) {
        return rand() % associativity;
    }

    vector<long long> getDataFromMainMemory(long long req) {
        req /= 4;
        long long start = (req / 16) * 16;
        vector<long long> temp;
        for (long long i = 0; i < 16; i++) {
            temp.push_back(start + i);
        }
        return temp;
    }

    long long generate_normal_random_address(long long mean, long long stddev) {
        dist = normal_distribution<double>(mean, stddev);
        long long result;
        do {
            result = (dist(gen));
        } while (result < 0);

        return result * 4;
    }

public:
    Cache() : sets(numSets, vector<Block>(associativity)), gen(rd()), dist(10000, 3000) {}

    void handleRandomRequests(){
        long long hit=0,miss=0;
        long long numOfRequest = 10000000; 
        long long toPrint = numOfRequest/100; 
        
        cout<<"[";
        while(numOfRequest--){
            if(numOfRequest%toPrint==0) cout<<"#";
            if(numOfRequest==0) cout<<"]";
            long long request = (rand()%20000)*4;
            long long type = rand()%2;

            addressLocation req = getLocationInfo(request);

            bool foundInCache = false;
            for(long long index = 0; index < associativity; index++){
                Block& block = sets[req.set][index];
                if(block.tag == req.tag && block.state == VALID){
                    hit++;
                    // HIT
                    if(type==0){
                        // Read : data at location block.data[req.offset/4]
                    } 
                    else{
                        // Write :  Update cache block at block.data[req.offset/4] and main memory 
                    }
                    foundInCache = true;
                    break;
                }
            }

            if(foundInCache) continue;
            miss++;
            //MISS
            if(type==0){
                long long pos = getNewLocation(req.set);
                Block& block = sets[req.set][pos];
                block.state = MISS_PENDING;

                block.tag = req.tag;
                block.state = VALID;
                // Read : Data at location block.data[req.offset/4]
            }
            else{
                // Write : Updating Main Memory
            }
        }
        cout<<endl;
        cout<<"Hit rate  : "<<(hit*1.0/100000)<<endl;
        cout<<"Miss rate : "<<(miss*1.0/100000)<<endl;
    }

    void handleRandomRequests2(){
        long long hit = 0, miss = 0;
        long long numOfRequest = 10000000;
        long long toPrint = numOfRequest / 100;

        // Parameters for normal distribution
        long long mean = (1LL<<38);
        long long stddev = 5000;

        cout<<"[";
        while(numOfRequest--){
            if(numOfRequest%toPrint==0) cout<<"#";
            if(numOfRequest==0) cout<<"]";

            long long request = generate_normal_random_address(mean, stddev);
            long long type = rand() % 2;
            // cout<<request<<" ";

            addressLocation req = getLocationInfo(request);
            bool foundInCache = false;

            for(long long index = 0; index < associativity; index++){
                Block& block = sets[req.set][index];
                if(block.tag == req.tag && block.state == VALID){
                    hit++;
                    //HIT
                    if(type==0){
                        // Read
                    }
                    else {
                        // Write
                    }
                    foundInCache = true;
                    break;
                }
            }

            if(foundInCache) continue;
            miss++;
            //MISS
            if(type==0){
                //Read
                long long pos = getNewLocation(req.set);
                Block& block = sets[req.set][pos];
                block.state = MISS_PENDING;

                block.tag = req.tag;
                block.state = VALID;
            }
            else{
                // Write : Update the main memory
            }
        }

        cout<<endl;
        cout<<"Hit rate  : "<<(hit*1.0/100000)<<endl;
        cout<<"Miss rate : "<<(miss*1.0/100000)<<endl;
    }
};

int main() {
    srand(time(nullptr));
    Cache cache;
    cache.handleRandomRequests2();
    return 0;
}