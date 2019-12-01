#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <assert.h>
#include <math.h>
#include <chrono>
using namespace std;

const int NUM_MAX_KINGDOM = 100000;
const long long MAX_STRENGTH = 5000000000;

class PrimeUtil {

    vector<int> primes;

public:
    PrimeUtil(int max) {
        primes.push_back(2);
        vector<bool> sieve_mask(max);
        for (int i = 3; i <= max; i+=2) {
            if (!sieve_mask[i]) {
                primes.push_back(i);
                auto inc = i<<1;
                for (int j = i*3; j <= max; j+=inc) {
                    sieve_mask[j] = true;
                }
            }
        }
    }

    int num_prime_factors(long long number) {

        int num_pf = 0;
        for (auto prime : primes) {
            if (prime > sqrt(number) + 1) {
                break;
            }
            if (number && number%prime == 0) {
                num_pf++;
                while(number%prime == 0) {
                    number/=prime;
                }
            }
        }
        if (number>1) {
            num_pf++;
        }

        return num_pf;
    }
};

class Processor {

    PrimeUtil primeUtil;
    vector<long long> strengths;
    vector<long long> sorted_strengths;
    vector<int> sorted_strengths_num_factors;
    vector<vector<int>> max_len;
    long long threshold;

    int get_left_pos(long long value) {
        int l = 0;
        int r = sorted_strengths.size()-1;
        while (l < r) {
            int mid = (l+r)>>1;
            if (sorted_strengths[mid] > value) {
                r = mid-1;
            } else {
                if (sorted_strengths[mid+1] <= value) {
                    l = mid+1;
                } else {
                    return mid;
                }
            }
        }
        return l;
    }

    int get_right_pos(long long value) {
        int l = 0;
        int r = sorted_strengths.size()-1;

        while (l<r) {
            int mid = (l+r)>>1;

            if (sorted_strengths[mid] < value) {
                l = mid+1;
            } else {
                if (sorted_strengths[mid-1] >= value) {
                    r = mid-1;
                } else {
                    return mid;
                }
            }
        }
        return r;
    }

    int get_pos(long long value) {
        int l = 0;
        int r = sorted_strengths.size()-1;

        while (l < r) {
            int mid = (l+r)>>1;

            if (sorted_strengths[mid] > value) {
                r = mid-1;
            } else if (sorted_strengths[mid] < value) {
                l = mid+1;
            } else {
                return mid;
            }
        }
        return l;
    }

    int get_max_len(int l, int r, int pos, int l_pos, int r_pos, int ind) {
        if (l_pos <= l && r <= r_pos) {
            return max_len[pos][ind];
        } else if (l_pos > r || r_pos < l) {
            return INT32_MIN;
        } else {
            auto l_max = get_max_len(l, (l+r)>>1, pos<<1, l_pos, r_pos, ind);
            auto r_max = get_max_len(((l+r)>>1)+1, r, (pos<<1)+1, l_pos, r_pos, ind);
            return max(l_max, r_max);
        }
    }

    void update(int l, int r, int pos, int best_len, int ind, int loc) {
        if (l == r) {
            for (int i = 0; i < 10; i++) {
                if (ind != i && max_len[pos][i] < best_len) {
                    max_len[pos][i] = best_len;
                }
            }
        } else {
            if (loc <= (l+r)>>1) {
                update(l, (l+r)>>1, pos<<1, best_len, ind, loc);
            } else {
                update(((l+r)>>1)+1, r, (pos<<1)+1, best_len, ind, loc);
            }
            for (int i = 0; i < 10; i++) {
                max_len[pos][i] = max(max_len[pos << 1][i], max_len[(pos << 1) + 1][i]);
            }
        }
    }

    int process(long long current) {

        auto update_loc = get_pos(current);
        auto current_unique = sorted_strengths_num_factors[update_loc];
        auto loc_l = get_left_pos(current-threshold);
        auto loc_r = get_right_pos(current+threshold);

        auto left_max_len = get_max_len(0, sorted_strengths.size()-1, 1, 0, loc_l, current_unique-1);
        auto right_max_len = get_max_len(0, sorted_strengths.size()-1, 1, loc_r, sorted_strengths.size()-1, current_unique-1);
        auto max_len = max(left_max_len, right_max_len) + 1;
        update(0, sorted_strengths.size()-1, 1, max_len, current_unique-1, update_loc);

        return max_len;
    }

public:

    Processor(PrimeUtil &primeUtil1, vector<long long> &strengths, long long threshold) :
            primeUtil(primeUtil1), strengths(strengths), threshold(threshold) {
        std::set<long long> strength_set;
        for (auto i : strengths) {
            strength_set.insert(i);
        }

        sorted_strengths.push_back(INT64_MIN);
        sorted_strengths_num_factors.push_back(0);//has no impact, so set 0.
        for (auto i : strength_set) {
            sorted_strengths.push_back(i);
            sorted_strengths_num_factors.push_back(primeUtil.num_prime_factors(i));
        }
        sorted_strengths.push_back(INT64_MAX);
        sorted_strengths_num_factors.push_back(0);//has no impact, so set 0.

        max_len.resize(sorted_strengths.size() << 2, vector<int>(10, 0));
    }

    int solve() {
        int ans = 0;
        for (int i = 0; i < strengths.size(); i++) {
            auto max_len = process(strengths[i]);
            if (max_len > ans) {
                ans = max_len;
            }
        }
        return ans;
    }
};

int main() {

    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);

    auto t_start = std::chrono::high_resolution_clock::now();
    PrimeUtil primeUtil((int)sqrt(MAX_STRENGTH)+1);

    int n;
    long long threshold;
    while (cin >> n >> threshold) {
        assert(1 <= n && n <= NUM_MAX_KINGDOM);
        assert(1 <= threshold && threshold <= MAX_STRENGTH);
        vector<long long> strengths(n);

        for (int i = 0; i < n; i++) {
            cin >> strengths[i];
            assert(2 <= strengths[i] && strengths[i] <= MAX_STRENGTH);
        }

        Processor processor(primeUtil, strengths, threshold);

        cout << processor.solve() << endl;
    }

    auto t_end = std::chrono::high_resolution_clock::now();

    double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();
    //std::cout << elapsed_time_ms << endl;

    return 0;
}