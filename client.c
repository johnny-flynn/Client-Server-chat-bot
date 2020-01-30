#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define MAXBUFFLEN 256 //eliminate ghost numbers
#define PROTOCOL 0 //eliminate ghost numbers
#define CLOSECONNECTION "!EXIT\n"

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;

    struct hostent *server;

    char buffer[MAXBUFFLEN];


    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    
    portno = atoi(argv[2]);


    sockfd = socket(AF_INET, SOCK_STREAM, PROTOCOL);

    if (sockfd < 0) 
        error("ERROR opening socket");
    else{
        printf("Socket Opened\n");
    }
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);}
    else{
        printf("Connected to Host\n");
    }


    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);


    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    else{
        printf("Connection Successful\n");
    }
    bool runLoop = true;
    while(runLoop){
        printf("Enter message to Server: ");

        bzero(buffer,MAXBUFFLEN);

        fgets(buffer,MAXBUFFLEN-1,stdin);

        if (strcmp(buffer, CLOSECONNECTION) == 0) {
            if (shutdown(sockfd, SHUT_RDWR) == 0) {
                printf("Connection closed...\n");
                exit(0);
            }
            else {
                error("ERROR unable to close connection");
            }
        }

        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
            error("ERROR writing to socket");
        else{
            printf("message sent to server\n");
        }
        bzero(buffer,MAXBUFFLEN);
        n = recv(sockfd,buffer,MAXBUFFLEN-1,0);
        if (n < 0) 
            error("ERROR reading from socket");
        printf("Here is the servers message: %s\n",buffer);
    }
    return 0;
}