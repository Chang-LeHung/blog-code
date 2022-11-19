

#include <stdio.h>
#include <omp.h>
#include <unistd.h>

int main()
{

  omp_set_num_threads(2);
  #pragma omp parallel
  {
    printf("tid = %d\n", omp_get_thread_num());
    #pragma omp parallel
    {
      printf("Inner tid = %d\n", omp_get_thread_num());
    }
  }

  return 0;
}