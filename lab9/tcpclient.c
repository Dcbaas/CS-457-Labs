#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char** argv){
  int sockfd = socket(AF_INET,SOCK_STREAM,0);
  if(sockfd < 0){
    printf("There was an error creating the socket\n");
    return 1;
  }

  struct sockaddr_in serveraddr;
  serveraddr.sin_family=AF_INET;
  serveraddr.sin_port=htons(9876);
  serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");

  int c = connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
  if(c<0){
    printf("There was an error connecting\n");
    return 1;
  }
  
  printf("Enter a message: ");
  char line[5000];
  fgets(line,5000,stdin);
  send(sockfd,line,strlen(line)+1,0);
  close(sockfd);
  return 0;
}
