


#include <stdio.h>
#include <omp.h>
#include <stdint.h>
#include <unistd.h>

int i = 1;
int main() {
  omp_set_num_threads(2);
  #pragma omp parallel for
  for(int i = 0; i < 4; i++)
  {
    printf("i = %d tid = %d\n", i, omp_get_thread_num());
  }
  return 0;
}