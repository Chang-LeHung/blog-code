
#include <iostream>

using namespace std;

int fib(int n)
{
    if(n == 0)
    {
      return 7;
    }
    else if(n == 1)
    {
      return 11;
    }
    else
    {
        return fib(n-1) + fib(n-2);
    }
}

int main()
{
  int i;
  while(1)
  {
    cin >> i;
    if(fib(i) % 3 == 0)
    {
      cout << "yes\n";
    }
    else
    {
      cout << "no\n";
    }
  }
  return 0;
}