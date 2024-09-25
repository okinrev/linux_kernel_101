#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sched.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <unistd.h>

#define DEV_NAME "/dev/kplayground"

int fd1, fd2;

int main()
{
    char buf[0x400];

    fd1 = open(DEV_NAME, O_RDWR);
    close(fd1);

    fd2 = open(DEV_NAME, O_RDWR);
    
    strcpy(buf, "hello");
    write(fd2, buf, strlen(buf));
    printf("[+] writing: %s\n", buf);

    read(fd2, buf, 0x400);
    printf("[+] read fd2 buf: %s\n", buf);




    close(fd2);

    return 0;
}