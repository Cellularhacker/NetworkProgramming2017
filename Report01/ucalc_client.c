/*
* 2017-10-10 KST
* Report01/ucalc_client.c
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
//#define BUF_SIZE 1024


//
// Declaration of function
//
void error_handling(char *message);


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
    int sock;
    struct data_packet d_pack; //Using the struct instead of opmsg[] and op_cnt.
    int result, i;
    struct sockaddr_in serv_adr, from_adr;
    socklen_t adr_sz;

    // This application requires 3 parameters
    if(argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    while(1) {
        fputs("Operand count: ", stdout);

        // Clearing the memory space allocated in data_packet struct.
        memset(&d_pack, 0, sizeof(d_pack));

        // Looping the scanning process if it is not in correct range.
        while(1) {
            scanf("%d", &d_pack.op_num);
            if(d_pack.op_num > 30 || d_pack.op_num < 2)
                printf("It must be 2 to 30 numbers");
            else
                break;
        }

        for(i=0; i<d_pack.op_num; i++) {
            printf("Operand %d: ", i+1);
            scanf("%d", &d_pack.operand[i]);
        }

        fgetc(stdin);
        fputs("Operator: ", stdout);
        scanf("%c", &d_pack.op_code);

        //sendto();
        sendto(sock, &d_pack, sizeof(d_pack), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
        
        //recvfrom();
        recvfrom(sock, &result, sizeof(result), 0, (struct sockaddr*)&from_adr, &adr_sz);

        printf("Operation result: %d \n", result);
    }
    close(sock);

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