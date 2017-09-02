/*
* 2017-09-03 KST
* Cellularhacker
* /Day01/low_open.c
* Ver1.0
*/

// This code is based on Linux.
#include <stido.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

//
// Declaration of function
//
void error_handling(char *message);

//
// Main Function
//
int main(void) {
    int fd;
    char buf[] = "Let's go!\n";

    fd = open("data.txt", O_CREAT|O_WRONLY|O_TRUNC);
    if(fd == -1)
        error_handling("open() error!");
    printf("file descriptor: %d \n", fd);

    if(write(fd, buf, sizeof(buf)) == -1)
        error_handling("write() error!");
    
    // Closing the File Descriptor.
    close(fd);

    return 0;
}

// 
// Definition of function
// 
void error_handling(char *message) {
    fputs(message, stderr); // "stderr" is a File Pointer.
    fputc('\n', stderr);
    exit(1);
}