#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void get_port(int* port);

int main(int argc, char **argv){
  int sockfd = socket(AF_INET,SOCK_DGRAM,0);

  if(sockfd < 0){
    printf("There was an error creating the socket\n");
    return 1;
  }

  int port_num;

  get_port(&port_num);

  struct timeval timeout;
  timeout.tv_sec=5;
  timeout.tv_usec=0;
  setsockopt(sockfd,SOL_SOCKET,
	     SO_RCVTIMEO,&timeout,sizeof(timeout));
  
  struct sockaddr_in serveraddr,clientaddr;
  serveraddr.sin_family=AF_INET;
  serveraddr.sin_port=htons(port_num);
  serveraddr.sin_addr.s_addr=INADDR_ANY;

  bind(sockfd,(struct sockaddr*)&serveraddr,
       sizeof(serveraddr));
  while(1){
    int len = sizeof(clientaddr);
    char line[5000];
    int n = recvfrom(sockfd,line,5000,0,
		     (struct sockaddr*)&clientaddr,&len);
    if(n==-1){
      printf("Didn't get anything. Reset Timeout\n");
    } else {
      printf("Got from client: %s\n",line);
      printf("Sending back the message\n");
      sendto(sockfd, line, strlen(line), 0, 
          (struct sockaddr*)&clientaddr, sizeof(clientaddr));

    }
  }
}

void get_port(int* port){
  printf("Enter Port Number: ");
  scanf("%d", port);
}
