/* The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "CUnit/CUnitCI.h"
#include "osapi.h"
#include <climits>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

class ClientThread : public osapi::Thread
{
public:
    ClientThread() :
        Thread(3, 512, osapi::JOINABLE)
    {
        bzero(buffer,256);
    }
    void SetSocketId(int soc){
        socket_id = soc;
    }

    int GetMessage(char * buf){
        int len_msg = 0;
        
        if(!isRunning()){
            return 0;
        }

        if((len_msg = strlen(buffer)) != 0){
            strcpy(buf, buffer);
            bzero(buffer,256);
            if(false == mutex_client.lock(1)){
                mutex_client.unlock();
            }
        }
        
        return len_msg;
    }

    void SendMessage(char *data){
        if(isRunning()){
            send(socket_id, data, strlen(data), 0);
        }
    }
private:
    int socket_id;
    char buffer[256];
    int bytes_read;
    osapi::Mutex mutex_client;
    virtual void body() {
        while(1){
            if(mutex_client.lock(UINT_MAX)){
                bytes_read = read(socket_id,buffer,255);
                if(bytes_read == 0){
                    printf("Disconnect client\n");
                    break;
                }
            }
        }   
        close(socket_id);
    }
};

class AcceptThread : public osapi::Thread
{
public:
    AcceptThread() : 
        Thread(3, 512, osapi::JOINABLE) 
    { 
        sock_id = -1;
        readed_sock = false;
    }
    void SetParameters(int sock_, socklen_t clilen_, struct sockaddr_in cli_addr_){
        sockfd = sock_;
        clilen = clilen_;
        cli_addr = cli_addr_;
    }
    int GetSocketId(void){
        int sock_temp = -1;
        if(sock_id > 0 ){
            if(false == mutex_accept.lock(1)){
                sock_temp = sock_id;
                sock_id = -1;
                mutex_accept.unlock();
                return sock_temp;
            }
        }
        return sock_temp;
    }
private:
    int sockfd;
    int sock_id;
    bool readed_sock;
    osapi::Mutex mutex_accept;
    socklen_t clilen;
    struct sockaddr_in cli_addr;
    virtual void body() {
        while(1){
            if(mutex_accept.lock(UINT_MAX)){
                sock_id = accept(sockfd, 
                    (struct sockaddr *) &cli_addr, &clilen);
                printf("server: got connection from %s port %d\n",
                        inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
            }
        }   
    }
};


class ServerThread : public osapi::Thread
{
public:
    ServerThread(int port) : 
        Thread(3, 512, osapi::JOINABLE),  
        portno(port)
    { 
        // create a socket
     // socket(int domain, int type, int protocol)
     sockfd =  socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

     // clear address structure
     bzero((char *) &serv_addr, sizeof(serv_addr));

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
    clilen = sizeof(cli_addr);
    accept.SetParameters(sockfd, clilen, cli_addr);
    accept.run();
    num_client = 0;
    
    run(); 
    }
    ~ServerThread(){
        close(sockfd);
    }
private:
    int sockfd, newsockfd; 
    int portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    ClientThread client[5];
    int num_client;
    AcceptThread accept;
    virtual void body() {

        while(1){
           //check new connections
            if((sockfd = accept.GetSocketId()) != -1){
                printf("New connection! %d\n", sockfd);
                client[num_client].SetSocketId(sockfd);
                client[num_client].run();
                ++num_client;
            }

            //check disconnect clients
            /*
            for(int i=0;i<num_client;++i){
                if(!client[i].isRunning()){
                    --num_client;

                }
            }
            */

            //send to all clients
            for(int i=0;i<num_client;++i){
               if(client[i].GetMessage(buffer)){
                   for(int j=0;j<num_client;++j){
                       if(i != j){
                           client[j].SendMessage(buffer);
                       }
                   }
               }
           }
        }
        
    }
};

int main(int argc, char *argv[])
{
    ServerThread server(1007);
    server.join(UINT_MAX);

    return 0; 
}