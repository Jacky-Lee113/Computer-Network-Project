/* A simple echo server using TCP */
#include <stdio.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


#define SERVER_TCP_PORT 3000	/* well-known port */
#define BUFLEN		256	/* buffer length */

int echod(int);
void reaper(int);


int main(int argc, char **argv)
{
	struct sockaddr_in serv_addr;
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int 	sd, new_sd, client_len, port;
	struct	sockaddr_in server, client;

	switch(argc){
	case 1:
		port = SERVER_TCP_PORT;
		break;
	case 2:
		port = atoi(argv[1]);
		break;
	default:
		fprintf(stderr, "Usage: %s [port]\n", argv[0]);
		exit(1);
	}

	/* Create a stream socket	*/	
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "Can't creat a socket\n");
		exit(1);
	}

	/* Bind an address to the socket	*/
	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1){
		fprintf(stderr, "Can't bind name to socket\n");
		exit(1);
	}

	/* queue up to 5 connect requests  */
	listen(sd, 5);

	(void) signal(SIGCHLD, reaper);

	while(1) {
	  client_len = sizeof(client);
	  new_sd = accept(sd, (struct sockaddr *)&client, &client_len);
	  if(new_sd < 0){
	    fprintf(stderr, "Can't accept client \n");
	    exit(1);
	  }
	  switch (fork()){
	  case 0:		/* child */
		(void) close(sd);
		exit(echod(new_sd));
		exit(0);
	  default:		/* parent */
		(void) close(new_sd);
		break;
		exit(0);
	  case -1:
		fprintf(stderr, "fork: error\n");
	  }
	}
}









/*	echod program	*/
int echod(int sd)
{
	FILE *fp;
	char	*bp, buf[BUFLEN];
	int 	n, bytes_to_read;
	int MaxBytes = 100;
	int name_len = 0;
	
	
	n = recv(sd, buf, BUFLEN ,0);
	printf("Received the filename: %s", buf);


		// Removes any null or unprintable chatacters form the string
		while(n>0 && !isprint(buf[n-1])){
			buf[--n] = '\0';
		}

		char *name = buf;
		fp = fopen(name, "r");



		// Sends an error message to the client if failed to open the file
		if(fp == NULL){
			printf("✧Error Invalid File Input✧\n");
			char message[BUFLEN] = "✧Error Invalid File Input✧";
			send (sd, message, sizeof(message), 0);
			exit(0);
		}

		// sends the file in packets of 100 bytes if it opened the file successfully
		else{
			printf("File Open Successful, sending to client...\n");
			while((n = fread(buf, sizeof(char), MaxBytes, fp)) > 0){
				if(send(sd, buf, n, 0) != n){
					printf("Error Sending Packets\n");
				}
				
			}
			printf("File Transfer Complete\n");
			fclose(fp);



		}
		// closes the socket and the program
		close(sd);
		exit(0);

}


/*	reaper		*/
void	reaper(int sig)
{
	int	status;
	while(wait3(&status, WNOHANG, (struct rusage *)0) >= 0);
}
