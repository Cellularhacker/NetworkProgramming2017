/*
* 2017-09-19 KST
* Cellularhacker
* /Day04/endian_conv.c
* Ver1.0
*/

// This code is based on Linux.
#include <stido.h>
#include <arpa/inet.h>

//
// Main Function
//
int main(int argc, char *argv[]) {
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_addr = 0x123455678;
    unsigned long net_addr;

    net_port = htons(host_port);    // Host to Network Short
    net_addr = htonl(host_addr);    // Host to Network Long

    printf("Host ordered port: %#x \n", host_port);
    printf("Network ordered port: %#x \n", net_port);
    printf("Host ordered address: %#x \n", host_addr);
    printf("Network ordered address: %#x \n", net_addr);

    return 0;
}