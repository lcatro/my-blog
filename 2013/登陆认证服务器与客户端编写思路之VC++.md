

## 基础理论



1. 客户与线程



#### 轮询



    如果服务器只使用了一个线程来实现服务客户的话,那这个线程必须要对每个客户的Sock 进行数据传输和接收.



```c++
#define ERR_RECV_NULL 0
#define LEN_BUFFER_RECV 1024

void main(void)
{
    //...
    for (int i=0;i<Num_Online;i++){
        if (ERR_RECV_NULL!=recv(List_Sock[i],RecvBuffer,LEN_BUFFER_RECV,0)){
            //  根据客户传递进来的数据进行解析与处理
            send(List_Sock[i],(char *)&ClientResponed,sizeof(API_RES),0);
        }
    }
    //...
}
```



在这里,服务器需要根据储存在本地的所有客户Socket 标识符来轮流查询是否有客户发送请求数据到服务端.这样的设计是很简单的,不过如果有比较多的用户同时发送请求又或者是某些用户要处理的数据占用服务器时间比较长的话,那后面排队等待处理数据的用户将会一直等待直到服务器把它的请求数据处理完成为止.这时,等待的客户有可能等待3 秒,30 秒,甚至是30 分钟.而且轮询的方法会使服务器增加一点负担,以为它需要连续从TCP 接收数据缓冲上查询是否有新的数据,这样一来有很多不应该被消耗的CPU 时间被白白消耗了.不过轮询的方法写起来很简单,却也只能适用于客户少的情况下.



#### 多线程



用多线程来写服务器的话必须要保证主线程是"自由的",它不能作为通信模块的"傀儡",有很多时候,主线程需要去调配这些服务线程,比如:强踢某个用户下线,关闭服务器程序等功能.假如主线程也作为通信模块中的一部分,那主线程在调配服务线程时的设计将会很困难.一般地,服务器会给每个客户分配一个专用与服务它的线程.



```c++
#define LINE_MAX_LISTEN 2
void main(void)
{
    //...  //  服务器建立Winsock 代码
    Listen(LINE_MAX_LISTEN);    //  开始监听客户传递的连接
    Server_Run=true;    //  它是用来告诉Thread_Accept() 什么时候应该退出接收客户传递的连接
    Server_Thread_Accept=CreateThread(NULL,0,&ThreadAccept,NULL,0,NULL);
    //...
}

void WINAPI Thread_Accept(void *In)
{
    while (Server_Run){    //  如果主线程没有发出信号说要关闭服务器的话就一直监听
NewClient=accept(Sock,NULL,NULL);

if (INVALID_SOCKET!=NewClient)
    
if (Login_Check(NewClient)){    //  验证用户身份(Login_Chack() 后面会详细讲解)
                printf("New Client in it");    //  原则上来说这样是很危险的,下一节会详细讲解
                int EmptyIndex=FindEmptyThreadList();    //  搜索空的线程列表
                ServerThreadCount++;    //  当前服务线程计数
                ServerThreadList[EmptyIndex]=CreateThread(NULL,0,&Server_Thread,(LPVOID)&NewClient,0,NULL);

                //...
            }
    }
}

void WINAPI Server_Thread(void *In)
{
    while (Server_Run){
        if (ERR_RECV_NULL!=recv(List_Sock[i],RecvBuffer,LEN_BUFFER_RECV,0)){
            //  根据客户传递进来的数据进行解析与处理
            send(List_Sock[i],(char *)&ClientResponed,sizeof(API_RES),0);
        }
    }
}
```



主线程把接收客户连接和客户数据处理都分配给了独立的线程,使得服务程序的性能有所提高,而且,主线程还可以根据服务器管理员输入的命令来管理服务程序.



2. 线程之间的共享数据竞争



有很多时候,线程与线程之间需要相互使用某些数据,我们把它称之为共享数据,既然是相互使用,那就免不了会对共享数据进行写操作.现在请思考一下,如果线程1 正在读数据的时候突然间被线程2 修改了这个数据,那将会发生什么问题?



先来看看<<嵌入式系统软件教程>> 里面的一段经典代码:



