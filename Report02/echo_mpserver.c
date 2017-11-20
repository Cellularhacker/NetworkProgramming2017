/*
* Cellularhacker
* Original: storeserv.c
*/

// Including header files.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// Macro Constants
#define BUF_SIZE 100
#define MAX_LOG 10

// structs
struct s_msg {
	char name[30];
	char msg[BUF_SIZE];
};

// Declaring Functions
void error_handling(char *message);
void read_childproc(int sig);


/*
* STARTING OF "Main Function"
*/
int main(int argc, char *argv[]) {
	// socket variables;
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t adr_sz;
	int str_len;

	// process variables;
	int fds[2];
	pid_t pid;
	struct sigaction act;
	int state;

	// etc;
	struct s_msg message;

	if(argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// Processing about status of child processes;
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0);

	// Processing about socket;
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	pipe(fds);

	// First Furcation point;
	pid = fork();
	if(pid == 0) {
		FILE * fp = fopen("echomsg.txt", "wt");
		int i;
		char msgbuf[sizeof(struct s_msg) + 10]; // 10 is for extra space.
		memset(&msgbuf, 0, sizeof(msgbuf));

		for(i=0; i<MAX_LOG; i++) {
			read(fds[0], &message, sizeof(struct s_msg));
			strcpy(msgbuf, "[");
			strcat(msgbuf, message.name);
			strcat(msgbuf, "] : ");
			strcat(msgbuf, message.msg);
			fwrite((void*)msgbuf, 1, strlen(msgbuf), fp);
			printf("%s", msgbuf);
		}

		fclose(fp);
		return 0;
	}

	while(1) {
		adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		if(clnt_sock == -1)
			continue;
		else
			puts("new client connected...");

		// Second Furcation point;
		pid = fork();
		if(pid == 0) {
			close(serv_sock);
			while((str_len = read(clnt_sock, &message , sizeof(message))) != 0) {
				write(clnt_sock, &message, str_len);
				write(fds[1], &message, str_len);
			}

			close(clnt_sock);
			puts("client disconnected...");

			return 0;
		} else
			close(clnt_sock);
	}

	close(serv_sock);
	return 0;
}
/*
* ENDING OF "Main Function"
*/


// Defining functions
void read_childproc(int sig) {
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG);
	printf("removed proc id: %d\n",pid);
}

void error_handling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}