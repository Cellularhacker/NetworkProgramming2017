/*
* Cellularhacker
* Original: echo_mpclient.c
*/

// Including header files.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// Macro Constants
#define BUF_SIZE 100

// structs
struct s_msg{
	char name[30];
	char msg[BUF_SIZE];
};

// Declaring Functions
void error_handling(char *message);
void read_routine(int sock, struct s_msg message);
void write_routine(int sock, struct s_msg message);


/*
* STARTING OF "Main Function"
*/
int main(int argc, char *argv[]) {
	// socket variables;
	int sock;
	struct sockaddr_in serv_adr;

	// process variables;
	pid_t pid;
	
	// etc;
	struct s_msg message;
	char buf[BUF_SIZE];

	if(argc != 3) {
		printf("Usage : %s <IP> <port> \n", argv[0]);
		exit(1);
	}

	// Processing about socket;
	sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");

	pid = fork();
	if(pid == 0)
		write_routine(sock, message);
	else
		read_routine(sock, message);

	close(sock);
	return 0;
}
/*
* ENDING OF "Main Function"
*/


// Defining functions
void read_routine(int sock, struct s_msg message) {
	while(1) {
		int str_len = read(sock, &message, sizeof(message));
		if(str_len == 0) //requested to exit the application.
			return;
		
		message.msg[str_len] = '\0';
		printf("[%s] : %s", message.name, message.msg);
	}
}

void write_routine(int sock, struct s_msg message) {
	printf("Your name : ");
	fgets(message.name, sizeof(message.name), stdin);
	message.name[strlen(message.name)-1] = '\0';
	sprintf(message.name, "%s", message.name);
	while(1) {
		fgets(message.msg, BUF_SIZE, stdin);
		if(!strcmp(message.msg, "q\n") || !strcmp(message.msg, "Q\n")) {
				shutdown(sock, SHUT_WR);
				return;
		}
		write(sock, (void*)&message, sizeof(message));
	}
}

void error_handling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}