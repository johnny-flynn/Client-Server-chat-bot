/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h> //used for input and output 
#include <sys/types.h> //used to declare data types used in system calls
#include <sys/socket.h> //declares everything needed for sockets
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses
#include <stdlib.h> //added the next few bits to remove warnings from the starter code and for the 
#include <unistd.h> //use of strings when needed
#include <string.h>
#include <stdbool.h>

#define MAXBUFFLEN 256 //eliminate ghost numbers
#define PROTOCOL 0 //eliminate ghost numbers
#define MAXCONNECTIONS 5 //eliminte ghost numbers

void error(char *msg) //This function is called when a system call fails. It displays a message about the error on stderr and then aborts the program
{
    perror(msg);
    exit(1);
}

int connectionManagement(int sokfd, struct sockaddr_in cliaddr, socklen_t clilen){ //handles coneections and waits for the client to attempt to connect
    printf("Ready to connect\n");
    int newConnection = accept(sokfd, (struct sockaddr *) &cliaddr, &clilen);
    if (newConnection < 0) {
          error("ERROR on accept");
          return 0;
    }
     else {
        printf("Connection Successful\n");
        return newConnection;
     }
}


int main(int argc, char *argv[])
{
     int sockfd, newsockfd; //sockfd and newsockfd are file descriptors. These two variables store the values returned by the socket system call and the accept system call.
     int portno; //stores the port number on which the server accepts connections.
     int clilen; //stores the size of the address of the client. This is needed for the accept system call.
     int n; //is the return value for the read() and write() calls; i.e. it contains the number of characters read or written.

     char buffer[MAXBUFFLEN];                            //The server reads characters from the socket connection into this buffer

     struct sockaddr_in serv_addr, cli_addr;             //A sockaddr_in is a structure containing an internet address.


     if (argc < 2) {                                     //The user needs to pass in the port number on which the server will accept 
         fprintf(stderr,"ERROR, no port provided\n");    //connections as an argument. This code displays an error message if the user fails to do this.
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, PROTOCOL);    //The socket() system call creates a new socket and passes three agruments. 
                                                         //AF_INET is the address domain of the socket, the second is the type of socket,
                                                         //and the third is the protocol(should almost always be 0 but I declated it to a variable to avoid ghosting)
    
     if (sockfd < 0)                                     //If the socket call fails, it returns -1. In this case the program displays and error 
        error("ERROR opening socket");                   //message and exits. However, this system call is unlikely to fail.

     bzero((char *) &serv_addr, sizeof(serv_addr));      //The function bzero() sets all values in a buffer to zero.
                                                         //It takes two arguments, the first is a pointer to the buffer and the second is the size of the buffer.
     
     portno = atoi(argv[1]);                             //The port number on which the server will listen for connections is passed in as an argument, 
                                                         //and this statement uses the atoi() function to convert this from a string of digits to an integer.
     
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     
     if (bind(sockfd, (struct sockaddr *) &serv_addr,    //The bind() system call binds a socket to an address
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     
     listen(sockfd,MAXCONNECTIONS); //The listen system call allows the process to listen on the socket for connections.
     clilen = sizeof(cli_addr);

     newsockfd = connectionManagement(sockfd, cli_addr, clilen);
     bool runLoop = true;
    while(runLoop){
        bzero(buffer,MAXBUFFLEN);
        n = read(newsockfd,buffer,MAXBUFFLEN-1);
        if (n < 0) 
            error("ERROR reading from the socket");
        else {
            printf("Here is the clients message: %s\n",buffer);
            }
        if (n == 0) {
            printf("Connection with client lost\n");
            newsockfd = connectionManagement(sockfd, cli_addr, clilen);
        }
        if(n!=0){
            printf("Enter message to the Client : ");
            bzero(buffer,MAXBUFFLEN);
            fgets(buffer,MAXBUFFLEN-1,stdin);
            n = write(newsockfd, buffer, strlen(buffer));
            if (n < 0) 
                error("ERROR writing to socket");
            else{
            printf("Message sent to Client\n");
        }
        }
    }
    return 0;
}