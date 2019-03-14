#include "file_utils.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
    //Select Stuff
    fd_set sockets;
    FD_SET(serverSocket,&sockets); 


    if(serverSocket <0){
        printf("Error establishing socket\n");
        return 1;
    }

    struct sockaddr_in serveraddr,clientaddr;

    //File handling stuff
    //Buffer serves as use for the sockets as well and starts at 1000.
    char* buffer = (char*)malloc(1000*sizeof(char));
    char filename[1000];
    int fileSize;
    char fileSizeChar[4];

    //Starting the server
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(3456);
    serveraddr.sin_addr.s_addr=INADDR_ANY;
    
    bind(serverSocket,(struct sockaddr*)&serveraddr,
            sizeof(serveraddr));

    int result = listen(serverSocket,10);
    printf("The result of listen is %d\n", result);
    while(1){
        fd_set tmpset=sockets;
        int n = select(FD_SETSIZE,&tmpset,NULL,NULL,NULL);
        printf("The value of n is %d\n", n);
        printf("The value of FD_ISSET is %d\n", FD_ISSET(serverSocket, &tmpset));
        if(FD_ISSET(serverSocket,&tmpset)){
            printf("Added a select\n");
            socklen_t len = sizeof(clientaddr);
            printf("Pre accept\nh");
            int clientsocket = accept(serverSocket,
                    (struct sockaddr*)&clientaddr,&len);
            FD_SET(clientsocket,&sockets);
            printf("End of adding a select\n");
        } else {
            int ii;
            for(ii=0; ii<FD_SETSIZE; ii++){
                printf("%d\n", ii);
                if(FD_ISSET(ii,&tmpset)){
                    printf("Got a request\n");
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
        printf("End loop\n");
    }
    return 0;
}