```c
#define TEMP_ALARM 10
    
float Tempture_Inside=0.0;
float Tempture_Outside=0.0;

void main(void)
{
    //...
    if (CheckTemptureButton_Click()){    //  如果检查温度按钮被按下时
        if (Tempture_Inside>=Tempture_Outside+TEMP_ALARM){    //  如果核反应堆内外温差太大时就要报警
            Alarm_Beep();    //  启动警报
        }
    }
    //...
}

void interrupt GetTempture(void)
{
    //...
    Tempture_Outside=TemptureSensor_Outside();    //  通过外部温度传感器来获取温度
    Tempture_Inside=TemptureSensor_Inside();
    //...
}
```





怎么看这两段代码都没有错误.先来分析一下,GetTempture() 和main() 都共同使用了Tempture_Inside ,Tempture_Outside ,前者是对这两个变量进行写操作,后者是访问.理想情况下,温度数据是事先读好的然后再让main() 用来对比.但是GetTempture() 和main() 之间的切换却是"随时随地"的.假如在main() 将两个数据对比的时候突然间切换到GetTempture() 来改写数据呢?先把main() 里面的判断代码用汇编翻译过来.



```assembly
main()
  ;...
  LE R1,ADDR_TEMP_OUT    ;  根据Tempture_Outside 的地址从RAM 中读出数据到R1 寄存器中
  ADD R1,R1,#10                    ;  把R1 寄存器中的数据再多加10
  LE R2,ADDR_TEMP_IN        ;  根据Tempture_Inside 的地址从RAM 中读出数据到R2 寄存器中(注意这里!!)
  SUB R3,R2,R1                      ;  把R2 - R1 的值保存到R3 中
  BRn :ExitCheck                     ;  如果R3 为负数时就跳到:ExitCheck
  JRZ ...                                   ;  Call Alarm() ,调用Alarm()
:ExitCheck    ...
  ;...
\------------------
GetTempture()
  ;...
  JRZ ...                                      ;  调用TemptureSensor_Outside()
  ST R6,ADDR_TEMP_OUT       ;  把调用TemptureSensor_Outside() 的返回值放到RAM 中的Tempture_Outside
  AND R6,R6,#0                         ;  利用位运算来将R6 清0
  JRZ ...                                      ; Call TemptureSensor_Inside()
  ST R6,ADDR_TEMP_IN           ;  同上
  ;...
```



当main() 函数把ADD 指令运行完了,突然间切换到GetTempture() 去修改数据,这时GetTempture() 会把两个共享数据都给改写了,这将会与main() 读出来的数据有所不同(因为在ADD 指令之前main() 已经从内存中把Tempture_Outside 读到R1 寄存器中了).这是数据流动图.

 

![图片](pic_temp7\psb1.jpg)



即使这时内部温度和外部温度都相同,系统仍然会拉响警报!为什么呢?因为核反应堆里面的温度变化是非常快的,有可能1 秒内升高几十度.这反应堆升温的这段时间里,内,外部的温度都会升高,main() 中的R1 寄存器可能是1 秒前的数据,而R2 可能是1 秒后的数据,所以R2 比R1 大几十度的事情不是不可能会发生,于是就产生了系统的误报.



##  客户端



1. 建立Winsock



在建立Winsock 代码之前必须要对Winsock 库进行初始化.



```c++
#include <winsock.h>

#pragma comment (lib,"ws2_32")

void InitWinsock(void)
{
WSADATA WSAData;
memset(&WSAData,0,sizeof(WSADATA));
WSAStartup(VER_WINSOCK,&WSAData);

Server_IP=ResolveHost(HOST_SERVER);
}

void CleanWinsock(void)
{
WSACleanup();
}
```



初始完成后就要和服务器来连接



```c++
#define PORT_LOCAL  10131
#define PORT_SERVER 10132
    
bool ConnectServer(void)
{
if (NULL==Server_IP) return false;

sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

if (SOCKET_ERROR!=sock){
sockaddr_in local;
memset(&local,0,sizeof(sockaddr_in));
local.sin_family=AF_INET;
local.sin_port=htons(PORT_LOCAL);

if (SOCKET_ERROR!=bind(sock,(const sockaddr *)&local,sizeof(sockaddr_in))){
sockaddr_in remote;
memset(&remote,0,sizeof(sockaddr_in));
remote.sin_addr.S_un.S_addr=inet_addr(Server_IP);
remote.sin_family=AF_INET;
remote.sin_port=htons(PORT_SERVER);

if (SOCKET_ERROR!=connect(sock,(const sockaddr *)&remote,sizeof(sockaddr_in)))
return true;
}
}

closesocket(sock);
sock=SOCKET_ERROR;
return false;
}
```



