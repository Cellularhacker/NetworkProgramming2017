/*
* 2017-09-03 KST
* Cellularhacker
* /Day02/fd_seri.c
* Ver1.0
*/

// This code is based on Linux.
#include <stido.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

//
// Main Function
//
int main(void) {
    int fd1, fd2, fd3;

    fd1 = socket(PF_INET, SOCK_STREAM, 0);
    fd2 = open("test.dat", O_CREAT|O_WRONLY|O_TRUNC);
    fd3 = socket(PF_INET, SOCK_DgRAM, 0);

    printf("file descriptor 1: %d\n", fd1);
    printf("file descriptor 2: %d\n", fd2);
    printf("file descriptor 3: %d\n", fd3);

    // Closing File Descriptors that opened.
    close(fd1); close(fd1); close(fd3);

    return 0;
}