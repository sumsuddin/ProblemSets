//
// Created by SHOJIB on 13-Nov-19.
//

#include <iostream>
#include <fstream>
#include <random>
#include <map>
using namespace std;

ofstream ofs("D:/input.txt");
std::random_device rd; // obtain a random number from hardware
std::mt19937 eng(rd()); // seed the generator
std::uniform_int_distribution<> distr(1, INT32_MAX); // define the range

long long get_random_long_long() {
    int random = distr(eng);
    int random2 = distr(eng);

    return (long long)random * (long long)random2;
}

int main() {

    long long max_lim = 1000000000;
    int n = 100000;

    ofstream ofs("D:/input.txt");
    ofs << n << endl;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 16; j++) {
            ofs << get_random_long_long()%10;
        }
        ofs << endl;
    }

    return 0;
}