2. 关于服务器IP 地址



由于客户端连接服务器需要一个IP 地址,对于IP 地址的指定不同人会有不同的设计方法,有人喜欢直接用IP 地址给客户程序,有人喜欢根据服务器的域名来获取服务器的IP 地址.后者的设计要比前者高明,因为服务器的IP 地址可能会由于许多原因(比如:服务器维修,重启后DHCP 分配新的IP 地址)会另它改变,而且用域名的方法还可以获取多个服务器的IP 地址.这里将使用根据域名解析IP 地址的方法来设计客户端获取服务器的IP 地址.



```c++
#define HOST_SERVER "server.test.cn"

char *Server_IP;

char *ResolveHost(const char *HostName)
{
hostent *HostData=NULL;
char *IP=NULL;
HostData=gethostbyname(HostName);
    
if (NULL!=HostData){
IP=inet_ntoa(*((struct in_addr *)HostData->h_addr_list[0]));    //  这里只返回解析成功第一个IP 地址
return IP;
}else
return NULL;
}
```



3. 用户登陆与登陆状态码



用户登陆到服务器时,服务器需要根据实际情况来对用户的登陆进行回应,比如:验证密码成功,服务器爆满等.细细列举了一下,有下面的值:



```c++
enum Server_Code{
Responed_Login_OK= 200,    //  登陆成功
Responed_Login_ERR= 201,    //  登陆错误
Responed_Login_STUFF= 202,    //  服务器爆满
Responed_Login_EXIT_CLIENT      = 203,    //  客户退出服务器
Responed_Login_EXIT_SERVER= 204,    //  服务器强踢用户下线
Responed_Login_ONLINE= 205,    //  用户已在线上
Responed_Login_NOUSER= 206,    //  该用户不存在
};
```



把它们折合成一个简单的登陆数据包:



```c++
enum Login_Code{           //  用户登陆代码
Login_In = 100,         //  用户登陆
Login_Out = 101,      //  用户退出
};

#define LEN_CLIENT_NAME 16
#define LEN_CLIENT_PASSWORD 16

#define LEN_SERVER_LOGIN sizeof(Server_Login)

struct Server_Login{
Login_Code Code;        //  无论是用户登陆还是退出,都必须要认证用户的身份,以防止恶意强制下线
char ClientName[LEN_CLIENT_NAME];
char ClientPassword[LEN_CLIENT_PASSWORD];
};

```



有了这些结构后,接下来就要编写用户登陆和退出的代码了:



```c++
struct Server_Packet{        //  服务器登陆认证返回数据包
Server_Code State;   //  回应代码
};

Server_Code RecvServer(void)
{
Server_Packet Packet;
memset(&Packet,0,sizeof(Server_Packet));

recv(sock,(char *)&Packet,sizeof(Server_Packet),0);    //  默认它是阻塞的
return Packet.State;
}

Server_Code LoginServer(const char *Name,const char *Password)
{
if (LEN_CLIENT_NAME<LEN_NAME || LEN_CLIENT_PASSWORD<LEN_PASSWORD)
return Responed_Login_ERR;

Server_Login Packet;
    
memset(&Packet,0,sizeof(Server_Login));
Packet.Code=Login_In;        //  填充用户登陆数据包
memcpy(&Packet.ClientName,Name,LEN_NAME);
memcpy(&Packet.ClientPassword,Password,LEN_PASSWORD);

send(sock,(const char *)&Packet,sizeof(Server_Login),0);

return RecvServer();
}

Server_Code ExitServer(const char *Name,const char *Password)
{
if (SOCKET_ERROR!=sock){
Server_Login Packet;
memset(&Packet,0,sizeof(Server_Login));

Packet.Code=Login_Out;        //  填充用户退出数据包
memcpy(&Packet.ClientName,Name,LEN_NAME);
memcpy(&Packet.ClientPassword,Password,LEN_PASSWORD);

send(sock,(const char *)&Packet,sizeof(Server_Login),0);

return RecvServer();
}


return Responed_Login_ERR;
}
```



最后在客户端上组合好这些代码:



