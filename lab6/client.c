#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>

void get_port_and_address(int* port, char* ip, int ip_buffer_len);


int main(int argc, char** argv){
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    int port_num;
    char ip[20];

    if(sockfd < 0){
        printf("There was an error creating the socket\n");
        return 1;
    }
    get_port_and_address(&port_num, ip, 20);


    //Get an ivp4 address variable.
    struct sockaddr_in server_address;

    server_address.sin_family=AF_INET;
    server_address.sin_port = htons(port_num);
    server_address.sin_addr.s_addr = inet_addr(ip);

    char* handshake = "client connect";

    sendto(sockfd, handshake, strlen(handshake) + 1, 0,
            (struct sockaddr*) &server_address, sizeof(server_address));

    char buffer[5000];
    char inbuf[5000];

    socklen_t len = sizeof(server_address);
    int n, nn;

    fd_set myfds;
    FD_SET(sockfd,&myfds);
    FD_SET(STDIN_FILENO,&myfds);

    fflush(stdin);

    while(1){
        fd_set tmp = myfds;
        nn = select(FD_SETSIZE,&tmp,NULL,NULL,NULL);
        if(FD_ISSET(sockfd,&tmp)){
            n = recvfrom(sockfd, buffer, 5000, 0,
                    (struct sockaddr*)&server_address,&len);

            printf("Server says: %s\n", buffer);


            //For some reason I need to subtract off an extra
            //newline character in order for the comparison to work
            if(strcmp(buffer, "quit") - 10== 0 || strcmp(buffer, "Quit") - 10 == 0){
                printf("Server Disconneted. Quitting program\n");
                break;
            }
        }
        else if(FD_ISSET(STDIN_FILENO,&tmp)){
            fgets(inbuf,5000,stdin);
            sendto(sockfd, inbuf, strlen(inbuf)+1 , 0,
                    (struct sockaddr*)&server_address, sizeof(server_address));
            printf("Sent Message: %s\n", inbuf);


            //For some reason I need to subtract off an extra
            //newline character in order for the comparison to work
            if(strcmp(inbuf,"quit") - 10 == 0 || strcmp(inbuf, "Quit")- 10 == 0){
                printf("Exiting\n");
                break;
            }
        }
        fflush(stdin);
    }

}

void get_port_and_address(int* port, char* ip, int ip_buffer_len){
    printf("Enter a port: ");
    scanf("%d", port);
    fgetc(stdin);
    printf("Enter an IP:\n");
    fgets(ip, ip_buffer_len, stdin);
}

