
#include <stdio.h>

void func_a()
{
  void* p = __builtin_frame_address(1);
  printf("caller frame address = %p\n", p);
}


int main()
{
  void* p = __builtin_frame_address(0);
  printf("main frame address = %p\n", p);
  func_a();
  return 0;
}