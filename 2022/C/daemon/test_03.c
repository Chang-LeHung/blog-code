

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 20

int main()
{
  if(fork())
    exit(1);
  FILE* fp = fopen("test.txt", "w");
  while(1)
  {
    fprintf(fp, "%lu\n", time(NULL));
    printf("%lu\n", time(NULL));
    fflush(fp);
    rewind(fp);
    sleep(1);
  }

	int abs_fd;
	char *obj_file;
	obj_file = malloc(SIZE);
	char buf[SIZE] = {'\0'};
 
	snprintf(buf,sizeof(buf), "/proc/self/fd/%d", abs_fd);
 
	if (readlink(buf,obj_file,SIZE) < 0) 
	{
		perror("readlink() error \n");
		return 0;
	}
	
	printf("The absolute filepath is: \n%s \n",obj_file);
  return 0;
}