```c++
#include "stdafx.h"
#include "Client_Sock.h"

#include <stdio.h>

#define STR_TEST_NAME "LCatro"
#define STR_TEST_PASSWORD STR_TEST_NAME

int main(int argc, char* argv[])
{
InitWinsock();


if (!ConnectServer()){
printf("Connect Server ERR!\n");
return 1;
}

Server_Code Rtn=LoginServer(STR_TEST_NAME,STR_TEST_PASSWORD);    //  用户登陆

if (Responed_Login_OK==Rtn){
printf("Login OK\n");

getchar();    //  让客户端卡一下

if (Responed_Login_EXIT==ExitServer(STR_TEST_NAME,STR_TEST_PASSWORD))    //  用户退出
printf("Exit Login OK\n");
else
printf("Exit ERR!\n");
}else
PrintLoginERR(Rtn);

CleanWinsock();
return 0;
}
```



最好应该留出一个专门用于打印错误的函数.



```c++
void PrintLoginERR(Server_Code In)
{
switch (In)
{
case Responed_Login_ERR:
printf("Password ERR!\n");
case Responed_Login_STUFF:
printf("Server STUFF!\n");
case Responed_Login_ONLINE:
printf("Client had Online!\n");
case Responed_Login_NOUSER:
printf("No Exist This Client!\n");
case Responed_Login_EXIT_SERVER:
printf("Server Kick!\n");
}
}
```



##  服务器



1. 线程分配与共享数据竞争的解决方案



在这个服务器中有三种线程:调配线程,服务线程,清理线程.调配和服务线程好理解,为什么要有清理线程呢?其实服务线程退出的时候是可以顺带清理些数据的,但是它不能关闭自己的线程句柄和清理堆栈数据(这可能会导致内存泄漏).它们之间的关系是这样的.



![图片](pic_temp7\psb2.jpg)



 线程之间的共享数据竞争还是有的.那我们应该怎么去解决它呢?



 让我们再来看看核反应堆的代码:



```c++
void main(void)
{
    //...
    if (CheckTemptureButton_Click()){
        if (Tempture_Inside>=Tempture_Outside+TEMP_ALARM){
            Alarm_Beep();
        }
    }
    //...
}
void interrupt GetTempture(void)
{
    //...
    Tempture_Outside=TemptureSensor_Outside();
    Tempture_Inside=TemptureSensor_Inside();
    //...
}
```



我们怎么样才可以在代码使用数据时检查是否其它的地方也在使用它呢?在此提出"锁"概念,就是把将要读/写的数据锁起来,让其它线程不可以来修改访问它,它只有某个线程读/写完共享数据之后才可以给另外的线程读/写.那这个"锁"的原理是怎么样的呢?

 

![图片](pic_temp7\psb3.jpg)



首先,main() 函数调用了LockShareData() ,突然间CPU 切换了线程,这个时候把代码执行权交给GetTempture() ,GetTempture() 也调用到了LockShareData() ,这个时候会有两种可能:1.LockShareData() 一直阻塞,直到CPU 把代码执行权交给其它线程,2.LockShareData() 检测到main() 



 已经把数据锁定,立即让CPU 把代码执行劝交回main() 函数来处理数据.显然,第二种方法要比第一种方法更高明,但它的实现却是很难的.在此为了简单明了一点就用第一种处理方法来写LockShareData() .它的机制是使用信号.



```c++
void LockShareData(void)
{
if (NULL!=BlockSignal_ShareData)        //  如果共享数据正在被使用
WaitForSingleObject(BlockSignal_ShareData,INFINITE);    //  阻塞等待它处理完成

BlockSignal_ShareData=CreateSemaphore(NULL,NUM_BLOCKSIGNAL_INIT,NUM_BLOCKSIGNAL_INIT,NULL);    //  创建新的阻塞信号
}

void UnockShareData(void)
{
ReleaseSemaphore(BlockSignal_ShareData,NUM_BLOCKSIGNAL_RELEASE,NULL);    //  释放这个信号
CloseHandle(BlockSignal_ShareData);    //  关闭这个信号
BlockSignal_ShareData=NULL;    //  清空信号
}
```



于是记得要在访问/修改线程间共享数据之前调用它们.如果你忘记了,那将会给你的服务器留下一个定时炸弹.



首先来编写服务端Winsock 初始化函数.



