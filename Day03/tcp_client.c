/*
* 2017-09-19 KST
* Cellularhacker
* /Day03/tcp_client.c
* Ver1.0
*/

// This code is based on Linux.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//
// Declaration of function
//
void error_handling(char *message);

//
// Main Function
//
int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len, idx=0;

    if(argc != 3) {   // What if it doesn't have 3 parameters at start, Exit the program
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) // Failed to getting a new File Descriptor.
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) // Failed to connect the server.
        error_handling("connect() error");

    // Reading by fetching each bytes.
    while(str_len == read(sock, &message[idx++], 1)) {
        if(str_len == -1)
            error_handling("read() error");

        str_len+=str_len;
    }
    
    printf("Message from server : %s \n", message);
    printf("Function read call count: %d \n", str_len);
    
    // Closing Socket
    close(sock);

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
