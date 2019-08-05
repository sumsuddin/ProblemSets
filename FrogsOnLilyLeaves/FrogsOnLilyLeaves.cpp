#include <iostream>
#include <vector>
#include <assert.h>
#include <chrono>

using namespace std;

const long long MOD = 1000000007LL;

class Solution {

    int n;
    vector<long long> tree;

    long long gcd(long long a, long long b) {
        if (a%b == 0) {
            return b;
        } else {
            return gcd(b, a%b);
        }
    }

    int nthCommonLeaf(int N, int A, int B) {
        if (A > B) {
            int c = A;
            A = B;
            B = c;
        }

        long long g = ((long long)A*(long long)B)/gcd(A, B);

        long long l = A;
        long long h = (long long)B * (long long)N;

        while (l<h) {
            long long mid = (l+h)/2LL;
            long long val = mid / A + mid / B - mid / g;
            if (val < N) {
                l = mid+1LL;
            } else if (val > N){
                h = mid-1LL;
            } else {
                if (mid % A == 0 || mid % B == 0){
                    return (int)(mid % MOD);
                } else {
                    h = mid-1LL;
                }
            }
        }
        return (int)(l % MOD);
    }

    void buildTree(int l, int r, int pos) {
        tree[pos] = 0;
        if (l != r) {
            buildTree(l, (l+r)/2, pos*2);
            buildTree((l+r)/2+1, r, pos*2+1);
        }
    }

    void addRange(int l, int r, int value, int posl, int posr, int pos) {

        if (l<= posl && posr <= r) {
            tree[pos] = (tree[pos]+ (long long)value)%MOD;
        } else if (l > posr || r < posl) {
            return;
        } else {
            addRange(l, r, value, posl, (posl + posr) / 2, pos * 2);
            addRange(l, r, value, (posl + posr) / 2 + 1, posr, pos * 2 + 1);
        }
    }

    int getAt(int loc, int posl, int posr, int pos) {
        if (posl == posr) {
            return tree[pos];
        } else {

            tree[pos*2] = (tree[pos*2] + tree[pos])%MOD;
            tree[pos*2+1] = (tree[pos*2+1] + tree[pos])%MOD;
            tree[pos] = 0LL;

            if (loc <= (posl + posr) / 2) {
                return getAt(loc, posl, (posl + posr) / 2, pos * 2);
            } else {
                return getAt(loc, (posl + posr) / 2 + 1, posr, pos * 2 + 1);
            }
        }
    }

public:

    Solution(int n) : tree(4*n, 0LL) {
        this->n = n;
        buildTree(1, n, 1);
    }

    void calculateSeeds(int a, int b, int c) {
        int ans = nthCommonLeaf(c, a, b);
        addRange(a, b, ans, 1, n, 1);
    }

    int getAt(int pos) {
        return getAt(pos, 1, n, 1);
    }
};

int main() {

    freopen("D:/input.txt", "r", stdin);
    freopen("D:/output.txt", "w", stdout);

    int d, n;
    auto t_start = std::chrono::high_resolution_clock::now();
    while (cin >> n >> d) {
        assert(1 <= n && n <= 100000 && 1 <= d && d <= 100000);
        Solution soln(n);
        while (d--) {
            int a, b, c;
            cin >> a >> b >> c;
            assert(a <= b && 1 <= a && a <= 100000 && 1 <= b && b <= 100000);
            assert(1 <= c && c <= 1000000000);
            soln.calculateSeeds(a, b, c);
        }

        for (int i = 1; i < n; i++) {
            cout << soln.getAt(i) << " ";
        }
        cout << soln.getAt(n) << endl;
    }
    auto t_end = std::chrono::high_resolution_clock::now();

    double elaspedTimeMs = std::chrono::duration<double, std::milli>(t_end-t_start).count();
    //std::cout << elaspedTimeMs << std::endl;

    return 0;
}