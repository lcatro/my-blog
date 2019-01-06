## TTL(Time To Live,生存时间):



TTL意义:指定数据包被路由器丢弃之前允许通过的路由数量



如何理解:ICMP数据包每经过路由的时候ICMP头部数据中的TTL值便会减1,当TTL等于或小于0的时候路由器将不再转发这个数据包



假如一个TTL过期的ICMP数据包传递到路由,路由丢弃这个数据包而且向源主机返回类型3的ICMP数据包(意为:网络不可到达)



## Ping下对TTL更直观的了解



这是正常的ICMP测试



![](pic_temp7\psb1.jpg)



假设TTL值为10,ICMP数据包可以到达主机



![](pic_temp7\psb2.jpg)



TTL过小则不能到达远程主机,被路由丢弃



![](pic_temp7\psb3.jpg)



就连Ping命令也在提示,TTL值要大于0,否则就不给你发送ICMP数据包



![](pic_temp7\psb4.jpg)



有了上面的基础后,程序跟踪数据包经过路由的思路就出来了



## 程序思路



在发往IP地址的ICMP数据包头部中修改TTL从1到255值.接收到返回的ICMP数据包时IP头部会储存一个IP地址.当ICMP数据包不能达到目标主机时,这个地址则是在发送ICMP数据包经过某路由的IP地址,当ICMP数据包到达目标主机时,IP头部地址则是目的主机的IP地址



可以利用Ping命令和for命令写简单的跟踪路由测试



```sh
for /l %i in (1,1,255) do ping  -n 1 -i %i -w 2500 www.baidu.com
```



这只是一个粗糙的命令结合,它不会自动退出,测试到目标主机返回的数据也不会退出,还需要手动 CTRL+C 退出,略带麻烦..



对于C程序,为了简单易懂用IPhlpapi.dll 里面的ICMPCreateFile(),ICMPSendEcho(),ICMPCloseHandle()



为了方便大家查看函数的具体信息,这是百度百科ICMPSendEcho()用法: http://baike.baidu.com/view/6819233.htm?1345218087



## ICMP网络测试模块代码



ICMP网络测试主体代码:



```c++
#include "stdafx.h"      // VC++ 编译必须要这个头文件
#include "ICMPTest.h"

typedef DWORD  (__stdcall  *IcmpSendEcho)   (HANDLE,IPAddr,LPVOID,WORD,PIP_OPTION_INFORMATION,LPVOID,DWORD,DWORD);
typedef HANDLE ( __stdcall *IcmpCreateFile) (void);
typedef BOOL   ( __stdcall *IcmpCloseHandle)(HANDLE);

IcmpCreateFile  fIcmpCreateFile=NULL;
IcmpSendEcho    fIcmpSendEcho=NULL;
IcmpCloseHandle fIcmpCloseHandle=NULL;

HMODULE lModl=0;
HANDLE  ICMPHandle=INVALID_HANDLE_VALUE;
IPAddr  IP=0;
ip_option_information IPOpt;

bool InitFun(void)
{
	lModl=LoadLibrary ("Iphlpapi.dll");
	if (lModl!=0) 
	{
		fIcmpCreateFile   = (IcmpCreateFile) GetProcAddress (lModl,"IcmpCreateFile");
		fIcmpSendEcho     = (IcmpSendEcho)   GetProcAddress (lModl,"IcmpSendEcho");
		fIcmpCloseHandle  = (IcmpCloseHandle)GetProcAddress (lModl,"IcmpCloseHandle");   

	if (fIcmpCreateFile==NULL || fIcmpSendEcho==NULL || fIcmpCloseHandle==NULL)	
		return false;

	ICMPHandle=fIcmpCreateFile();
	if (ICMPHandle!=INVALID_HANDLE_VALUE)
	{
		memset(&IPOpt,0,sizeof(ip_option_information));
		IPOpt.Flags =0;
		IPOpt.OptionsData =NULL;
		IPOpt.OptionsSize =0;
		IPOpt.Tos =0;
		IPOpt.Ttl =255;
		return true;
	}
	else
		return false;
}
else
	return false;
}

void SetTestIP(char *IPAddr)
{
	IP=inet_addr((const char *)IPAddr);
}

void SetTestIPOption(int Flag=0,int TTL=256,int Tos=0,int OptionalSize=0,char *OptionsData=NULL)
{
	IPOpt.Flags =Flag;
	IPOpt.Ttl   =TTL;
	IPOpt.Tos   =Tos;
	IPOpt.OptionsSize =OptionalSize;
	IPOpt.OptionsData =(unsigned char *)OptionsData;
}

bool ICMPTest(int &TestStatu,icmp_echo_reply *pRtn)
{
	icmp_echo_reply Rtn;
	memset(&Rtn,0,sizeof(icmp_echo_reply));
	fIcmpSendEcho(ICMPHandle,IP,NULL,0,&IPOpt,(LPVOID)&Rtn,sizeof(icmp_echo_reply),0);
	TestStatu=Rtn.Status; 
	if (pRtn!=NULL) memcpy(pRtn,(const void *)&Rtn,sizeof(icmp_echo_reply));

if (Rtn.Status==ICMP_Success)
	return true;
else
	return false;
}

void CleanFun(void)
{
	fIcmpCloseHandle(ICMPHandle);
	fIcmpCreateFile   = NULL;
	fIcmpSendEcho     = NULL;
	fIcmpCloseHandle  = NULL;  
	FreeLibrary (lModl);
}
```



