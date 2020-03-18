#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <string>
#include <chrono>
#include <math.h>
using namespace std;

class TrieTree {

    const static int alphabet_size = 10;

    class TrieNode {
    public:
        TrieNode() : childrens(alphabet_size, nullptr), count(0){}

        std::vector<TrieNode*> childrens;
        int count;
    };

    const long long MOD;
    TrieNode* root;

    void insert(TrieNode *node, std::string &text, int pos) {

        if (pos >= text.size()){
            return;
        }

        if (node->childrens[text[pos]-'0'] == nullptr) {
            auto new_node = new TrieNode();
            node->childrens[text[pos]-'0'] = new_node;
        }

        node->childrens[text[pos]-'0']->count++;
        insert(node->childrens[text[pos]-'0'], text, pos+1);
    }

    int count_hash_value(TrieNode *node, std::string &text, int pos) {
        if (pos >= text.size() || node->count == 0 || node->childrens[text[pos]-'0'] == nullptr){
            return 1;
        }

        auto value = count_hash_value(node->childrens[text[pos]-'0'], text, pos+1);

        return (int)(((long long)value * (long long)node->childrens[text[pos]-'0']->count)%MOD);
    }

public:
    explicit TrieTree(int n) : root(new TrieNode()), MOD(n) {}

    void insert(std::string &text) {
        root->count++;
        insert(root, text, 0);
    }

    int count_hash_value(std::string text) {
        return count_hash_value(root, text, 0);
    }
};

class SegmentTree {

    std::set<int> uniques;
    std::vector<int> tree;
    std::map<int, int> mp;
    long long total_count = 0;

    int count_and_update(int l, int r, int pos, int loc) {

        if (loc < l) {
            return tree[pos];
        } else if (r < loc) {
            return 0;
        } else if (l == r) {
            if (loc == l) {
                tree[pos]++;
            }
            return 0;
        } else {
            auto count_l = count_and_update(l, (l+r)>>1, pos<<1, loc);
            auto count_r = count_and_update(((l+r)>>1)+1, r, (pos<<1)+1, loc);
            tree[pos] = tree[pos<<1] + tree[(pos<<1)+1];
            return count_l+count_r;
        }
    }

public:

    explicit SegmentTree(const std::vector<int> &hash_values) {

        for (int i = 0; i < hash_values.size(); i++) {
            uniques.insert(hash_values[i]);
        }

        tree.resize(uniques.size()*4);

        int i = 0;
        for (auto it = uniques.begin(); it != uniques.end(); it++) {
            mp[*it] = i++;
        }

        for (int i = 0; i < hash_values.size(); i++) {
            total_count += (long long)count_and_update(0, uniques.size(), 1, mp[hash_values[i]]);
        }
    }

    long long get_total_count() {
        return total_count;
    }
};


class WhuFileSystem {

    TrieTree *trieTree;
    std::vector<int> hash_values;
    long long total_num_moves;

public:

    explicit WhuFileSystem(int n, std::vector<std::string> &filenames) : trieTree(new TrieTree(n)), total_num_moves(0){

        for (auto filename : filenames) {
            auto hash_value = trieTree->count_hash_value(filename);
            hash_values.push_back(hash_value);
            trieTree->insert(filename);
        }

        SegmentTree segmentTree(hash_values);
        total_num_moves = segmentTree.get_total_count();
    }

    long long get_num_moves() {
        return total_num_moves;
    }
};


int main() {

    freopen("D:/input.txt", "r", stdin);
    freopen("D:/output.txt", "w", stdout);

    int n;
    while (cin >> n) {
        char c;
        cin.get(c);
        std::vector<std::string> filenames(n);
        for (int i = 0; i < n; i++) {
            std::getline(cin, filenames[i]);
        }

        WhuFileSystem whuFileSystem(n, filenames);
        std::cout << whuFileSystem.get_num_moves() << std::endl;
    }

    return 0;
}