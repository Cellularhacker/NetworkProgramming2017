/*
* 2017-10-10 KST
* Report01/ucalc_server.c
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


//
// Declaration of function
//
void error_handling(char *message);
int calculate(int opnum, int opnds[], char operator);


//
// Definition of struct
//
struct data_packet {
    int op_num;
    char op_code;
    int operand[30];
};


//
// Main Function
//
int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    struct data_packet d_pack; //Using the struct instead of opmsg[] and op_cnt.
    int result, i;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    // This application requires 2 parameters
    if(argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // socket();
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
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
        // accept();
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

        // Showing the information of the client connected.
        printf("[Client %d] has been connected! (socket: %d, IP: %u, port: %hu)\n",
            i, clnt_sock, ntohl(clnt_adr.sin_addr.s_addr), ntohs(clnt_adr.sin_port));
        //printf("\tsocket: %d, IP: %u, port: %hu", clnt_sock, ntohl(clnt_adr.sin_addr.s_addr), ntohs(clnt_adr.sin_port));

        // Expected the possibility getting broken packets.
        read(clnt_sock, &d_pack, BUF_SIZE-1);

        // Calculating Process
        result = calculate(d_pack.op_num, d_pack.operand, d_pack.op_code);

        // Sending the result of calculating requested from the client.
        write(clnt_sock, &result, sizeof(result));

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

int calculate(int opnum, int opnds[], char op) {
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