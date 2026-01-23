#include <iostream>
#include <vector>
using namespace std;

class Search {
public:
    static int BinarySearch(const vector<int>& arr, int tar) {
        int l = 0, r = arr.size() - 1;

        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (arr[mid] == tar) return mid;
            else if (arr[mid] < tar) l = mid + 1;
            else r = mid - 1;
        }

        return -1;
    }


    static int BinaryRecursive(const vector<int>& a,int tar,int l,int r,int m){
        int mid=l+(r-l)/2;
        if(l==r) return -1;
        if(a[mid]==tar) return mid;
        else if(a[mid]<tar) return BinaryRecursive(a,tar,mid+1,r,m);
        else return BinaryRecursive(a,tar,l,mid-1,m);
    }
};
int main() {
    vector<int> a = {1, 2, 4, 6, 8, 9, 12, 16, 18};
    //int i = Search::BinarySearch(a, 16);
    int i=Search::BinaryRecursive(a,16,0,a.size()-1,0);
    if (i == -1)
        cout << "Not Found" << endl;
    else
        cout << "Found at index " << i << endl;

    return 0;
}
