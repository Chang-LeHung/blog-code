
#include <stdio.h>
#include <ucontext.h>
#include <stdlib.h>

#define STACK_SIZE 1 << 20

void func()
{
  int i, j;
  for(i = 1; i < 10; ++i) 
  {
    for(j = 1; j <= i; ++j) {
      printf("%d x %d = %d\t", i, j, i * j);
    }
    printf("\n");
  }
}

int main()
{
  void* stack = malloc(STACK_SIZE);
  ucontext_t t, m;
  t.uc_link = &m;
  t.uc_stack.ss_sp = stack;
  t.uc_stack.ss_size = STACK_SIZE;
  getcontext(&t);
  makecontext(&t, func, 0);

  swapcontext(&m, &t);

  printf("finished\n");
  return 0;
}