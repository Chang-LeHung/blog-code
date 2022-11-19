
#include <stdio.h>

void func_a()
{
  void* p        = __builtin_return_address(0);
  void* rbp      = __builtin_frame_address(0);
  void* last_rbp = __builtin_frame_address(1);
  asm volatile(
    "leaq 16(%1), %%rsp;"
    "movq %2, %%rbp;"
    "jmp *%0;"::"r"(p), "r"(rbp), "r"(last_rbp):
  );
  printf("finished in func_a\n");
}


int main()
{
  void* p = __builtin_return_address(0);
  printf("main return address = %p\n", p);
  func_a();
  printf("finished in main function \n");
  int i, j;
  for(i = 1; i < 10; ++i) 
  {
    for(j = 1; j <= i; ++j) {
      printf("%d x %d = %d\t", i, j, i * j);
    }
    printf("\n");
  }
  return 0;
}