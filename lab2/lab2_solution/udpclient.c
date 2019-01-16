//All network programs will need the first two .h files
//Std socket libraries
#include <sys/socket.h>
//Refers to std internet protocools 
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

void get_port_and_address(int* port, char* ip, int ip_buffer_len);

int main(int argc, char** argv){
  //sock for socket, fd file descripter.
  //AF_INET = want a socket where behind the scenes the operating system
  //will use an ivp4 protoocol 
  //Sock_DGRAM = will use UDP user datagram protoocl 
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  int port_num;
  char ip[20];

  if(sockfd < 0){
    printf("There was an error creating the socket\n");
    return 1;
  }

  struct timeval timeout;
  timeout.tv_sec=5;
  timeout.tv_usec=0;
  setsockopt(sockfd,SOL_SOCKET,
      SO_RCVTIMEO,&timeout,sizeof(timeout));

  get_port_and_address(&port_num, ip, 20);


  //Get an ivp4 address variable. 
  struct sockaddr_in server_address;

  server_address.sin_family=AF_INET;
  server_address.sin_port = htons(port_num);
  server_address.sin_addr.s_addr = inet_addr(ip);

  printf("Enter a message ");
  char line[5000];
  char received[5000];
  fgets(line, 5000, stdin);

  sendto(sockfd, line, strlen(line) + 1, 0, 
      (struct sockaddr*) &server_address, sizeof(server_address));

  int len = sizeof(server_address);
  int n = recvfrom(sockfd, received, 5000, 0,
      (struct sockaddr*)&server_address, &len);

  if(n == -1){
    printf("Nothing was echoed. Did you configure settings right?\n");
  }
  else{
    printf("Echoed back %s\n", received);


  }

  close(sockfd);

  return 0;

}

void get_port_and_address(int* port, char* ip, int ip_buffer_len){
  char throw;
  printf("Enter a port: ");
  scanf("%d", port);
  fgetc(stdin);
  printf("Enter an IP:\n");
  fgets(ip, ip_buffer_len, stdin);
}

