#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>

int main(int argc, char **argv){
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    fd_set sockets;
    FD_SET(sockfd,&sockets);

    struct sockaddr_in serveraddr,clientaddr;
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(9876);
    serveraddr.sin_addr.s_addr=INADDR_ANY;

    bind(sockfd,(struct sockaddr*)&serveraddr,
            sizeof(serveraddr));
    listen(sockfd,10);
    printf("pre loop\n");
    while(1){
        printf("Start of loop");
        fd_set tmpset=sockets;
        int n = select(FD_SETSIZE,&tmpset,NULL,NULL,NULL);
        printf("The value of n is %d\n", n);
        if(FD_ISSET(sockfd,&tmpset)){
            int len = sizeof(clientaddr);
            int clientsocket = accept(sockfd,
                    (struct sockaddr*)&clientaddr,&len);
            FD_SET(clientsocket,&sockets);
        } 
        else {
            int ii;
            for(ii=0; ii<FD_SETSIZE; ii++){
                if(FD_ISSET(ii,&tmpset)){
                    char line[5000];
                    int n = recv(ii,line,5000,0);

                    printf("Got from client: %s\n",line);
                    FD_CLR(ii,&sockets);
                }
            }
        }

    }
}
