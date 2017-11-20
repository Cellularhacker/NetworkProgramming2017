/*
* Cellularhacker
* Original: echo_selectserv.c
*/

// Including header files.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

// Macro Constants
#define BUF_SIZE 100
#define MAX_LOG 10

// structs
struct s_msg{
	char name[30];
	char msg[BUF_SIZE];
};

// Declaring Functions
void error_handling(char *message);


/*
* STARTING OF "Main Function"
*/
int main (int argc, char *argv[]) {
	// socket variables;
	int serv_sock, clnt_sock[10];
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t adr_sz;

	// select() variables;
	struct timeval timeout;
	fd_set reads, cpy_reads;
	int fd_max, fd_num, cli_num=0, log_cnt=0;

	// etc;
	int i, cli_i;
	FILE * fp=fopen("chat_log.txt", "wt");
	struct s_msg message;
	char buf[sizeof(struct s_msg) + 10]; // 10 is for extra space.

	if(argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// Clearing some variables before using.
	memset(clnt_sock, 0, sizeof(clnt_sock));
	memset(buf, 0, sizeof(buf));

	// Processing about socket;
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	// Processing about select();
	FD_ZERO(&reads);
	FD_SET(0, &reads);
	FD_SET(serv_sock, &reads);
	fd_max = serv_sock;

	while(1) {
		cpy_reads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if((fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout)) == -1)
			break;
		if(fd_num == 0)
			continue;

		for(i=0; i<fd_max+1; i++) {
			if(FD_ISSET(i, &cpy_reads)) {
				if(i == 0) { // admin wanted to broadcast something to everyone;
					sprintf(message.name, "server");

					fgets(message.msg, BUF_SIZE, stdin);

					for(cli_i=1; cli_i<=cli_num; cli_i++) //sending the message to everybody.
						write(clnt_sock[cli_i], &message, sizeof(message));

					printf("[%s] : %s", message.name, message.msg); //and also showing it on admin's console.

					//==STARTING OF SAME BLOCK A==
					if(log_cnt < MAX_LOG) { //remaining logging counts.
						strcpy(buf, "["); //just using strcpy() to avoid using memset() for every while() context.
						strcat(buf, message.name);
						strcat(buf, "] : ");
						strcat(buf, message.msg);
						fwrite((void*)buf, 1, strlen(buf), fp);
						log_cnt++;
					} else if(log_cnt >= MAX_LOG) { //full of logging counts.
						fclose(fp);
						printf("closed fp.\n");
					}
					//==ENDING OF SAME BLOCK A==

				} else if(i == serv_sock) { // someone requested to connect;
					adr_sz=sizeof(clnt_adr);

					clnt_sock[++cli_num] = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
					
					FD_SET(clnt_sock[cli_num], &reads);
					if(fd_max < clnt_sock[cli_num])
						fd_max = clnt_sock[cli_num];

					printf("connected client : %d \n", clnt_sock[cli_num]);

				} else { // rests: reading messages;
					int str_len = read(clnt_sock[i-serv_sock], &message, sizeof(message));

					if(str_len == 0) { //someone requested to close the connection.
						FD_CLR(clnt_sock[i-serv_sock], &reads);
						close(clnt_sock[i-serv_sock]);
						cli_num--; //byebye.

						printf("closed client: %d \n", clnt_sock[i-serv_sock]);

					} else { //someone sent a message to server.
						for(cli_i=1; cli_i<=cli_num; cli_i++)
							write(clnt_sock[cli_i], &message, str_len);
						
						printf("[%s] : %s", message.name, message.msg);

						//==STARTING OF SAME BLOCK A==
						if(log_cnt < MAX_LOG) {
							strcpy(buf, "["); //just using strcpy() to avoid using memset() for every while() context.
							strcat(buf, message.name);
							strcat(buf, "] : ");
							strcat(buf, message.msg);
							fwrite((void*)buf, 1, strlen(buf), fp);
							log_cnt++;
						} else if(log_cnt >= MAX_LOG) { //full of logging counts.
							fclose(fp);
							printf("closed fp.\n");
						}
						//==ENDING OF SAME BLOCK A==
					}
				}
			}
		}
	}

	close(serv_sock);
	return 0;
}
/*
* ENDING OF "Main Function"
*/


// Defining functions
void error_handling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}