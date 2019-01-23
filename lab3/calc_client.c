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

  //I added a timeout for the client when
  //it looks for the response. This was for
  //convinence of testing when I messed
  //up inputing the correct ip and or port.
  struct timeval timeout;
  timeout.tv_sec=15;
  timeout.tv_usec=0;
  setsockopt(sockfd,SOL_SOCKET,
      SO_RCVTIMEO,&timeout,sizeof(timeout));

  get_port_and_address(&port_num, ip, 20);


  //Get an ivp4 address variable. 
  struct sockaddr_in server_address;

  server_address.sin_family=AF_INET;
  server_address.sin_port = htons(port_num);
  server_address.sin_addr.s_addr = inet_addr(ip);

  int operation;
  char op_code;

  int flag = 1;
  while(flag){
    printf("What operation do you want to perform?\n1. Add\n2. Subtract\n3. Multipy\n4. Divide\nAnswer: ");
    scanf("%d", &operation);
    flag = 0;
    if(operation < 0 || operation > 4){
      printf("Invalid Input\n\n");
      flag = 1;
    }
  }   
  switch(operation){
    case 1: op_code = 'a'; break;
    case 2: op_code = 's'; break;
    case 3: op_code = 'm'; break;
    case 4: op_code = 'd'; break;
    default:
            break;
  }

  int a, b;

  printf("Enter first operand: ");
  scanf("%d", &a);
  printf("Enter second operand: ");
  scanf("%d", &b);
  printf("Sending\n");

  char data[9];

  data[0] = op_code;
  memcpy(&data[1], &a, sizeof(int));
  memcpy(&data[5], &b, sizeof(int));

  sendto(sockfd, data, 9, 0,
      (struct sockaddr*) &server_address, sizeof(server_address));

  char result[5];

  socklen_t len = sizeof(server_address);
  socklen_t n = recvfrom(sockfd, result, 5, 0,
      (struct sockaddr*)&server_address, &len);


  if(n == -1){
    printf("Didn't get the answer");
    close(sockfd);
    return 1;
  }
  else{
    if(result[4] == 1){
      printf("error with operation");
      close(sockfd);
      return 2;
    }
    else{
      int result_val;
      memcpy(&result_val, result, sizeof(int));
      printf("The result is: %d\n",result_val);
      close(sockfd);
      return 0;
    }
    return 0;
  }



}

void get_port_and_address(int* port, char* ip, int ip_buffer_len){
  char throw;
  printf("Enter a port: ");
  scanf("%d", port);
  fgetc(stdin);
  printf("Enter an IP:\n");
  fgets(ip, ip_buffer_len, stdin);
}
