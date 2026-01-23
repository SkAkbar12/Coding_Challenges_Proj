#include <iostream>
#include <vector>

using namespace std;


class MergeSort{
public:
    static void divideAndConquer(vector<int>& a,int s,int e){
        if(s<e){
            int mid=s+(e-s)/2;
            divideAndConquer(a,s,mid);
            divideAndConquer(a,mid+1,e);
            merge(a,s,mid,e);
        }
    }

    static void merge(vector<int>& a,int s,int mid,int e){
        int i=s,j=mid+1;
        vector<int> t;
        while(i<=mid && j<=e){
            if(a[i]<=a[j]){
                t.push_back(a[i]);
                i++;
            }else{
                t.push_back(a[j]);
                j++;
            }
        }
        while(i<=mid){
            t.push_back(a[i++]);
        }
        while(j<=e){
            t.push_back(a[j++]);
        }
        for(int k=0;k<t.size();k++){
            a[k+s]=t[k];
        }
    }
};
int main(){
    vector<int> a={10,20,70,11,9,8,50};
    MergeSort::divideAndConquer(a,0,a.size()-1);
    for(int num:a) cout<<num<<" ";
    return 0;
}