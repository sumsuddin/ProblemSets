//
// Created by SHOJIB on 29-Dec-19.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <math.h>
using namespace std;

int main() {

    freopen("D:/input.txt", "r", stdin);
    freopen("D:/output_brute.txt", "w", stdout);

    int n;
    while (cin >> n) {
        char c;
        cin.get(c);
        std::vector<std::string> filenames(n);
        for (int i = 0; i < n; i++) {
            std::getline(cin, filenames[i]);
        }

        std::vector<int> values;
        for (int i = 0; i < filenames.size(); i++) {

            int ans = 1;
            for (int j = 1; j <= filenames[i].size(); j++) {
                int count = 0;

                for (int k = 0; k < i; k++) {
                    if (filenames[i].substr(0, j) == filenames[k].substr(0, j)) {
                        count++;
                    }
                }
                if (count == 0) {
                    break;
                }

                ans = (ans*count)%filenames.size();
            }

            values.push_back(ans);
        }

        std::vector<int> sorted;
        long long ans = 0;
        for (int i = 0; i < values.size(); i++) {
            auto sz = sorted.size();
            if (sz > 0 && sorted[sz-1] > values[i]) {
                sorted.push_back(sorted[sz-1]);
            } else {
                sorted.push_back(values[i]);
                continue;
            }
            int count = 0;
            int j = sz-1;
            for (; j >= 0; j--) {
                if (values[i] < sorted[j]) {
                    sorted[j+1] = sorted[j];
                    count++;
                } else {
                    break;
                }
            }
            sorted[j+1] = values[i];

            ans += (long long)count;
        }

        std::cout << ans << std::endl;
    }
    return 0;
}