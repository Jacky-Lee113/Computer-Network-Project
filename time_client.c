/* time_client.c - main */

#include <sys/types.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>                                                                            
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
                                                                                
#include <netdb.h>

#define	BUFSIZE 64



/*------------------------------------------------------------------------
 * main - UDP client for TIME service that prints the resulting time
 *------------------------------------------------------------------------
 */
 
int file_transfer(char* buf, int s, char MSG[256])
{
    int i;
    char ch;
    FILE* fp = fopen(MSG, "a");
    if (fp == NULL) {
        perror("fopen failed");
        return 1;
    }
	if (strstr(buf, "File transfer complete")) {
		printf("File transfer complete");
		return 1; // end of file transfer
    }
    for (i = 0; i < s; i++) {
        ch = buf[i];
        if (ch == EOF || ch == '\0')
            return 1;
        else {
            printf("%c", ch);
			fprintf(fp, "%c", buf[i]);
        }
    }

    printf("\n");
    fclose(fp);
    return 0;
}



 
int main(int argc, char **argv)
{
	char MSG[256] = "";
	char	*host = argv[1];
	int	port = atoi(argv[2]);
	char	now[100];		/* 32-bit integer to hold time	*/ 
	struct hostent	*phe;	/* pointer to host information entry	*/
	struct sockaddr_in sin;	/* an Internet endpoint address		*/
	int	s, n, type, x;	/* socket descriptor and socket type	*/

	switch (argc) {
	case 1:
		break;
	case 2:
		host = argv[1];
	case 3:
		host = argv[1];
		port = atoi(argv[2]);
		break;
	default:
		fprintf(stderr, "usage: UDPtime [host [port]]\n");
		exit(1);
	}

	memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;                                                                
        sin.sin_port = htons(port);
                                                                                        
    /* Map host name to IP address, allowing for dotted decimal */
        if ( phe = gethostbyname(host) ){
                memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
        }
        else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
		fprintf(stderr, "Can't get host entry \n");
                                                                                
    /* Allocate a socket */
        s = socket(AF_INET, SOCK_DGRAM, 0);
        if (s < 0)
		fprintf(stderr, "Can't create socket \n");
    /* Connect the socket */
        if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		fprintf(stderr, "Can't connect to %s %s \n", host, "Time");

	printf("Enter the file name including the extension (e.g .txt .c .doc etc): ");
	scanf("%s", MSG);
	write(s, MSG, strlen(MSG));
	remove(MSG);
    int bytes_received = 0;
	int packets_sent = 0;
	while ((n = recv(s, now, sizeof(now), 0)) > 0) {
		packets_sent++;
		bytes_received += n;
		if (file_transfer(now, n, MSG)) {
			break; /* end of file */
		}
	}

    if (bytes_received == 0) {
        printf("Error: no data received\n");
    } else {
        printf(", received %d bytes\n", bytes_received);
		printf("Number of packets received: %d", packets_sent);
    }
    exit(0);
}
