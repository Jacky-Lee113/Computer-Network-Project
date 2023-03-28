/* A simple echo client using TCP */
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <fcntl.h>      /* for file open flags */
#include <sys/stat.h>
#include <string.h>



#define SERVER_TCP_PORT 3000	/* well-known port */
#define BUFLEN		256	/* buffer length */

int main(int argc, char **argv)
{
	int 	n, i, bytes_to_read;
	int 	sd, port;
	struct	hostent		*hp;
	struct	sockaddr_in server;
	char	*host, *bp, rbuf[BUFLEN], sbuf[BUFLEN];
    FILE *fd; //file descriptor 

	switch(argc){
	case 2:
		host = argv[1];
		port = SERVER_TCP_PORT;
		break;
	case 3:
		host = argv[1];
		port = atoi(argv[2]);
		break;
	default:
		fprintf(stderr, "Usage: %s host [port]\n", argv[0]);
		exit(1);
	}

	/* Create a stream socket	*/	
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "Can't creat a socket\n");
		exit(1);
	}

	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if (hp = gethostbyname(host)) 
	  bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);
	else if ( inet_aton(host, (struct in_addr *) &server.sin_addr) ){
	  fprintf(stderr, "Can't get server's address\n");
	  exit(1);
	}

	/* Connecting to the server */
	if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1){
	  fprintf(stderr, "Can't connect \n");
	  exit(1);
	}

	printf("Enter File Name: \n");
	while(n=read(0, sbuf, BUFLEN)){
		write(sd, sbuf, n);
	  
		int MaxBytes = 100;
		FILE *file;
		char Buffer[BUFLEN];
		file = fopen(sbuf, "wb");
		
		// Receives the 1st 100 bytes of hte file
	  	n = recv(sd, Buffer, MaxBytes, 0);
	  	
	  	// Checks if the 1st byte among the 100 bytes 
	  	// doesnt match the 1st char byte of the 
	  	// intended error message
	  	if(strncmp(Buffer, "✧", 1) != 0){
	  		// Writes that 1st 100 bytes into the new file
	  		fwrite(Buffer, sizeof(char), n, file);
	  		// along with the rest
	  		while ((n = recv(sd, Buffer, MaxBytes, 0)) > 0){
		  	if (fwrite(Buffer, sizeof(char), n, file) != n){
		  		printf("Error Writing the file");
		  	}
		  	else{
		  		//printf("A File Packet Was Received\n");
	  		}
	  
			}
			fclose(file);
		  
		  	// Reopens the file to read and display it to the
		  	// terminal and user, then closes it
			file = fopen(sbuf, "r");
			printf("The contents of the file are\n");
			while(fgets(Buffer, 100, file) != NULL){
		  		printf("%s", Buffer);
			}
	  		return 0;
	  		fclose(fd);
    			close(sd);
	  	}
	  	
	  	else if(strncmp(Buffer, "✧", 1) == 0){
	  		printf("%s\n", Buffer);

	  		fclose(fd);
		    	close(sd);
	  		return 0;
	  	}

	  
	  
	  
		
	  	
	  	
	  	
	  	
	  	
	  	
	  	
	  	  
      
        }
        
        
        //fclose(fd);
    	//close(sd);
    	//return 0;
        
    }
    
   
