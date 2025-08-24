#include <iostream>
#include <vector>
using namespace std;

class UpperandLower {
public:
    static vector<int> bounds(const vector<int>& a, int n, int tar) {
        int f = -1; // lower bound index
        int s = -1; // upper bound index

        int l = 0, h = n - 1;

        // Find lower bound (first occurrence)
        while (l <= h) {
            int mid = l + (h - l) / 2;
            if (a[mid] >= tar) {
                if (a[mid] == tar) f = mid;
                h = mid - 1; // keep searching left
            } else {
                l = mid + 1;
            }
        }

        // Find upper bound (last occurrence)
        l = 0; h = n - 1;
        while (l <= h) {
            int mid = l + (h - l) / 2;
            if (a[mid] <= tar) {
                if (a[mid] == tar) s = mid;
                l = mid + 1; // keep searching right
            } else {
                h = mid - 1;
            }
        }

        return {f, s}; // may be {-1, -1} if not found
    }
};

int main() {
    vector<int> arr = {2, 4, 4, 4, 6, 8, 10};
    int target = 4;

    vector<int> res = UpperandLower::bounds(arr, arr.size(), target);
    cout << "Lower Bound Index: " << res[0] << "\n";
    cout << "Upper Bound Index: " << res[1] << "\n";

    return 0;
}
