#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

const int MAX_SIZE = 100000;
const int MOD = 1000000007;


//****Start of****Max Min with index***************//

struct ValueInd {
    int value;
    int index;
};

struct Node {
    ValueInd minValInd;
    ValueInd maxValInd;
} tree[MAX_SIZE*4+50];

ValueInd getMin(ValueInd a, ValueInd b) {
    ValueInd ret;
    ret.value = min(a.value, b.value);
    if( (a.value == b.value && b.index < a.index) || b.value < a.value) {
        ret.index = b.index;
    } else {
        ret.index = a.index;
    }
    return ret;
}

ValueInd getMax(ValueInd a, ValueInd b) {
    ValueInd ret;
    ret.value = max(a.value, b.value);
    if( (a.value == b.value && b.index > a.index) || b.value > a.value) {
        ret.index = b.index;
    } else {
        ret.index = a.index;
    }
    return ret;
}

void build(int left, int right, int pos, vector<int> &weights) {

    if (left == right) {
        ValueInd minValInd;
        minValInd.value= weights[left];
        minValInd.index = left;
        tree[pos].minValInd = minValInd;
        tree[pos].maxValInd = minValInd;
    } else {
        build(left, (left+right)/2, pos<<1, weights);
        build((left+right)/2+1, right, (pos<<1)+1, weights);

        tree[pos].minValInd = getMin(tree[pos*2].minValInd, tree[pos*2+1].minValInd);
        tree[pos].maxValInd = getMax(tree[pos*2].maxValInd, tree[pos*2+1].maxValInd);
    }
}

int query(int left, int right, int pos, int ind) {
    if (left == right) {
        return tree[pos].minValInd.value;
    } else {
        if (ind <= (left + right)/2) {
            return query(left, (left+right)/2, pos*2, ind);
        } else {
            return query((left+right)/2+1, right, pos*2+1, ind);
        }
    }
}

void update(int left, int right, int pos, int ind, int val) {
    if (left == right) {
        ValueInd valueInd;
        valueInd.value = val;
        valueInd.index = left;
        tree[pos].minValInd = tree[pos].maxValInd = valueInd;
    } else {
        if (ind <= (left + right)/2) {
            update(left, (left+right)/2, pos*2, ind, val);
        } else {
            update((left+right)/2+1, right, pos*2+1, ind, val);
        }

        tree[pos].minValInd = getMin(tree[pos*2].minValInd, tree[pos*2+1].minValInd);
        tree[pos].maxValInd = getMax(tree[pos*2].maxValInd, tree[pos*2+1].maxValInd);
    }
}
//****End of****Max Min with index***************//

void copy(vector<vector<int>> &ret, vector<vector<int>> &val) {
    for (int i = 0; i < ret.size(); i++) {
        for (int j = 0; j < ret[i].size(); j++) {
            ret[i][j] = val[i][j];
        }
    }
}

void multiply(vector<vector<int>> &a, vector<vector<int>> &b, int MOD) {
    vector<vector<int>> ret(a.size(), vector<int>(b[0].size()));
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < b[0].size(); j++) {
            long long res = 0;
            for (int k = 0; k < a[i].size(); k++) {
                res = (res + ((long long)a[i][k] * (long long)b[k][j]) % MOD) % MOD;
            }
            ret[i][j] = (int)res;
        }
    }
    copy(a, ret);
}


void matrixExpo(vector<vector<int>> &ans, vector<vector<int>> &mat, int n, int MOD) {
    if (n == 1) {
        copy(ans, mat);
    } else {
        if (n&1) {
            matrixExpo(ans, mat, n-1, MOD);
            multiply(ans, mat, MOD);
        } else {
            matrixExpo(ans, mat, n>>1, MOD);
            multiply(ans, ans, MOD);
        }
    }
}

/*
 * f3 = f2 + 2*f1 + 1
 *
 *  1 2 1   f2   f3
 *  1 0 0 * f1 = f2
 *  0 0 1    1    1
 *
 * */
int optimizerFunction(int f1, int f2, int n, int MOD) {
    if (n == 1) {
        return f1 % MOD;
    } else if (n == 2) {
        return f2 % MOD;
    } else {
        vector<vector<int>> mat = {{1, 2, 1}, {1, 0, 0}, {0, 0, 1}};
        vector<vector<int>> multiplied(mat.size(), vector<int>(mat.size()));
        matrixExpo(multiplied, mat, n-2, MOD);
        int a = ((long long)multiplied[0][0] * (long long)f2) % MOD;
        int b = ((long long)multiplied[0][1] * (long long)f1) % MOD;
        int c = ((long long)a+(long long)b) % MOD;
        return ((long long)c + (long long)multiplied[0][2]) % MOD;
    }
}

void optimize(int n, vector<int> &weights) {
    auto minValInd = tree[1].minValInd;
    auto maxValInd = tree[1].maxValInd;

    int indU = optimizerFunction(minValInd.index, maxValInd.index, n, weights.size());
    int valU = optimizerFunction(minValInd.value, maxValInd.value, n, MOD);
    int weight = ((long long)query(0, weights.size()-1, 1, indU) + (long long)valU) % MOD;
    update(0, weights.size()-1, 1, indU, weight);
}

int main()
{
    auto t_start = std::chrono::high_resolution_clock::now();
    freopen("D:/input.txt", "r", stdin);
    freopen("D:/output.txt", "w", stdout);
    int n;
    int weightSize;
    vector<Node> tree;

    while (cin >> n >> weightSize) {
        vector<int> weights(weightSize);

        for (int i = 0; i < weightSize; i++) {
            cin >> weights[i];
        }

        build(0, weightSize-1, 1, weights);

        for (int i = 0; i < n; i++) {
            optimize(i+1, weights);
        }

        long long sum = 0;
        for (int i = 0; i < weightSize; i++) {
            sum += (long long)query(0, weightSize-1, 1, i);
        }
        cout << sum << endl;
    }
    auto t_end = std::chrono::high_resolution_clock::now();
    double elaspedTimeMs = std::chrono::duration<double, std::milli>(t_end-t_start).count();
    //cout << "Elapsed time: " << elaspedTimeMs << endl;
    return 0;
}