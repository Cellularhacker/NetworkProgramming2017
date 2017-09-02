/*
* 2017-09-03 KST
* Cellularhacker
* /Day01/low_read.c
* Ver1.0
*/

// This code is based on Linux.
#include <stido.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 100

//
// Declaration of function
//
void error_handling(char *message);

//
// Main Function
//
int main(void) {
    int fd;
    char buf[BUF_SIZE];

    fd = open("data.txt", O_RDONLY);
    if(fd == -1)    // Failed to getting a new File Descriptor.
        error_handling("open() error!");
    printf("file descriptor: %d \n", fd);

    if(read(fd, buf, sizeof(buf)) == -1)    // Failed to read the content from the File Descriptor.
        error_handling("read() error!");
    printf("file data: %s", buf);
    
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