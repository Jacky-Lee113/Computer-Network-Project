/* time_server.c - main */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SIZE 1024

/*------------------------------------------------------------------------
 * main - Iterative UDP server for TIME service
 *------------------------------------------------------------------------
 */

#define DATA_PDU 'D'
#define FINAL_PDU 'F'
#define ERROR_PDU 'E'
#define FILENAME_PDU 'C'

#define DATA_SIZE 100

typedef struct SPDU {
  char type;
  char data[DATA_SIZE];
} SPDU;



void send_file(FILE* fp, int s, struct sockaddr_in fsin)
{
  int n;
  char buffer[SIZE];

  // Sending the data
  while (fgets(buffer, SIZE, fp) != NULL)
  {
    printf("\n[SENDING] Data: %s", buffer);
    n = sendto(s, buffer, strlen(buffer), 0, (struct sockaddr*)&fsin, sizeof(fsin));
    if (n == -1)
    {
      perror("[ERROR] sending data to the server.");
      exit(1);
    }
  }

  // Sending the terminator
  strcpy(buffer, "\nFile transfer complete");
  printf("\n\nFile transfer complete");
  sendto(s, buffer, strlen(buffer), 0, (struct sockaddr*)&fsin, sizeof(fsin));

  fclose(fp);
}

int main(int argc, char *argv[])
{
	static const SPDU empty_pdu;
	SPDU pdu;
	char ch, file_name[25];
	struct  sockaddr_in fsin;	/* the from address of a client	*/
	char	buf[100];		/* "input" buffer; any size > 0	*/
	char    *pts;
	int	sock;			/* server socket		*/
	time_t	now;			/* current time			*/
	int	alen;			/* from-address length		*/
	struct  sockaddr_in sin; /* an Internet endpoint address         */
    int     n, s, type;        /* socket descriptor and socket type    */
	int 	port=atoi(argv[1]);
                                                                                

	switch(argc){
		case 1:
			break;
		case 2:
			port = atoi(argv[1]);
			break;
		default:
			fprintf(stderr, "Usage: %s [port]\n", argv[0]);
			exit(1);
	}

        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_port = htons(port);
                                                                                                 
    /* Allocate a socket */
        s = socket(AF_INET, SOCK_DGRAM, 0);
        if (s < 0)
		fprintf(stderr, "can't create socket\n");
                                                                                
    /* Bind the socket */
        if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		fprintf(stderr, "can't bind to %d port\n",port);

	while (1) {
		alen = sizeof(fsin);
		n = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&fsin, &alen);
		if (n < 0)
			fprintf(stderr, "recvfrom error\n");
		buf[n] = '\0'; // null-terminate the buffer
		char *name = buf;
		printf("Received filename: %s", name);
		FILE *fp;
		if ((fp = fopen(name,"r")) == NULL){
			char* error_msg = "File not found";
			sendto(s, error_msg, strlen(error_msg), 0, (struct sockaddr *)&fsin, sizeof(fsin));
			printf("\nError: %s", error_msg);
			exit(0);
		}
		send_file(fp, s, fsin);
		exit(0);
	}
	return 0;
}
