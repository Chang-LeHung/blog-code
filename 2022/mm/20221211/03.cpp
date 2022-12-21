
#include <iostream>
#include <vector>

using namespace std;

bool is_prime(int n) {
    if (n <= 1) return false;

    for (int i = 2; i*i <= n; i++) {
        if (n % i == 0) return false;
    }

    return true;
}

bool is_semi_prime(int n) {
    if (n <= 1) return false;

    for (int i = 2; i*i <= n; i++) {
        if (n % i == 0 && is_prime(i) && is_prime(n / i)) return true;
    }

    return false;
}


int main()
{

    int i;
  while(1)
  {
    cin >> i;
    if(is_semi_prime(i))
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