//
// Created by SHOJIB on 13-Nov-19.
//

#include <iostream>
#include <fstream>
#include <random>
using namespace std;

ofstream ofs("D:/input.txt");
long long max_strength = 5000000000;
std::random_device rd; // obtain a random number from hardware
std::mt19937 eng(rd()); // seed the generator
std::uniform_int_distribution<> distr(1, INT32_MAX); // define the range

long long get_random_long_long() {
    int random = distr(eng);
    int random2 = distr(eng);

    return (long long)random * (long long)random2;
}

int main() {

    for (int i = 0; i < 1; i++) {
        int n = 100000;
        int m = (distr(eng) % n) + 1;
        ofs << n << " " << m << endl;
        ofs << (get_random_long_long() % max_strength)+1LL;
        for (int j = 1; j < n; j++) {
            auto rand = (get_random_long_long() % max_strength)+1LL;
            if (rand < 2) {
                rand += 2;
            }
            ofs << " " << rand;
        }
        ofs << endl;
    }

    return 0;
}