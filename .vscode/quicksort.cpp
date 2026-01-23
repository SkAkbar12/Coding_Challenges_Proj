#include <iostream>
#include <vector>

using namespace std;

class QuickSort{
public:
    static void divideandconquer(vector<int>& a,int s,int e){
        if(s<e){
            int p=partitioning(a,s,e);
            divideandconquer(a,s,p-1);
            divideandconquer(a,p+1,e);
        }
    }
    static int partitioning(vector<int>& a,int s,int e){
        int idx=s-1;
        int p=a[e];
        for(int i=s;i<e;i++){
            if(a[i]<p){
                idx++;
                swap(a[i],a[idx]);
            }
        }
        idx++;
        swap(a[idx],a[e]);
        return idx;
    }
};
int main(){
    vector<int> a={10,40,20,9,8,6,30,90};
    QuickSort::divideandconquer(a,0,a.size()-1);
    for(int n:a) cout<<n<<" ";
    return 0;
}