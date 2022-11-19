

#include <stdio.h>

#define lowbit(x) (1 << (__builtin_ctz(x)))

int lowbit2(int x)
{
  return (x) & (-x);
}

int main()
{
  int i = 15;
  printf("bits = %d\n", __builtin_popcount(i));
  for(int i = 0; i < 100; ++i)
  {
    printf("macro = %d function = %d\n", lowbit(i), lowbit2(i));
  }
  return 0;
}