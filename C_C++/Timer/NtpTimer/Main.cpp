#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <Windows.h>
#include <iostream>

#define debug 1
#define TIMEOUT 3
#define JAN_1970 0x83AA7E80
#define NTP_SERVER_1 "202.120.2.101"    //cn.pool.ntp.org
#define NTP_SERVER_2 "122.226.192.4"  //cn.pool.ntp.org
#define NTP_SERVER_3 "218.75.4.130"   //cn.pool.ntp.org

#pragma comment(lib,"ws2_32.lib") 

using namespace std;

const int SvrAddrCountTime = 5;

char SvrAddrTime[SvrAddrCountTime][20] = {
    "132.163.4.101", 
    "64.90.182.55",
    "96.47.67.105",
    "165.193.126.229",
    "129.6.15.28"
};


int TimeProtocol()
{
    cout << "====time protocol start====" << endl;

    WSADATA  Ws;
	if (WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
	{
		cout<<"Init Windows Socket Failed::"<<GetLastError()<<endl;
		return 0;
	}
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	if (s == INVALID_SOCKET) 
	{ 
		std::cout<<"create socket error"<<std::endl; 
		return 0;  
	} 
	sockaddr_in servAddr;
	memset(&servAddr,0,sizeof(sockaddr_in));
	servAddr.sin_family = AF_INET; 
	servAddr.sin_port = htons(37); 
	

    char *pSvrAddr;
    int idx;
    for(idx = 0; idx < SvrAddrCountTime; ++idx){
        pSvrAddr = SvrAddrTime[idx];

        servAddr.sin_addr.S_un.S_addr = inet_addr(pSvrAddr);
        if (SOCKET_ERROR == connect(s, (sockaddr *)&servAddr, sizeof(servAddr))) 
	    { 
		    std::cout<<"connect socket error. idx=" << idx << ", addr=" << pSvrAddr <<std::endl; 
		    continue; 
	    }

	    ULONG ulTime = 0; 
	    int nRecv = recv(s, (char *)&ulTime, sizeof(ulTime), 0); 
	    std::cout<<ulTime<<std::endl;
	    ulTime = ntohl(ulTime) - JAN_1970; 
	    closesocket(s); 

	    std::cout<<ulTime<<std::endl;

	    cout<<"----------------------"<<endl;
	    time_t timep = ulTime;
	    struct tm *t;
	    t = gmtime(&timep);
	    printf("%04d-%02d-%02d %02d:%02d:%02d\n"
		    , t->tm_year + 1900
		    , t->tm_mon +1
		    , t->tm_mday
		    , t->tm_hour +8
		    , t->tm_min
		    , t->tm_sec);
    }
 
    cout << "====time protocol end====" << endl;

    return 0;
}

int main()
{
    TimeProtocol();

    cout << "end." << endl;
    int c;
    cin >> c;

    return (0);
}