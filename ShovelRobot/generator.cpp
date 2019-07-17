#include <iostream>
#include <fstream>
using namespace std;

int main() {

    string inputFile = "D:/input.txt";

    ofstream ofs(inputFile);

    int n = 100000;
    ofs << n << " " << n << endl;
    for (int j = 0; j < n - 16; j++) {
        int op = rand() % 100;
        int l = (rand() % n) + 1;
        int h = (rand() % n) + 1;
        if (op < 50) {
            ofs << "S " << l << " " << h << endl;
        } else if (op < 80) {
            ofs << "H " << l << " " << h << endl;
        } else {

            if (l > h) {
                swap(l, h);
            }

            ofs << "A " << l << " " << h << endl;
        }
    }
    return 0;
}