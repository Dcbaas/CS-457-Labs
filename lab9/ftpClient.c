#include "file_utils.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(int argc, char** argv){
    int portNum;
    char* ipAddress;

    if(argc == 3){
        portNum = atoi(argv[1]);
        ipAddress = argv[2];
    }
    else{
        printf("Usage: ./client <port number> <ip address>\n");
        return 1;
    }

    //Socket stuff
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(clientSocket < 0){
        printf("Failure to create the socket\n");
        return 1;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(portNum);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    //File stuff
    char filename[1000];
    char* recvBuffer = (char*)malloc(4*sizeof(char));
    int fileSize;

    int connection = connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if(connection < 0){
        printf("There was an error connecting to the server\n");
        return 1;
    }

    printf("What file do you want?: ");
    scanf("%s", filename);

    send(clientSocket, filename, strlen(filename) + 1, 0);
    int n = recv(clientSocket, recvBuffer, 4, 0);
    memcpy(&fileSize, recvBuffer, 4);

    recvBuffer = (char*)realloc(recvBuffer, fileSize * sizeof(char));

    n = recv(clientSocket, recvBuffer, fileSize, 0);

    //Save the file.
    char outfilename[1010];
    strcat(outfilename, "recive/");
    strcat(outfilename, filename);

    write_file(outfilename, recvBuffer, fileSize);
    close(clientSocket);
    return 0;
}
