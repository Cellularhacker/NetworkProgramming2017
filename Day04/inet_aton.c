/*
* 2017-09-19 KST
* Cellularhacker
* /Day04/inet_aton.c
* Ver1.0
*/

// This code is based on Linux.
#include <stido.h>
#include <stdlib.h>
#include <arpa/inet.h>

//
// Declaration of function
//
void error_handling(char *message);

//
// Main Function
//
int main(int argc, char *argv[]) {
    char *addr = "127.232.124.79";
    struct sockaddr_in addr_inet;

    if(!inet_aton(addr, &addr_inet.sin_addr))
        error_handling("Conversion error");
    else
        printf("Network ordered integer addr: %#x \n", addr_inet.sin_addr.s_addr);

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
