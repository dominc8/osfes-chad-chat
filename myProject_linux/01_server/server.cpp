/* The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     // create a socket
     // socket(int domain, int type, int protocol)
     sockfd =  socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

     // clear address structure
     bzero((char *) &serv_addr, sizeof(serv_addr));

     portno = atoi(argv[1]);

     /* setup the host_addr structure for use in bind call */
     // server byte order
     serv_addr.sin_family = AF_INET;  

     // automatically be filled with current host's IP address
     serv_addr.sin_addr.s_addr = INADDR_ANY;  

     // convert short integer value for port must be converted into network byte order
     serv_addr.sin_port = htons(portno);

     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

     listen(sockfd,5);

     // The accept() call actually accepts an incoming connection

     
        clilen = sizeof(cli_addr);

        newsockfd = accept(sockfd, 
                    (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) 
            error("ERROR on accept");

        printf("server: got connection from %s port %d\n",
                inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));


    while(1){
        // This send() function sends the 13 bytes of the string to the new socket
        bzero(buffer,256);
        n = read(newsockfd,buffer,255);
        if (n > 0){
            if(!strcmp(buffer, "exit\n")){
                printf("exit command\n");
                break;
            }

            send(newsockfd, buffer, strlen(buffer), 0);
        }

     }

     close(newsockfd);
     close(sockfd);
     return 0; 
}