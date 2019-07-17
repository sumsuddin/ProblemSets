#include <iostream>
#include <fstream>
#include <random>
using namespace std;

int main() {

    string inputFile = "D:/input.txt";

    ofstream ofs(inputFile);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1000000000);

    for (int i = 0; i < 5; i++) {
        ofs << 10000 << " " << 100000 << endl;

        ofs << dis(gen);
        for (int j = 1; j < 100000; j++) {
            ofs << " " << dis(gen);
        }
        ofs << endl;
    }
    ofs.close();

    return 0;
}