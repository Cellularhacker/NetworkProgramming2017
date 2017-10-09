/*
* 2017-10-09 KST
* Day05/op_server.c
* Cellularhacker
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


//
// Definition of Macro constant
//
#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4


//
// Declaration of function
//
void error_handling(char *message);
int calculate(int opnum, int opnds[], char operator);

//
// Main Function
//
int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    char opmsg[BUF_SIZE];
    int result, opnd_cnt, i;
    int recv_cnt, recv_len;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    // This application requires 2 parameters
    if(argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // socket();
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = INADDR_ANY;
    serv_adr.sin_port = htons(atoi(argv[1]));

    // bind();
    if( bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1 )
        error_handling("bind() error");

    // listen();
    if( listen(serv_sock, 5) == -1 )
        error_handling("listen() error");

    clnt_adr_sz = sizeof(clnt_adr);

    for(i=0; i<5; i++) {
        opnd_cnt = 0;
        // accept();
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        read(clnt_sock, &opnd_cnt, 1);

        recv_len = 0;
        while( (opnd_cnt*OPSZ+1) > recv_len ) {
            recv_cnt = read(clnt_sock, &opinfo[recv_len], BUF_SIZE-1);
            recv_len += recv_cnt;
        }

        // Calculating Process
        result = calculate(opnd_cnt, (int*)&result, szieof(result));

        // Closing the socket connected with the client
        close(clnt_sock);
    }

    // Closing this server's socket
    close(serv_sock);

    return 0;
}


//
// Other Functions
//
void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int calculate(int opnum, int opnds[], char operator) {
    int result = opnds[0], i;
    switch(op) {
        case '+':
            for(i=1; i<opnum; i++) result += opnds[i];
            break;
        case '-':
            for(i=1; i<opnum; i++) result -= opnds[i];
            break;
        case '*':
            for(i=1; i<opnum; i++) result *= opnds[i];
            break;
    }

    return result;
}