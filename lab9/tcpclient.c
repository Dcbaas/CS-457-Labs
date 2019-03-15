#include "file_utils.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char** argv){
    char* ipAddress;
    int portNum;
    if(argc != 3){
        printf("Usage: ./client <port number> <ip address>\n");
        return 1;
    }
    else{
        portNum = atoi(argv[1]);
        ipAddress = argv[2];
    }
    char filename[500];
    char* recvBuffer;
    int fileSize;
    char fileSizeChar[4];

    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0){
        printf("There was an error creating the socket\n");
        perror("Error: ");
        return 1;
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(portNum);
    serveraddr.sin_addr.s_addr=inet_addr(ipAddress);

    int c = connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
    if(c<0){
        printf("There was an error connecting\n");
        return 1;
    }

    printf("Enter a filename: ");
    scanf("%s", filename);

    send(sockfd,filename,strlen(filename)+1,0);
    int n = recv(sockfd, fileSizeChar, 4, 0);

    memcpy(&fileSize, fileSizeChar, 4);
// printf("The result of the the message was: %d\n", fileSize);
    if(fileSize != -1){
        //Create the output file name
        char* recvFolder = "recive/";
        char outfile[1000];
        strcat(outfile, recvFolder);
        strcat(outfile, filename);

        //Allocate the buffer space
        recvBuffer = (char*) malloc(fileSize * sizeof(char));
        n = recv(sockfd, recvBuffer, fileSize, 0);

        write_file(outfile, recvBuffer, fileSize);
        free(recvBuffer);
        printf("The file was put into the recive folder\n");
    }
    else{
        printf("The file specifed was not found\n");
    }
    close(sockfd);
    return 0;
}