```c++
HANDLE BlockSignal_OnlineClient=NULL;

bool InitServer(void)
{
Sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

if (SOCKET_ERROR!=Sock){
sockaddr_in Local;
memset(&Local,0,sizeof(sockaddr_in));

Local.sin_family=AF_INET;
Local.sin_port=htons(PORT_SERVER);

if (SOCKET_ERROR!=bind(Sock,(const sockaddr *)&Local,sizeof(sockaddr_in))){
listen(Sock,NUM_LISTEN);

ThreadAccept=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)&Thread_Accept,NULL,CREATE_SUSPENDED,NULL);    //  注意这里有个CREATE_SUSPENDED ,代表线程创建好了并不立即运行,而是由服务器程序自己决定什么时候应该运行.如果线程在这里运行的话,那它有可能接收不到AcceptBlock 的阻塞信号导致Thread_Accept() 直接退出线程.其实是可以把设置AcceptBlock 信号放在创建线程的前面的,假如线程创建失败了呢?抑或还需要传递更多的数据到Thread_Accept() 呢?
StartCleanBlock();
CleanThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)&Clean_Thread,NULL,0,NULL);

if (NULL!=ThreadAccept && NULL!=CleanThread){
AcceptBlock=true;    //  控制Thread_Accept() 是否一直接收用户的连接
Run_Accept();    //  运行Thread_Accept()
return true;
}
}

closesocket(Sock);    //  注意,创建Sock 失败时记得要关闭Sock
Sock=SOCKET_ERROR;
}

return false;
}
```



2. Thread_Accept() 线程认证


Thread_Accept() 是专门用于接收用户传递进来的连接和分配服务线程的:



```c++
OnlineClientList OnlineClient;    //  在线用户数据列表

typedef void (*FUNC_REG_ACCEPT)(SOCKET);    //  回调函数类型


HANDLE ThreadAccept=NULL;

FUNC_REG_ACCEPT FuncAccept=NULL;

void RegAccept(FUNC_REG_ACCEPT FuncAddr)    //  注册接收用户成功函数回调函数
{
FuncAccept=FuncAddr;
}

void __stdcall Thread_Accept(void *in)
{
SOCKET NewClient=INVALID_SOCKET;

while (AcceptBlock){
NewClient=accept(Sock,NULL,NULL);

if (INVALID_SOCKET!=NewClient){
LockOnlineClientList();

if (Login_Chack(NewClient))
FuncAccept(NewClient);    //  注册成功则调用回调函数

UnlockOnlineClientList();
}
}
}
```



为了方便管理Thread_Accept() ,特意写了三个和它有关的函数:`Run_Accept(void)`, `Suspend_Accept(void)`,`Exit_Accept(void)`



```c++
void Run_Accept(void)
{
if (NULL!=ThreadAccept)
ResumeThread(ThreadAccept);
}

void Suspend_Accept(void)
{
if (NULL!=ThreadAccept)
SuspendThread(ThreadAccept);
}

void Exit_Accept(void)
{
if (NULL!=ThreadAccept){
Suspend_Accept();
AcceptBlock=false;
Set_Block(false);
Run_Accept();

CloseHandle(ThreadAccept);
ThreadAccept=NULL;
}
}
```



注意,在Thread_Accept() 里面有个客户身份验证函数`Login_Chack()` ,它的原形是:`bool Login_Chack(SOCKET NewClient);`



看看它是怎么验证用户数据的:



```c++
bool Login_Chack(SOCKET NewClient)
{
Server_Login Packet;
memset(&Packet,0,sizeof(Server_Login));

if (SOCKET_ERROR!=recv(NewClient,(char *)&Packet,sizeof(Server_Login),0)){    //  读取客户端发送过来的认证数据包
if (Login_In==Packet.Code){    //  判断这个数据包是否为登陆数据包
Check_Code Check=ClientCheck((const char *)&Packet.ClientName,(const char *)&Packet.ClientPassword);    //  验证用户身份(最后会有讨论)
if (Check_OK==Check){    //  验证成功
if (FIND_ERR==IsOnline((const char *)&Packet.ClientName)){    //  检查这个用户是否在线
if (LINE_MAX>OnlineClient.OnlineCount+1){    //  计算是否超过服务器处理连接的最大值
AddOnlineClient((const char *)&Packet.ClientName,(const char *)GetRemoteIP(NewClient),NewClient);

Login_Responed(NewClient,Responed_Login_OK);
return true;
}else
Login_Responed(NewClient,Responed_Login_STUFF);
}
else
Login_Responed(NewClient,Responed_Login_ONLINE);
}else if (Check_NOUSER==Check)    //  如果该用户不存在
Login_Responed(NewClient,Responed_Login_NOUSER);
else    //  如果验证失败
Login_Responed(NewClient,Responed_Login_ERR);
}
}

closesocket(NewClient);
return false;
}
```


