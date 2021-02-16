#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "osapi.h"
#include <climits>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

class ReadClientThread : public osapi::Thread
{
public:
    ReadClientThread() :
        Thread(3, 2512, osapi::JOINABLE)
    {
    }
    void SetSocketId(int soc){
        sockfd = soc;
    }
private:
    int n; 
    char buffer[256];
    int sockfd;
    virtual void body() {
        while(1){
            bzero(buffer,256);
            n = read(sockfd, buffer, 255);
            if (n < 0) 
                error("ERROR reading from socket");
            //scanf("%1023[^\n]", line);
            printf("%s", buffer);
        } 
    }
};

class WriteClientThread : public osapi::Thread
{
public:
    WriteClientThread() :
        Thread(3, 512, osapi::JOINABLE)
    {
    }
    void SetSocketId(int soc){
        sockfd = soc;
    }
private:
    int n;
    char buffer[256];
    int sockfd;
    virtual void body() {
        while(1){
            printf("Please enter the message: ");
            bzero(buffer,256);
            fgets(buffer,255,stdin);
            n = write(sockfd, buffer, strlen(buffer));
            if (n < 0) 
                error("ERROR writing to socket");
        }
    }
};


class ClientThread : public osapi::Thread
{
public:
    ClientThread(int port, const char *hostname) :
        Thread(3, 512, osapi::JOINABLE), 
        portno(port), 
        server(gethostbyname(hostname))
    {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) 
            error("ERROR opening socket");
        if (server == NULL) {
            fprintf(stderr,"ERROR, no such host\n");
            exit(0);
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
        serv_addr.sin_port = htons(portno);
        if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
            error("ERROR connecting");
        bzero(buffer,256);
        run();
    }

private:
    char buffer[256];
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    ReadClientThread read;
    WriteClientThread write;
    virtual void body() {

        read.SetSocketId(sockfd);
        write.SetSocketId(sockfd);

        read.run();
        write.run();

        read.join(UINT_MAX);
        write.join(UINT_MAX);

        close(sockfd); 
    }
};

int main(int argc, char *argv[])
{
    ClientThread client(1007, "127.0.0.1");
    client.join(UINT_MAX);
    return 0;
}
