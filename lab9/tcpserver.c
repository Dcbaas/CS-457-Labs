#include "file_utils.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>

int main(int argc, char **argv){
    int portNum;
    if(argc != 2){
        printf("Usage: ./server <port number>\n");
        return 1;
    }
    else{
        portNum = atoi(argv[1]);
    }
    char* sendBuffer;
    char filename[500];
    int fileSize;
    char sizeSender[4];

    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    fd_set sockets;
    FD_ZERO(&sockets);
    FD_SET(sockfd,&sockets);
    

    struct sockaddr_in serveraddr,clientaddr;
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(portNum);
    serveraddr.sin_addr.s_addr=INADDR_ANY;

    bind(sockfd,(struct sockaddr*)&serveraddr,
            sizeof(serveraddr));
    listen(sockfd,10);
    while(1){
        printf("while loop start\n");
        fd_set tmpset=sockets;
        int n = select(FD_SETSIZE,&tmpset,NULL,NULL,NULL);
        if(FD_ISSET(sockfd,&tmpset)){
            printf("FD_ISSET was true\n");
            int len = sizeof(clientaddr);
            int clientsocket = accept(sockfd,
                    (struct sockaddr*)&clientaddr,&len);
            FD_SET(clientsocket,&sockets);
        } 
        else {
            int ii;
            for(ii=0; ii<FD_SETSIZE; ii++){
                if(FD_ISSET(ii,&tmpset)){
                    //Recive the filename
                    int filenameLen = recv(ii,filename,5000,0);

                    //Find the file
                    fileSize = read_file(filename, &sendBuffer);
                    memcpy(sizeSender, &fileSize, sizeof(int));

                    //Send the result
                    send(ii, sizeSender, 4, 0);

                    //Did the file exist? If it did, send it.
                    if(fileSize > -1){
                        send(ii, sendBuffer, fileSize, 0);
                        printf("The file %s was sent\n", filename);
                    } 
                    else{ 
                        printf("The file %s didn't exist and wasn't sent\n", filename);
                    }

                    FD_CLR(ii,&sockets);
                }
            }
            free(sendBuffer);
        }

    }
}