请读者们注意代码`AddOnlineClient((const char *)&Packet.ClientName,(const char *)GetRemoteIP(NewClient),NewClient);`,它是用来添加新客户到在线客户总列表的.



```c++
long AddOnlineClient(const char *ClientName,const char *ClientIP,SOCKET ClientSock)
{
LockOnlineClientList();

OnlineClient.OnlineCount++;
long OnlineClientIndex=FindEmptyClientDataIndex();
memcpy(&OnlineClient.Client[OnlineClientIndex].Name,ClientName,LEN_CLIENT_NAME);
memcpy(&OnlineClient.Client[OnlineClientIndex].IPAddr,ClientIP,LEN_IP_V4);
OnlineClient.Client[OnlineClientIndex].ServerSock=ClientSock;
OnlineClient.Client[OnlineClientIndex].StatCode=Stat_Connect;

UnlockOnlineClientList();
return OnlineClientIndex;
}

void DeleteOnlineClient(long OnlienIndex)
{
memset(&OnlineClient.Client[OnlienIndex],0,LEN_CLIENT_DATA);
OnlineClient.OnlineCount--;
}

//    FindEmptyClientDataIndex() 是寻找在线客户总列表的空位置.
    
long FindEmptyClientDataIndex(void)
{
for (int i=0;i<LINE_MAX;i++)
if (Stat_NoConnect==OnlineClient.Client[i].StatCode)
return i;

return FIND_ERR;
}
```



请注意,为什么不在`AddOnlineClient()` 内判断`FindEmptyClientDataIndex()` 返回FIND_ERR 呢?因为在`Login_Chack()` 已经确定了接下来是必定有位置存放客户数据的.
 

3. 一些数据统计函数与强踢用户下线的编写



作为服务器的管理员,他很需要查看当前服务器工作情况,这就需要服务器程序来提供接口来显示程序当前的运行状况.



要实现最基本的功能,就告诉管理员当前有多少客户在服务器中和各个客户的数据.



```c++
#define LEN_IP_V4 16
#define LEN_CLIENT_DATA sizeof(ClientData)

struct ClientData{
Client_Stat StatCode;
SOCKET ServerSock;
char Name[LEN_CLIENT_NAME];
char IPAddr[LEN_IP_V4];
};
struct OnlineClientList{
long OnlineCount;    //  当前在线用户数
ClientData Client[LINE_MAX];    //  各个用户的数据
};

struct GetClientList{
long Count;
ClientData Client[LINE_MAX];
};
GetClientList *GetOnlineClient(void)
{
GetClientList *Rtn=(GetClientList *)malloc(sizeof(GetClientList));
memset(Rtn,0,sizeof(GetClientList));

LockOnlineClientList();    // <-- 注意其它线程在这儿会对它有I/O
long AllOnlineClient=OnlineClient.OnlineCount;

for (long i=0,AllocIndex=0;i<LINE_MAX;i++)
{
if (Stat_Connect==OnlineClient.Client[i].StatCode)
{
Rtn->Client[AllocIndex].StatCode=Stat_Connect;
memcpy(&Rtn->Client[AllocIndex].Name,(const void *)&OnlineClient.Client[i].Name,LEN_CLIENT_NAME);
memcpy(&Rtn->Client[AllocIndex].IPAddr,(const void *)&OnlineClient.Client[i].IPAddr,LEN_IP_V4);
AllocIndex++;
}
}

Rtn->Count=AllocIndex;
UnlockOnlineClientList();

return Rtn;
}
```



查看函数固然是很重要的,但我们不能忽略一个事实:"每个用户都有可能在服务器上散播不良信息",对于这种情况,服务器管理员有权把该用户踢下线.



