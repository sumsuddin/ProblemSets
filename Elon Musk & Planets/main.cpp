#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <math.h>
using namespace std;

class PlanetManager {

    int base_max;
    int power_max;

    vector<int> unique_masses;
    vector<int> encountered;
    vector<int> power_count;
    vector<int> power_count_lazy;

    void build(int l, int r, int pos) {
        power_count[pos] = 0;
        power_count_lazy[pos] = 0;
        encountered[pos] = 0;

        if (l == r) {
            return;
        } else {
            build(l, (l+r)>>1, pos<<1);
            build(((l+r)>>1)+1, r, (pos<<1)+1);
        }
    }

    void propagate_down(int &pos) {
        power_count[pos<<1] += power_count[pos];
        power_count[(pos<<1)+1] += power_count[pos];
        power_count[pos] = 0;

        power_count_lazy[pos<<1] += power_count_lazy[pos];
        power_count_lazy[(pos<<1)+1] += power_count_lazy[pos];
        power_count[(pos<<1)+1] += encountered[pos<<1] * power_count_lazy[pos];
        power_count_lazy[pos] = 0;
    }

    void update(int l, int r, int pos, int loc) {

        if (l == r) {
            encountered[pos] = 1;
            power_count[pos] = 1;
            power_count_lazy[pos] = 0;
            return;
        }

        propagate_down(pos);

        if (((l+r)>>1) >= loc) {
            update(l, (l+r)>>1, pos<<1, loc);
        } else {
            update(((l+r)>>1)+1, r, (pos<<1)+1, loc);
        }

        encountered[pos] = encountered[pos<<1] + encountered[(pos<<1)+1];
    }

    long long calculate_cov(int val) {
        long long tmp = (long long)val;
        return (val * (val+1LL))>>1;
    }

    int search_max_coverage(int mass) {
        int l = 0;
        int r = mass;
        while( l < r) {
            int mid = (l+r)>>1;

            if (calculate_cov(mid+1) <= (long long)mass) {
                l = mid+1;
            } else if (calculate_cov(mid) <= (long long)mass) {
                return mid;
            } else {
                r = mid-1;
            }
        }
        return l;
    }

    int update_lazy(int l, int r, int pos, int loc_l, int loc_r, int coverage, int gross_inc) {

        if (loc_l <= l && r <= loc_r && encountered[pos] <= coverage) {
            power_count_lazy[pos] += 1;
            power_count[pos] += gross_inc;
            return encountered[pos];
        } else if (loc_l > r || l > loc_r || coverage == 0) {
            return 0;
        } else {
            int left_coverage = update_lazy(l, (l+r)>>1, pos<<1, loc_l, loc_r, coverage, gross_inc);
            if (left_coverage < coverage) {
                return left_coverage +
                       update_lazy(((l+r)>>1)+1,
                                   r,
                                   (pos<<1)+1,
                                   loc_l, loc_r,
                                   coverage-left_coverage,
                                   gross_inc+left_coverage);
            } else {
                return left_coverage;
            }
        }
    }

    int search(int mass) {
        int l = 0;
        int r = unique_masses.size()-1;

        while (l < r) {
            auto mid = (l+r)>>1;

            if (unique_masses[mid] > mass) {
                r = mid-1;
            } else if (unique_masses[mid] < mass){
                l = mid+1;
            } else {
                return mid;
            }
        }
        return l;
    }

    void process(vector<int> &masses) {
        for (int mass : masses) {
            auto loc = search(mass);
            auto coverage = search_max_coverage(mass);
            update_lazy(0, unique_masses.size() - 1, 1, 0, loc - 1, coverage, 0);
            update(0, unique_masses.size()-1, 1, loc);
        }
    }

    int query(int l, int r, int pos, int loc) {
        if (l == r) {
            return power_count[pos] + power_count_lazy[pos];
        } else {
            propagate_down(pos);

            if ((l+r)>>1 >= loc) {
                return query(l, (l+r)>>1, pos<<1, loc);
            } else {
                return query(((l+r)>>1)+1, r, (pos<<1)+1, loc);
            }
        }
    }

    bool is_greater(int a, int b, int x, int y) {
        return (double)y*log(x) < (double)b*log(a);
    }

public:

    explicit PlanetManager(vector<int> masses)
            : unique_masses(masses),
              encountered(masses.size()*4),
              power_count_lazy(masses.size()*4),
              power_count(masses.size()*4) {

        sort(unique_masses.begin(), unique_masses.end());
        build(0, masses.size()-1, 1);
        process(masses);

        base_max = unique_masses[0];
        power_max = query(0, unique_masses.size()-1, 1, 0);
        //cout << unique_masses[0] << " " << power_max << endl;
        for (int i = 1; i < unique_masses.size(); i++) {
            int now = query(0, unique_masses.size()-1, 1, i);
            if (is_greater(unique_masses[i], now, base_max, power_max)) {
                base_max = unique_masses[i];
                power_max = now;
            }
            //cout << unique_masses[i] << " " << now << endl;
        }
    }


    int get_max_base() {
        return base_max;
    }

    int get_max_power() {
        return power_max;
    }
};

int main() {

    freopen("D:/input.txt", "r", stdin);
    freopen("D:/output.txt", "w", stdout);

    //auto t_start = std::chrono::high_resolution_clock::now();
    int t;

    cin >> t;
    while (t--) {

        int n;
        cin >> n;
        vector<int> M(n);

        for (int i = 0; i < n; i++) {
            cin >> M[i];
        }

        PlanetManager manager(M);
        cout << manager.get_max_base() << "^" << manager.get_max_power() << endl;
    }
    //auto t_end = std::chrono::high_resolution_clock::now();

    //double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();
    //std::cout << elapsed_time_ms << endl;

    return 0;
}