ICMP网络测试模块库文件:



```c++
#ifndef __ICMPTEST_H__
#define __ICMPTEST_H__

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define ICMP_Success     11010
#define ICMP_TTL_Transit 11013

typedef unsigned long IPAddr;
typedef struct ip_option_information {
  UCHAR  Ttl;
  UCHAR  Tos;
  UCHAR  Flags;
  UCHAR  OptionsSize;
  PUCHAR OptionsData;
} IP_OPTION_INFORMATION, *PIP_OPTION_INFORMATION;
typedef struct icmp_echo_reply {
  IPAddr                       Address;
  ULONG                        Status;
  ULONG                        RoundTripTime;
  USHORT                       DataSize;
  USHORT                       Reserved;
  PVOID                        Data;
  struct ip_option_information  Options;
} ICMP_ECHO_REPLY, *PICMP_ECHO_REPLY;

bool InitFun(void);
void SetTestIP(char *IPAddr);
void SetTestIPOption(int Flag,int TTL,int Tos,int OptionalSize,char *OptionsData);
bool ICMPTest(int &TestStatu,icmp_echo_reply *pRtn);
void CleanFun(void);

#endif
```



里面构造5个函数:



```c++
//  初始化:
bool InitFun(void);

//  设置ICMP测试IP地址:
void SetTestIP(char *IPAddr);

//  设置ICMP测试IP头部相关数据:
void SetTestIPOption(int Flag,int TTL,int Tos,int OptionalSize,char *OptionsData);

//  ICMP测试:  TestStatu是ICMP测试返回代码,pRtn可以获取更多细节数据
bool ICMPTest(int &TestStatu,icmp_echo_reply *pRtn);

//  关闭模块:
void CleanFun(void);
```



## 程序主体代码



有了写好的模块,程序主体代码看起来也就较美观,简洁



```c++
// ShadowICMPPacket.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ICMPTest.h"
#include <stdio.h>

char *InternetAddressToCharIP(unsigned long InternetAddress)
{
	struct in_addr addr;
	memcpy(&addr,&InternetAddress,4);
	return inet_ntoa(addr);
}

void main()
{
	InitFun();
	SetTestIP("220.181.112.143");
	int rtn=0;
	icmp_echo_reply G;
	memset(&G,0,sizeof(icmp_echo_reply));

	for (int i=1;i<=255;i++)
	{
		SetTestIPOption(0,i,0,0,NULL);
		ICMPTest(rtn,&G);
		if(rtn==11010)
		{
			printf("Target:%s \n",InternetAddressToCharIP(G.Address));
			break;
		}
		else
			printf("Route:%s \n",InternetAddressToCharIP(G.Address));
	}
	printf("Test OK!");

	CleanFun();
	return ;
}
```



注意函数:



```c++
char *InternetAddressToCharIP(unsigned long InternetAddress);
//  把网络字节顺序转换为标准IP字符串
```



ICMPSendEcho() 无论在调用失败还是成功都会有返回码,根据返回码11010 [测试成功]和11013 [TTL时间过短]来判断这是路由还是目标主机