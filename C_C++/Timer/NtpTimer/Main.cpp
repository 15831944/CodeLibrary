#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <Windows.h>

#define debug 1
#define TIMEOUT 3
#define JAN_1970 0x83AA7E80
#define NTP_SERVER_1 "202.120.2.101"    //cn.pool.ntp.org
#define NTP_SERVER_2 "122.226.192.4"  //cn.pool.ntp.org
#define NTP_SERVER_3 "218.75.4.130"   //cn.pool.ntp.org

#pragma comment(lib,"ws2_32.lib") 

void construct_ntp_packet(char content[])
{
    time_t           timer;

    memset(content, 0, 48);
    content[0] = 0x1b;             // LI = 0 ; VN = 3 ; Mode = 3 (client);
    
    time((time_t *)&timer);
    timer = htonl(timer + JAN_1970 );
    
    printf("construct_ntp_packet memcpy\n");
    memcpy(content + 40, &timer, sizeof(timer));  //trans_timastamp
    
    printf("construct_ntp_packet memcpy end\n");
}

int get_ntp_time(int sockfd, struct sockaddr_in *server_addr, struct tm *net_tm) 
{
    char           content[256];
    time_t         timet;
    long           temp;
    int            addr_len = 16;
    struct timeval block_time;
    fd_set         sockfd_set;

    FD_ZERO(&sockfd_set);
    FD_SET(sockfd, &sockfd_set);
    block_time.tv_sec = TIMEOUT;      //time out 
    block_time.tv_usec = 0;

    printf("construct_ntp_packet\n");
    
    construct_ntp_packet(content);
    
    printf("sendto\n");
    if (sendto(sockfd, content, 48, 0, (struct sockaddr *)server_addr, addr_len) < 0) {
#if debug
        printf("sendto error");
        printf("send error:%d\n", WSAGetLastError());
#endif
        return (-1);
    }

    printf("select\n");
    if(select(sockfd + 1, &sockfd_set, NULL, NULL, &block_time ) > 0) {
        if (recvfrom(sockfd, content, 256, 0, (struct sockaddr *)server_addr, (socklen_t *)&addr_len) < 0) {
#if debug
            perror("recvfrom error");
#endif
            return (-1);
        }
        else {
            memcpy(&temp, content + 40, 4);
            temp = (time_t)(ntohl(temp) - JAN_1970 );
            timet = (time_t)temp;
            memcpy(net_tm, gmtime(&timet), sizeof(struct tm));

            net_tm->tm_hour = net_tm->tm_hour + 8;  //beijing time zone
        }
    }
    else {
        return(-1);
    }
    return(0);
}

int main()
{
    int                  sockfd, i;
    struct tm            *net_tm;
    struct sockaddr_in   addr;
    char                 ip[4][16]= { { NTP_SERVER_1 } , { NTP_SERVER_2 }, { NTP_SERVER_3 } };
    char                 date_buf[50];
    
     SOCKET uiFdSocket;

   WSADATA wsaData;

   char szbuffer[1024] = "\0";

   struct sockaddr_in stServerAddr;

   struct sockaddr_in stClientAddr;

   int iAddrlen = sizeof(sockaddr_in);
   
   printf("WSAStartup\n");
   
    if (0 != WSAStartup(MAKEWORD(2,1), &wsaData)) 

    {

         printf("Winsock init failed!\r\n");

         WSACleanup();

         return 0;

    }

    printf("loop\n");
    net_tm = (struct tm *)malloc(sizeof(struct tm));

    for (i = 0 ; i < 3 ; i++ ) {
        memset(&addr, 0, sizeof(addr));
        addr.sin_addr.s_addr = inet_addr( ip[i] );
        addr.sin_port = htons(123);
        
         printf("loop %d\n", i);
         
        if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
#if debug
            printf("socket error");
#endif
            WSAGetLastError();
            return 0;
        }
    
        printf("get_ntp_time\n");
        if (get_ntp_time(sockfd, &addr, net_tm) == 0) {
            break;
        }

        closesocket(sockfd);
    }
    
    strftime(date_buf, sizeof(date_buf), "date -s \"%F %T\"", net_tm);
    system(date_buf);

    return (0);
}