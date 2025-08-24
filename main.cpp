#include <iostream>
#include <vector>
using namespace std;

void countingSort(vector<int>& arr) {
    if (arr.empty()) return;

    // Find the maximum element in the array
    int maxElement = *max_element(arr.begin(), arr.end());

    // Create count array and initialize with 0
    vector<int> count(maxElement + 1, 0);

    // Store the count of each element
    for (int num : arr) {
        count[num]++;
    }

    // Reconstruct the sorted array
    int index = 0;
    for (int i = 0; i <= maxElement; i++) {
        while (count[i] > 0) {
            arr[index++] = i;
            count[i]--;
        }
    }
}

int main() {
    vector<int> arr = {4, 2, 2, 8, 3, 3, 1};

    cout << "Original array: ";
    for (int num : arr) cout << num << " ";
    cout << endl;

    countingSort(arr);

    cout << "Sorted array: ";
    for (int num : arr) cout << num << " ";
    cout << endl;

    return 0;
}
