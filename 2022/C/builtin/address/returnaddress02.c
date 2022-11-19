

#include <stdio.h>
#include <sys/types.h>

#define return_address            \
    u_int64_t rbp;                \
    asm volatile(                 \
      "movq %%rbp, %%rcx;"        \
      "movq (%%rcx), %%rcx;"      \
      "movq %%rcx, %0;"           \
      :"=m"(rbp)::"rcx"           \
    );                            \
    printf("From inline assembly return address = %p\n", (u_int64_t*)*(u_int64_t*)(rbp + 8));

void func_a()
{
  printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
  void* p = __builtin_return_address(1);
  printf("fun_a return address = %p\n", p);
  return_address
  printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
}


int main()
{
  func_a();
  printf("main address = %p\n", main);
  return 0;
}