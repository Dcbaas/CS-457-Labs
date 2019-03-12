#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>

void get_port(int* port);

int main(int argc, char** argv){
    int port;

    get_port(&port);

    int sockfd = socket(AF_INET,SOCK_DGRAM,0);

    if(sockfd < 0){
        printf("There was an error creating the socket\n");
        return 1;
    }

    fd_set myfds;
    FD_SET(sockfd,&myfds);
    FD_SET(STDIN_FILENO,&myfds);

    struct sockaddr_in serveraddr,clientaddr;
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(port);
    serveraddr.sin_addr.s_addr=INADDR_ANY;

    bind(sockfd,(struct sockaddr*)&serveraddr,
            sizeof(serveraddr));

    char buffer[5000];
    char inbuf[5000];
    socklen_t len = sizeof(clientaddr);
    int nn;

    int n = recvfrom(sockfd,buffer,5000,0,
            (struct sockaddr*)&clientaddr, &len);

    fflush(stdin);

    if(strcmp(buffer, "client connect") == 0){
        printf("A client has connected start swearing!\n");
        while(1){
            fd_set tmp = myfds;
            nn = select(FD_SETSIZE,&tmp,NULL,NULL,NULL);
            if(FD_ISSET(sockfd,&tmp)){
                printf("Getting Data\n");
                n = recvfrom(sockfd, buffer, 5000, 0,
                        (struct sockaddr*)&clientaddr,&len);

                printf("Client says: %s\n", buffer);


                //For some reason I need to subtract off an extra 
                //newline character in order for the comparison to work/
                if(strcmp(buffer, "quit") - 10 == 0 || strcmp(buffer, "Quit") - 10 == 0){
                    printf("Client Disconneted. Quitting program\n");
                    break;
                }
            }
            else if(FD_ISSET(STDIN_FILENO,&tmp)){
                fgets(inbuf,5000,stdin);
                sendto(sockfd, inbuf, strlen(inbuf)+1, 0, 
                        (struct sockaddr*)&clientaddr, sizeof(clientaddr));
                printf("Sent Message: %s\n", inbuf);

                //For some reason I need to subtract off an extra 
                //newline character in order for the comparison to work
                if(strcmp(inbuf, "quit") - 10 == 0 || strcmp(inbuf, "Quit") - 10 == 0){
                    printf("Exiting\n");
                    break;
                }
            }
            fflush(stdin);
        }
    }
    close(sockfd);
}

void get_port(int* port){
    printf("Enter Port Number: ");
    scanf("%d", port);
}
