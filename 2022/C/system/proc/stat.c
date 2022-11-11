



#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


void get_info_from_unistd_api() {
    printf("pid   = %d\n", getpid());
    printf("ppid  = %d\n", getppid());
    printf("pgrep = %d\n", getpgrp());
    printf("sid   = %d\n", getsid(0));
}

int main(int argc, char **argv) {
    get_info_from_unistd_api();
    return 0;
}