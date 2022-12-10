
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        int maxres=1;
        int n=nums.size();
        if(n==0) return 0;
        vector<int> res(n,1);
        for(int i=1;i<n;i++){
            for(int j=0;j<i;j++){
                if(nums[i]>nums[j]){
                    res[i]=max(res[i],res[j]+1);
                }
            }
            maxres=max(maxres,res[i]);
        }
        return maxres;
    }
};

int main()
{
  Solution s;
  int n;
  while(1)
  {
    cin >> n;
    vector<int> data;
    for(int i = 0; i < n; ++i)
    {
      int t;
      cin >> t;
      data.push_back(t);
    }
    cout << s.lengthOfLIS(data) << endl;
  }
  return 0;
}