#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define _M_CEE_SAFE
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "wsock32.lib")

#include <time.h>

// ref: http://www.codeproject.com/KB/datetime/DateValidator.aspx

string GetHostIP(const string& strHost)
{
	//WSADATA wsa;
	//::WSAStartup(MAKEWORD(2, 0), &wsa);

	PHOSTENT hostinfo = gethostbyname(strHost.c_str());
	if(hostinfo != NULL)
	{
		//DWORD dwIP = (DWORD)htonl((*(struct in_addr *)*hostinfo->h_addr_list).S_un.S_addr);
		string strIP = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
		return strIP;
	}

	//::WSACleanup();
	return string("");
}


//设置套接字超时时间 
BOOL SetTimeout(SOCKET s, int nTime, BOOL bRecv) 
{ 
	int ret = ::setsockopt(s, SOL_SOCKET, bRecv ? SO_RCVTIMEO : SO_SNDTIMEO, (char *)&nTime, sizeof(nTime)); 
	return ret != SOCKET_ERROR; 
} 

int SntpTimerTest()
{
	vector<string> vecTimeSvr;
	vecTimeSvr.push_back("132.163.4.101");
	vecTimeSvr.push_back("64.90.182.55");
	vecTimeSvr.push_back("96.47.67.105");
	vecTimeSvr.push_back("165.193.126.229");
	vecTimeSvr.push_back("129.6.15.28");
	vecTimeSvr.push_back("129.6.15.30");


    WSADATA  Ws;
	if (WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
	{
		cout<<"Init Windows Socket Failed::"<<GetLastError()<<endl;
		return 0;
	}

	for(size_t i = 0; i < vecTimeSvr.size(); ++i)
	{
		string strHostName = vecTimeSvr[i];
		string strIP = strHostName; ///GetHostIP(strHostName);
		cout<<strHostName<<": "<<strIP<<endl;

		int SIZE_SOCKADDR_IN =sizeof(SOCKADDR_IN);
		sockaddr_in SvrIp;
		SvrIp.sin_family		= AF_INET;
		SvrIp.sin_addr.s_addr	= inet_addr(strIP.c_str());
		SvrIp.sin_port			= htons(123);

		int nLenRcv = 0;
		char szBuff[48] = {'\0'};
		szBuff[0] = 3 | (4<<3);

		SOCKET sSession = ::socket(AF_INET, SOCK_DGRAM, 0);
		SetTimeout(sSession, 3000, FALSE);
		if(SOCKET_ERROR != sendto(sSession, szBuff, 48, 0, (sockaddr*)&SvrIp, SIZE_SOCKADDR_IN))
		{
			SetTimeout(sSession, 3000, TRUE);
			nLenRcv = recvfrom(sSession, szBuff, 48, 0, (sockaddr*)&SvrIp, &SIZE_SOCKADDR_IN);
		}
		else
		{
			cout<<"send request failed!"<<endl;
		}
		closesocket(sSession);

		if(nLenRcv == 48)
		{
			time_t t = 0;
			for(int i = 40; i <= 43; i++)
			{
				unsigned char c = (unsigned char)szBuff[i];
				t = (t<<8) | c;
			}

			t -= 0x83AA7E80; //2209075200;	// 1970.1.1 0:0:0
			t += 8 * 60 * 60;	// UTC to Beijin

            //t = t - 0x83AA7E80;

			tm *_t = gmtime(&t);
			cout<<(_t->tm_year + 1900)<<"-"<<(_t->tm_mon + 1)<<"-"<<(_t->tm_mday)<<" "<<_t->tm_hour<<":"<<_t->tm_min<<":"<<_t->tm_sec<<endl;
		}
		else
		{
			cout<<"recieve data failed!"<<endl;
		}
		cout<<endl;
	}

	
	system("pause");
	return 0;
}
