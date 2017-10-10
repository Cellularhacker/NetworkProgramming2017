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
//#define BUF_SIZE 128


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
    int result, i=0;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    // This application requires 2 parameters
    if(argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // socket();
    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = INADDR_ANY;
    serv_adr.sin_port = htons(atoi(argv[1]));

    // bind();
    if( bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1 )
        error_handling("bind() error");

    clnt_adr_sz = sizeof(clnt_adr);

    while(1) {
        // recvfrom(); --> Receiving a data.
        clnt_adr_sz = sizeof(clnt_adr);
        recvfrom(serv_sock, &d_pack, sizeof(d_pack), 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

        // Showing the information of the client requested.
        printf("[Client %d] just requested to calc something! (socket: %d, IP: %u, port: %hu)\n",
            i, clnt_sock, ntohl(clnt_adr.sin_addr.s_addr), ntohs(clnt_adr.sin_port));
        //printf("\tsocket: %d, IP: %u, port: %hu", clnt_sock, ntohl(clnt_adr.sin_addr.s_addr), ntohs(clnt_adr.sin_port));

        // Calculating Process
        result = calculate(d_pack.op_num, d_pack.operand, d_pack.op_code);

        // sendto(); --> Sending the result of calculating requested from the client.
        sendto(serv_sock, &result, sizeof(result), 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);
        
        // Increasing the count. This cannot tracking that the client is a same one or not.
        i++;
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