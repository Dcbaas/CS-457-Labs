#include "file_utils.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>


int main(int argc, char** argv){
    //Verify command line args first.
    uint16_t portNum;
    if(argc != 2){
        printf("Usage: ./server <port number>\n");
        return 1;
    }
    else{
        portNum = atoi(argv[1]);
    }
    //Socket stuff 
    int serverSocket = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in serveraddr,clientaddr;

    //Select Stuff
    fd_set sockets;
    FD_SET(serverSocket,&sockets); 

    //File handling stuff
    //Buffer serves as use for the sockets as well and starts at 1000.
    char* buffer = (char*)malloc(1000*sizeof(char));
    char filename[1000];
    int fileSize;
    char fileSizeChar[4];

    //Starting the server
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(9876);
    serveraddr.sin_addr.s_addr=INADDR_ANY;
    bind(serverSocket,(struct sockaddr*)&serveraddr,
            sizeof(serveraddr));
    listen(serverSocket,10);
    while(1){
        fd_set tmpset=sockets;
        int n = select(FD_SETSIZE,&tmpset,NULL,NULL,NULL);
        if(FD_ISSET(serverSocket,&tmpset)){
            int len = sizeof(clientaddr);
            int clientsocket = accept(serverSocket,
                    (struct sockaddr*)&clientaddr,&len);
            FD_SET(clientsocket,&sockets);
        } else {
            int ii;
            for(ii=0; ii<FD_SETSIZE; ii++){
                if(FD_ISSET(ii,&tmpset)){
                    //Handle the file process
                    int n = recv(ii, filename, 1000, 0);

                    //Check for the file
                    fileSize = read_file(filename, &buffer);
                    //Error the file doesn't exist.
                    if(fileSize == -1){
                        printf("The file requested doesn't exist\n");
                    }
                    else{
                        memcpy(fileSizeChar, &fileSize, sizeof(int)); 
                        send(ii, fileSizeChar, sizeof(int), 0);
                        send(ii, buffer, fileSize, 0);
                        free(buffer);
                    }
                    FD_CLR(ii,&sockets);
                }
            }
        }
    }
    return 0;
}