```c++
void SetKickSingal(long OnlineIndex)
{
LockServerThreadData();
ServerKickSingal[OnlineIndex]=true;
UnlockServerThreadData();
}

bool KickOnlineClient(long OnlineIndex)
{
LockOnlineClientList();

if (Stat_Connect==OnlineClient.Client[OnlineIndex].StatCode){
Server_Packet Packet;
memset(&Packet,0,sizeof(Server_Packet));

Packet.State=Responed_Login_EXIT_SERVER;
send(OnlineClient.Client[OnlineIndex].ServerSock,(const char *)&Packet,sizeof(Server_Packet),0);
SetKickSingal(OnlineIndex);

closesocket(OnlineClient.Client[OnlineIndex].ServerSock);
DeleteOnlineClient(OnlineIndex);
UnlockOnlineClientList();

return true;
}

UnlockOnlineClientList();

return false;
}
```



踢用户下线的原理是向服务线程发送一个强踢信号,在发送信号之前首先要告诉客户:"你已经被服务器踢下线了".先看一下服务线程的代码.



```c++
long FindThreadIndex(SOCKET s)
{
for (int i=0;i<LINE_MAX;i++)
if (s==ServerSockList[i])
return i;

return FIND_ERR;
}

void __stdcall Server_Thread(SOCKET In)    // <- 注意,这里没有利用指针来获取数据
{
SOCKET Sock=In;
int RecvLen=0;
char Buffer[LEN_BUFFER]={0};
long ThreadIndex=FindThreadIndex(Sock);

while(true){
RecvLen=recv(Sock,Buffer,LEN_BUFFER,0);

LockServerThreadData();
if (true==ServerKickSingal[ThreadIndex]){    //  <-  注意,服务线程判断管理员是否发送了强踢信号
UnlockServerThreadData();
break;
}
UnlockServerThreadData(); 

if (LEN_SERVER_LOGIN==RecvLen){
Server_Login Exit;
memset(&Exit,0,LEN_SERVER_LOGIN);
memcpy(&Exit,(const void *)&Buffer,LEN_SERVER_LOGIN);

if (Login_Out==Exit.Code){    //  客户主动下线
Login_Exit(Sock);
break;
}
}
}

CleanMe(ThreadIndex);
}
```



在这里顺便给出清理线程代码:



```c++
void CleanMe(long ServerThreadIndex)    // <- 由于线程在运行的时候不能清理和自己有关的数据,所以需要发信号到清理线程

{

CleanThreadIndex=ServerThreadIndex;

ExitCleanBlock();    // <- 退出清理线程阻塞

}

void __stdcall Clean_Thread(void *In)

{

while(Server_Run){

WaitForSingleObject(Signal_Clean,INFINITE);



if (INVALID_THREAD_INDEX!=CleanThreadIndex){

Sleep(TIME_WAIT_THREAD);

LockServerThreadData();

closesocket(ServerSockList[CleanThreadIndex]);

CloseHandle(ServerThreadList[CleanThreadIndex]);



ServerSockList[CleanThreadIndex]=SOCKET_ERROR;

ServerKickSingal[CleanThreadIndex]=false;

ServerThreadList[CleanThreadIndex]=NULL;

ServerThreadCount--;

CleanThreadIndex=INVALID_THREAD_INDEX;

UnlockServerThreadData();

}

StartCleanBlock();    // <- 启动清理线阻塞

}

}
```



清理线程的阻塞是有必要的,因为它根本不知道什么时候应该进行数据清理.



```c++
void StartCleanBlock(void)

{

if (NULL!=Signal_Clean) return;



Server_Run=true;

Signal_Clean=CreateSemaphore(NULL,NUM_BLOCKSIGNAL_INIT,NUM_BLOCKSIGNAL_INIT,NULL);

}



void ExitCleanBlock(void)

{

Server_Run=false;

ReleaseSemaphore(Signal_Clean,NUM_BLOCKSIGNAL_RELEASE,NULL);

CloseHandle(Signal_Clean);

Signal_Clean=NULL;

}
```



4. 关于用户身份验证的方法讨论 



亲们,关于这个客户身份验证应该怎么写呢?我的代码是这样的:



