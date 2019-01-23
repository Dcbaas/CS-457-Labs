#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void get_port(int* port);
void do_math(char* buffer, char* answer_buff);

int main(int argc, char **argv){
  int sockfd = socket(AF_INET,SOCK_DGRAM,0);

  if(sockfd < 0){
    printf("There was an error creating the socket\n");
    return 1;
  }

  int port_num;

  get_port(&port_num);

  struct timeval timeout;
  timeout.tv_sec= 3;
  timeout.tv_usec=0;
  setsockopt(sockfd,SOL_SOCKET,
	     SO_RCVTIMEO,&timeout,sizeof(timeout));
  
  struct sockaddr_in serveraddr,clientaddr;
  serveraddr.sin_family=AF_INET;
  serveraddr.sin_port=htons(port_num);
  serveraddr.sin_addr.s_addr=INADDR_ANY;

  bind(sockfd,(struct sockaddr*)&serveraddr,
       sizeof(serveraddr));

  socklen_t len = sizeof(clientaddr);
  char line[5000];
  char* answer;

  while(1){
    int n = recvfrom(sockfd,line,5000,0,
		(struct sockaddr*)&clientaddr,&len);
    if(n==-1){
      printf("Didn't get anything. Reset Timeout\n");
    } else {
      answer = (char*) malloc(5 * sizeof(char));
      printf("Sending result back");
      do_math(line, answer);
      sendto(sockfd, answer, 4, 0, 
          (struct sockaddr*)&clientaddr, sizeof(clientaddr));
      free(answer);
    }

  }
}

void get_port(int* port){
  printf("Enter Port Number: ");
  scanf("%d", port);
}

void do_math(char* buffer, char* answer_buff){
    int a;
    int b;
    int result;
    int fail = 0;
    char* err = "err";
    char* over = "ovr";
    memcpy(&a,&buffer[1], sizeof(int));
    memcpy(&b,&buffer[5], sizeof(int));
    switch(buffer[0]){
        case 'a': result = a + b; break;
        case 'm': result = a * b; break;
        case 'd': result = a / b; break;
        case 's': result = a - b; break;
        default: fail = 1;
    }
    if(!fail && ((a < 0 && b < 0 && result > 0) ||
    (a > 0 && b > 0 && result < 0))){
        fail = 1;
    }
    if(fail){
     memcpy(answer_buff, &err, 4);
     answer_buff[4] = 1;
    }
    else{
     memcpy(answer_buff, &result, 4);
    }
    for(int i = 0; i < 5; ++i){
      printf("%c", answer_buff[i]);
    }
    printf("\n");
}