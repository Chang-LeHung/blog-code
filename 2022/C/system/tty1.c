
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
  printf("pid = %d pgid = %d\n", getpid(), getpgrp());
  pid_t pid = getpid();
  fork();
  fork();
  if(pid + 2 == getpid())
  {
    int fd = open("/dev/tty", O_RDWR);
    ioctl(fd, TIOCNOTTY, NULL);
    // setpgid(getpid(), 0);
    printf("pid = %d pgid = %d\n", getpid(), getpgrp());
  }
  while(1);
  return 0;
}