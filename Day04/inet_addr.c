/*
* 2017-09-19 KST
* Cellularhacker
* /Day04/inet_addr.c
* Ver1.0
*/

// This code is based on Linux.
#include <stido.h>
#include <arpa/inet.h>

//
// Main Function
//
int main(int argc, char *argv[]) {
    char *addr1 = "1.2.3.4";
    char *addr2 = "1.2.3.256";

    unsigned long conv_addr = inet_addr(addr1);
    if(conv_addr == INADDR_NONE)
        printf("Error occured! \n");
    else
        printf("Network ordered integer addr: %#lx \n", conv_addr);

    conv addr = inet_addr(addr2);
    if(conv_addr == INADDR_NONE)
        printf("Error occured! \n");
    else
        printf("Network ordered integer addr: %#lx \n", conv_addr);

    return 0;
}