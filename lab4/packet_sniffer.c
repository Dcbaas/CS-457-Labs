#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <netpacket/packet.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <net/ethernet.h>

#define ETH_HEAD_SIZE 14
#define IP_HEAD_SIZE 20


int main(int argc, char** argv){
  int sniff = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

  struct sockaddr_ll addr;
  struct sockaddr_ll sniffaddr;

  sniffaddr.sll_family = AF_PACKET;
  sniffaddr.sll_protocol = htons(ETH_P_ALL);
  sniffaddr.sll_ifindex = if_nametoindex("h1-eth0");
  bind(sniff, (struct sockaddr*)&sniffaddr, sizeof(sniffaddr));

  while (1){
    int len = sizeof(addr);
    char buffer [5000];

    int n = recvfrom(sniff, buffer, sizeof(buffer), 0, (struct sockaddr*)&addr, &len);
    if(addr.sll_pkttype != PACKET_OUTGOING){
      printf("Got a packet\n");

      struct ether_header pack_header;
      memcpy(&pack_header, buffer, ETH_HEAD_SIZE);  
      printf("Destination %s\n",
          ether_ntoa((struct ether_addr*)&pack_header.ether_dhost));
      printf("Source %s\n",
          ether_ntoa((struct ether_addr*)&pack_header.ether_shost));

      printf("Type %d\n", ntohs(pack_header.ether_type));
      if(ntohs(pack_header.ether_type) == 0x800){
        struct iphdr ip_header;
        memcpy(&ip_header, &buffer[ETH_HEAD_SIZE], IP_HEAD_SIZE);
        struct in_addr source_addr = {ip_header.saddr};
        struct in_addr dest_addr = {ip_header.daddr};
        printf("Source IP Address: %s\n", inet_ntoa(source_addr));
        printf("Destination IP Address: %s\n\n\n", inet_ntoa(dest_addr));
      }
    }
  }
}
