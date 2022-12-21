
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
  int t;
  cin >> t;
  string s;
  vector<string> data;
  for(int i = 0; i < t; ++i)
  {
    cin >> s;
    data.push_back(s);
  }
  sort(data.begin(), data.end(), [](const string &a, const string &b){
    if(a.size() == b.size())
    {
      return b < a;
    }
    return b.size() < a.size();
  });
  for(auto & ss : data)
  {
    cout << ss << endl;
  }
  return 0;
}