```c++
enum Check_Code{
Check_OK = 100,
Check_ERR = 101,
Check_NOUSER = 102,
};

#define TEST_NAME "LCatro"
#define TEST_PASSWORD TEST_NAME

#if _DEBUG    // <- 如果是调试状态下的话

Check_Code ClientCheck(const char *Name,const char *Password)
{
if (0==strcmp(TEST_NAME,Name) && 0==strcmp(TEST_PASSWORD,Password))
return Check_OK;

return Check_ERR;
}
#else

Check_Code ClientCheck(const char *Name,const char *Password)
{
long ClientIndex=FindClientIndex_AllList(Name);
if (FIND_ERR==ClientIndex)
return Check_NOUSER;    // <- 该用户不存在

char *Chack_Password=GetUserPassword(ClientIndex);

if (0==strcmp(Chack_Password,Password))
return Check_OK;

return Check_ERR;
}

#endif
```



在调试状态下的代码读者们都可以理解,但是编译状态下的代码又是怎么样运作的呢?当时的设想是想把用户数据映射到内存里的,然后根据客户传递进来的用户名和密码来验证.先看看文件结构:



```txt
Client File Struct:

________________________   <-  BaseAddress

|All Client Data Statistics|

|--------------------------------------|  <-  DataAddress

|Client 1 Data        |

|--------------------------------------|

|Client 2 Data        |

|--------------------------------------|

|...        |

\--------------------------------------
```


在这里,每个客户的数据都是用Server_Login 结构来保存的,那Server_Login.Code 该怎么设置?其实这个标志并不会影响什么,服务器程序是依靠用户名和用户密码来验证的,所以它也就只充当了占用内存的作用.接下来是映射内存的主要代码:



```c++
struct DataStatistics{

long AllClientCount;

};



#define LEN_DATASTATISTICS sizeof(DataStatistics)



HANDLE ClientFile=NULL;

HANDLE MappingFile=NULL;

LPVOID BaseAddress=NULL;

LPVOID DataAddress=NULL;

void CleanDataStatistics(void)

{

memset(&AllClient,0,LEN_DATASTATISTICS);

}

void MappingClean(void)

{

if (NULL!=BaseAddress){

UnmapViewOfFile((LPCVOID)BaseAddress);

BaseAddress=NULL;

DataAddress=NULL;



CleanDataStatistics();

}

if (INVALID_HANDLE_VALUE!=MappingFile){

CloseHandle(MappingFile);

MappingFile=NULL;

}

if (INVALID_HANDLE_VALUE!=ClientFile){

CloseHandle(ClientFile);

ClientFile=NULL;

}

}
 

bool MappingClientFile(void)

{

if (NULL!=ClientFile)

return false;



ClientFile=CreateFile(STR_CLIENT_FILE_PATH,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);

if (INVALID_HANDLE_VALUE!=ClientFile){

unsigned long FileSize_Hige=0;

long FileSize_Low=GetFileSize(ClientFile,&FileSize_Hige);



MappingFile=CreateFileMapping(ClientFile,NULL,PAGE_READONLY,FileSize_Hige,FileSize_Low,STR_CLIENT_FILE_NAME);

if (INVALID_HANDLE_VALUE!=MappingFile){

BaseAddress=MapViewOfFile(ClientFile,FILE_MAP_READ,FileSize_Hige,FileSize_Low,FileSize_Low);

if (NULL!=BaseAddress){

CleanDataStatistics();

memcpy(&AllClient,(const void *)BaseAddress,LEN_DATASTATISTICS);



DataAddress=BaseAddress+LEN_DATASTATISTICS;

}



return true;

}

}

}



MappingClean();

return false;

}
```



有了映射内存,还需要寻找到客户的数据是再哪儿保存的.



```c++
long FindClientIndex_AllList(const char *Name)    // <- 从内存中寻找目的用户
{
if (NULL==DataAddress)
return FIND_ERR;

Server_Login *Point=NULL;
for (int i=0;i<AllClient.AllClientCount;i++){
Point=(Server_Login *)((long)DataAddress+sizeof(Server_Login)*i);
if (0==strcmp((const char *)&Point->ClientName,Name))
return i;
}

return FIND_ERR;
}

char *GetUserPassword(long ClientIndex)
{
Server_Login *Point=(Server_Login *)((long)DataAddress+sizeof(Server_Login)*ClientIndex);

return (char *)&Point->ClientPassword;
}
```



内存映射的方法是很简便,不过它有一个弊端,就是怎么样才可以把新注册的用户都放到这儿来呢?这是一个问题,望各路大神指点指点.

