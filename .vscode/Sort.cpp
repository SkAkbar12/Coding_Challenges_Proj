#include <iostream>
#include <vector>

using namespace std;

class Sort {
public:
    static vector<int> BubbleSort(vector<int> a) {
        int n = a.size();
        bool swapped;
        
        for (int i = 0; i < n - 1; i++) {
            swapped = false;
            for (int j = 0; j < n - i - 1; j++) {
                if (a[j] > a[j + 1]) {  // Changed to > for ascending order
                    swap(a[j], a[j + 1]);
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
        return a;
    }

    static vector<int> InsertionSort(vector<int> a) {
        int n=a.size();
        for(int i=1;i<n;i++){
            int val=a[i];
            int j=i-1;
            while(j>=0 and a[j]<val){
                a[j+1]=a[j];
                j--;
            }
            a[j+1]=val;

        }
        return a;
    }
};

int main() {
    vector<int> arr = {64, 34, 25, 12, 22, 11, 90};
    
    cout << "Original array: ";
    for (int num : arr) cout << num << " ";
    cout << endl;
    
    cout << "Bubble Sorted (ascending): ";
    vector<int> bubbleSorted = Sort::BubbleSort(arr);
    for (int num : bubbleSorted) cout << num << " ";
    cout << endl;
    
    cout << "Insertion Sorted (ascending): ";
    vector<int> insertionSorted = Sort::InsertionSort(arr);
    for (int num : insertionSorted) cout << num << " ";
    cout << endl;
    
    return 0;
}