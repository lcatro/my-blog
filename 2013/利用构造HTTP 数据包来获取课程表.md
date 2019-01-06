

##  实现前的理论依据



1. HTTP 数据包


  HTTP 协议有很多种数据包,每不同种类的数据包功能也不尽相同,用GET 和POST 来举例(因为这里就只用这两个),前者是从服务器上获取数据而后者是向服务端提交数据,所以在编程的时候要根据需要实现的功能来填充数据包.



  在HTTP 传输时必须要注意数据包里面的Connection: 请求选项,由于在HTTP 协议1.1 版本下的默认值是Keep Alive ,所以在传输结束时需要注意是不是还继续和服务端建立了连接,由于是服务器主动提出和客户端关闭请求的,所以在这个时候如果客户端先closesocket() 的话,那该socket 绑定的端口将会进入TIME_WAIT 状态,经测试,即使在setsockopt() 下设置了端口重用选项也还是不能重新创建新端口,如果还想重用,就等服务器MSL x2 时间过了吧.. 



2. 登陆系统时的HTTP 数据流情况



  下面的截图来自于HTTP 截包器..



  首先,我在登陆系统的时候输入用户名:0000,密码:0000



![图片](pic_temp6\psb1.webp.jpg)



```http
POST /sise/login_check_login.jsp HTTP/1.1
Accept: text/html, application/xhtml+xml, */*
Referer: <http://class.sise.com.cn:7001/sise/>
Accept-Language: zh-CN
User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0)
Content-Type: application/x-www-form-urlencoded
UA-CPU: AMD64
Accept-Encoding: gzip, deflate
Host: class.sise.com.cn:7001
Content-Length: 93
Connection: Keep-Alive
Cache-Control: no-cache
Cookie: JSESSIONID=SakbuQ9cXeAda2i6cdxLXTyF0ox9XpHDWLOCYO3H5GkfmEzLX3Bh!-1487795243

056fcba9547103508f992ae9396208b6=9bc6fb3a8875b74b1b6207b27b319d6b&username=0000&password=0000
```



附加数据里面有两个很关键的地方,就是`&username= ,&password=`.它倆用处你懂的.



```http
HTTP/1.1 200 OK
Date: Sun, 13 Oct 2013 13:49:32 GMT
Server: WebLogic Server 8.1 SP2 Fri Dec 5 15:01:51 PST 2003 316284 
Content-Length: 159
Content-Type: text/html; charset=GBK

<script>top.location.href='common/error.jsp?messages=系统数据库中找不到你的数据哦！！！ 你输入的是[0000] 毕业生或已完成结算了的也不行哦！';</script>
```



这个就是输入错误密码时的返回的数据包



然后收到了它就会自动跳到一个新窗口,这个就是GET .

 

![图片](pic_temp6\psb2.webp.jpg)



然后输入正确的用户和密码.



![图片](pic_temp6\psb3.webp.jpg)



由于在操作的时候连续做了两个动作,分别是:提交用户名和密码,进入课程表页面,所以在这里就拦截到两个GET 数据包.先来细细分析.



![](pic_temp6\psb4.webp.jpg)




这个GET 是获取华软的Mysise 系统的首页HTML 代码.



![图片](pic_temp6\psb5.webp.jpg)



这个GET 是获取Mysise 系统里面的课程表的HTML 代码.对应的HTTP 的GET 数据包:



```http
GET /sise/module/student_schedular/student_schedular.jsp HTTP/1.1
Accept: text/html, application/xhtml+xml, */*
Referer: <http://class.sise.com.cn:7001/sise/module/student_states/student_select_class/main.jsp>
Accept-Language: zh-CN
User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0)
UA-CPU: AMD64
Accept-Encoding: gzip, deflate
Host: class.sise.com.cn:7001
Connection: Keep-Alive
Cookie: JSESSIONID=SakbuQ9cXeAda2i6cdxLXTyF0ox9XpHDWLOCYO3H5GkfmEzLX3Bh!-1487795243
```



  然后服务器就会返回课表的HTML 代码.



##  编程实现



  开篇语:"代码还是规范点好".



#### TCP Socket 的搭建和recv()



```c++
#include <winsock.h>

#pragma comment (lib,"ws2_32")

#define VER_WINSOCK 1

#define PORT_HTTP 7001
#define PORT_LOCAL 10131
#define LEN_HTTP_BUFFER 65532
#define URL_TARGE "class.sise.com.cn"
 
void InitWinsock(void)
{
	WSADATA WSAData;
	memset(&WSAData,0,sizeof(WSADATA));
	WSAStartup(VER_WINSOCK,&WSAData);
}

void CleanWinsock(void)
{
	WSACleanup();
}
```







  你可能会感到惊奇,为什么服务器的端口会是7001 而不是80 呢?呵呵,这个问题应该去问网络中心哈..



  由于是用URL 来连接的,所以还必须支持解析URL 到IP 地址.[为什么要这样做? - 支持用IP 地址也可以,但服务可能会不定时换IP 地址又或以后改变域名]



```c++
char *ResolveHost(const char *HostName)
{
	hostent *HostData=NULL;
	char *IP=NULL;
	HostData=gethostbyname(HostName);
    
	if (NULL!=HostData){
		IP=inet_ntoa(*((struct in_addr *)HostData->h_addr_list[0]));
		return IP;
	}else
		return NULL;
}
```



  然后就是程序入口点代码:



```c++
int main(int argc, char* argv[])
{
	InitWinsock(); 
	SOCKET sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	if (SOCKET_ERROR!=sock){
		sockaddr_in local;
		memset(&local,0,sizeof(sockaddr_in));
		local.sin_family=AF_INET;
		local.sin_port=htons(PORT_LOCAL);

		if (SOCKET_ERROR!=bind(sock,(const sockaddr *)&local,sizeof(sockaddr_in))){
			sockaddr_in remote;
			memset(&remote,0,sizeof(sockaddr_in));
			char *remoteIP=ResolveHost(URL_TARGE);

			if (NULL!=remoteIP){
				remote.sin_addr.S_un.S_addr=inet_addr(remoteIP);
				remote.sin_family=AF_INET;
				remote.sin_port=htons(PORT_HTTP);

				if (SOCKET_ERROR!=connect(sock,(const sockaddr *)&remote,sizeof(sockaddr_in)))
				{
                                       //  Add your socket work code at it ..

				}
			}
		}
	}

	closesocket(sock);
	CleanWinsock();

	return 0;
}
```



  好了,至此一个简单的Socket 流程已经建成,现在想要给各位读者们提一下"宏编程的重要性".其实,代码不只应该写给机器读,最需要读的应该是人,无论是开源还是维修项目,可读性高的代码人人都喜欢.用宏和不用宏来编程是两个境界,打个比喻就好像"你老乡跟你说一句话用几种语言"一样.



  还有一个需要注意的就是recv() .由于课表的HTML 代码长度很大,大于LEN_HTTP_BUFFER ,所以程序不得不使用while() 来循环读取数据流,于是这就产生了一个问题,如果把数据都读完了然后再读一次呢?程序就会产生阻塞,进入"卡死状态".解决它的最好的方法就是:



```c++
timeval block;

memset(&block,0,sizeof(timeval));

block.tv_sec=1;
block.tv_usec=0;

setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(const char *)&block,sizeof(timeval));
```



  recv() 的接收延时设为0 ,这就意味着接下来的recv() 将会是无阻塞状态.阻塞问题就这样解决了.



#### 数据包的构造



  在这儿,程序只需要构造HTTP POST 数据包以提交输入的用户名和密码.



```c++
#define HTTP_DATA_1 "POST /sise/login_check_login.jsp HTTP/1.1\r\n" \
		"Accept: text/html, application/xhtml+xml, */*\r\n" \
		"Accept-Language: zh-CN\r\n" \
		"User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64;x64; Trident/5.0)\r\n" \
		"Content-Type: application/x-www-form-urlencoded\r\n" \
		"UA-CPU: AMD64\r\n" \
		"Accept-Encoding: gzip, deflate\r\n" \
		"Host: class.sise.com.cn:7001\r\n" \
		"Content-Length: "

#define HTTP_DATA_2 "\r\n" \
		"Connection: Keep-Alive\r\n" \
		"Cache-Control: no-cache\r\n" \
		"Cookie: JSESSIONID=Sb3B1Znt3wOHuB2SyHUbfjrHJJuZf93l53WZlFtwXVW7bLT63xY2!1996935212\r\n\r\n"

#define HTTP_DATA_3	056fcba9547103508f992ae9396208b6=9bc6fb3a8875b74b1b6207b27b319d6b"

#define HTTP_DATA_USERNAME "&username="
#define HTTP_DATA_PASSWORD "&password="

#define LEN_HTTP_DATA_1 strlen(HTTP_DATA_1)
#define LEN_HTTP_DATA_2 strlen(HTTP_DATA_2)
#define LEN_HTTP_DATA_3 strlen(HTTP_DATA_3)
#define LEN_HTTP_DATA_USERNAME strlen(HTTP_DATA_USERNAME)
#define LEN_HTTP_DATA_PASSWORD strlen(HTTP_DATA_PASSWORD)
#define LEN_HTTP_USERNAME strlen(Username)
#define LEN_HTTP_PASSWORD strlen(Password)
#define LEN_HTTP_LENGTH_ALLOC 3
#define LEN_HTTP_LEN_INT strlen((const char *)Length)
#define LEN_HTTP LEN_HTTP_DATA_1+LEN_HTTP_DATA_2+LEN_HTTP_DATA_3+LEN_HTTP_DATA_USERNAME+LEN_HTTP_USERNAME+LEN_HTTP_DATA_PASSWORD+LEN_HTTP_PASSWORD+LEN_HTTP_LENGTH_ALLOC
 
#define LEN_HTTP_OTHER_DATA LEN_HTTP_DATA_3+LEN_HTTP_DATA_USERNAME+LEN_HTTP_USERNAME+LEN_HTTP_DATA_PASSWORD+LEN_HTTP_PASSWORD
 
#define LEN_HTTP_CRLF 2
char *AllocLoginPacket(const char *Username,const char *Password)
{
	char *Packet=(char *)malloc(LEN_HTTP+LEN_HTTP_CRLF*2);
	memset(Packet,0,LEN_HTTP+LEN_HTTP_CRLF*2);

	char *Length=IntToChar(LEN_HTTP_OTHER_DATA);
sprintf(Packet,"%s%s%s%s%s%s%s%s\r\n\r\n",HTTP_DATA_1,Length,HTTP_DATA_2,HTTP_DATA_3,HTTP_DATA_USERNAME,Username,HTTP_DATA_PASSWORD,Password);
	free(Length);
	return Packet;
}
```




  **Q1:为什么要一个POST 数据包要拆分成三段#define HTTP_DATA 来写呢?**
  **A1:这是为了方便在下面的程序实现填充功能.**



  **Q2:为什么Packet 的大小是LEN_HTTP+LEN_HTTP_CRLF*2 呢?**

  **A2:LEN_HTTP_CRLF*2 是为了补上该行结束标志和HTTP 数据包发送结束标志.**



  **Q3:char *Length=IntToChar(LEN_HTTP_OTHER_DATA); 有什么作用?**

  **A3:请看一下POST 头:"Content-Length: 93",它的意思是说从这段的结尾到HTTP 数据包的还有多长.注意,这不是int 数据,而是ASCII 码(很简单,一般的int 是做不到的[除非你用%d ],而HTTP 数据本身就是字符串,这些字符串还可以直接连数字输出到窗口中,也只有ASCII 才可以这样做).**



```c++
char *IntToChar(int num)
{
	char *Rtn=(char *)malloc(4);
	if (100<=num)
		sprintf(Rtn,"%d",num);
	else if(10<=num)
		sprintf(Rtn,"%d",num);
	else if(0<=num)
		sprintf(Rtn,"%d",num);

	return Rtn;
}
```



  哎哟!你逗我呢?这个malloc(4) 是怎么一回事?哈,假如对这段代码换一种写法.



```c++
#define LEN_INT 4

char *IntToChar(int num)
{
	char *Rtn=(char *)malloc(LEN_INT);
	if (100<=num)
		sprintf(Rtn,"%d",num);
	else if(10<=num)
		sprintf(Rtn,"%d",num);
	else if(0<=num)
		sprintf(Rtn,"%d",num);

	return Rtn;
}

```



  是不是通俗易懂了呢?



  登陆成功和失败主要体现在服务器返回的数据包中.



成功:



```http
HTTP/1.1 200 OK
Date: Sun, 13 Oct 2013 13:49:32 GMT
Server: WebLogic Server 8.1 SP2 Fri Dec 5 15:01:51 PST 2003 316284 
Content-Length: 159
Content-Type: text/html; charset=GBK

<script>top.location.href='/sise/index.jsp'</script>
```



失败:



```http
HTTP/1.1 200 OK
Date: Sun, 13 Oct 2013 13:49:32 GMT
Server: WebLogic Server 8.1 SP2 Fri Dec 5 15:01:51 PST 2003 316284 
Content-Length: 159
Content-Type: text/html; charset=GBK

<script>top.location.href='common/error.jsp?messages=系统数据库中找不到你的数据哦！！！ 你输入的是[0000] 毕业生或已完成结算了的也不行哦！';</script>
```



  差别就在于最后一行,只要判断这两个字符串是否相等就可以得出登陆是否成功了.



  顺便提一下,这一个数据包其实是经历了两次recv() .第一次recv() 接收了HTTP 数据包头,无论缓冲区的大小有多大,它就只是接收数据包头,而HTTP 附加数据则需要再一次调用recv() 来获得.



3. Cookie-key



  发送登陆成功后,下一步就是要获取课表的HTML 代码了.



```c++
#define HTTP_GET_CLASS "GET /sise/module/student_schedular/student_schedular.jsp HTTP/1.1\r\n" \
		"Accept: text/html, application/xhtml+xml, */*\r\n" \
		"Referer: <http://class.sise.com.cn:7001/sise/module/student_states/student_select_class/main.jsp>\r\n" \
		"Accept-Language: zh-CN\r\n" \
		"User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0)\r\n" \
		"UA-CPU: AMD64\r\n" \
		"Accept-Encoding: gzip, deflate\r\n" \
		"Host: class.sise.com.cn:7001\r\n" \
		"Connection: Keep-Alive\r\n" \
		"Cookie: JSESSIONID=Sb3B1Znt3wOHuB2SyHUbfjrHJJuZf93l53WZlFtwXVW7bLT63xY2!1996935212\r\n\r\n"

//...
send(sock,
HTTP_GET_CLASS,LEN_HTTP_GET_CLASS,0
);

//...
```



  代码看起来是没问题的,要不来运行一下?..



```http
HTTP/1.1 200 OK
Date: Sat, 19 Oct 2013 11:41:08 GMT
Server: WebLogic Server 8.1 SP2 Fri Dec 5 15:01:51 PST 2003 316284 
Content-Length: 158
Content-Type: text/html; charset=GBK
Set-Cookie: JSESSIONID=SivUuEUszNo2a3UnMb41ticD2nievqoDLLBI7ZbD7vdk9Rv24vkV!-547174719; path=/
```



  看到没有,这时服务器并没有返回我们想要的HTML ,但在这个HTTP 数据头中,我们看到一个不同的地方,那就是Set-Cookie: 选项,经过多次测试,发现这个Cookie 的值会在某个时间会改变,如果要继续获取HTML 代码的话,那就必须把这个Set-Cookie 的Key 值提取出来,然后把它插入GET 数据包.于是代码就应该改写成下面这样:



```c++
#define HTTP_SETCOOKIE "Set-Cookie: JSESSIONID="

#define LEN_HTTP_SETCOOKIE strlen(HTTP_SETCOOKIE)
#define LEN_HTTP_KEY_COOKIE 63

#define HTTP_GET_CLASS "GET /sise/module/student_schedular/student_schedular.jsp HTTP/1.1\r\n" \
		"Accept: text/html, application/xhtml+xml, */*\r\n" \
		"Referer: <http://class.sise.com.cn:7001/sise/module/student_states/student_select_class/main.jsp>\r\n" \
		"Accept-Language: zh-CN\r\n" \
		"User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0)\r\n" \
		"UA-CPU: AMD64\r\n" \
		"Accept-Encoding: gzip, deflate\r\n" \
		"Host: class.sise.com.cn:7001\r\n" \
		"Connection: Keep-Alive\r\n" \
		"Cookie: JSESSIONID="

#define LEN_HTTP_GET_CLASS strlen(HTTP_GET_CLASS)+LEN_HTTP_KEY_COOKIE

char *GetCookitKey(const char *Input)
{
	char *rtn=(char *)malloc(LEN_HTTP_KEY_COOKIE+1);
	memset(rtn,0,LEN_HTTP_KEY_COOKIE+1);

	char *point=strstr(Input,HTTP_SETCOOKIE);
	memcpy(rtn,(const char *)point+LEN_HTTP_SETCOOKIE,LEN_HTTP_KEY_COOKIE);

	return rtn;
}

char *AllocSchdulePacket(const char *CookieKey)
{
	char *Packet=(char *)malloc(LEN_HTTP_GET_CLASS+LEN_HTTP_CRLF*2);
	memset(Packet,0,LEN_HTTP_GET_CLASS+LEN_HTTP_CRLF*2);

	sprintf(Packet,"%s%s\r\n\r\n",HTTP_GET_CLASS,CookieKey);
	return Packet;
}

#define HTTP_PACKET_SCHDULE AllocSchdulePacket(key)

//...

Packet=(char *)HTTP_PACKET_SCHDULE;

send(sock,Packet,strlen(Packet),0);

//...

  strlen(Packet) 估计都能看懂,但还是建议写为:
#define LEN_HTTP_PACKET_SCHDULE strlen(Packet)
//...
send(sock,Packet,LEN_HTTP_PACKET_SCHDULE,0);
```



4. 用户退出



  退出也是重要的一步,它的重要性不可忽略,如果缺少退出的话,那将不能重用端口(已论证),将导致下一次获取将可能会失败.



```c++
#define HTTP_GET_EXIT "GET /sise/common/sessionSetNull.jsp; HTTP/1.1\r\n" \
		"Accept: text/html, application/xhtml+xml, */*\r\n" \
		"Referer: <http://class.sise.com.cn:7001/sise/index.jsp>\r\n" \
		"Accept-Language: zh-CN\r\n" \
		"User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0)\r\n" \
		"UA-CPU: AMD64\r\n" \
		"Accept-Encoding: gzip, deflate\r\n" \
		"Host: class.sise.com.cn:7001\r\n" \
		"Connection: Keep-Alive\r\n" \
		"Cookie: JSESSIONID="

#define LEN_HTTP_GET_EXIT strlen(HTTP_GET_EXIT)+LEN_HTTP_KEY_COOKIE

char *AllocExitPacket(const char *CookieKey)
{
	char *Packet=(char *)malloc(LEN_HTTP_GET_EXIT+LEN_HTTP_CRLF*2);
	memset(Packet,0,LEN_HTTP_GET_EXIT+LEN_HTTP_CRLF*2);

	sprintf(Packet,"%s%s\r\n\r\n",HTTP_GET_EXIT,CookieKey);
	return Packet;
}
```



5. 完整的流程



```c++
char *httppacket=AllocLoginPacket((const char *)num,(const char *)password);
send(sock,httppacket,strlen(httppacket),0);

char *httpbuffer=(char *)malloc(LEN_HTTP_BUFFER);
memset(httpbuffer,0,LEN_HTTP_BUFFER);
recv(sock,httpbuffer,LEN_HTTP_BUFFER,0);

char *key=GetCookitKey((const char *)httpbuffer);

memset(httpbuffer,0,LEN_HTTP_BUFFER);
if (0<recv(sock,httpbuffer,LEN_HTTP_BUFFER,0)){
    char *Packet=NULL;
    if (0==strcmp(HTTP_POST_OK,(const char *)httpbuffer)){
        printf("\nlogin OK \n");

        Packet=(char *)HTTP_PACKET_SCHDULE;
        send(sock,Packet,strlen(Packet),0);

        timeval block;
        memset(&block,0,sizeof(timeval));
        block.tv_sec=1;
        block.tv_usec=0;
        setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(const char *)&block,sizeof(timeval));
							
        memset(httpbuffer,0,LEN_HTTP_BUFFER);
        while(0<recv(sock,httpbuffer,LEN_HTTP_BUFFER,0))
        {
            printf("%s",httpbuffer);						
            memset(httpbuffer,0,LEN_HTTP_BUFFER);
        }
    }
    else
        printf("\nlogin ERR \n");

    Packet=(char *)HTTP_PACKET_EXIT;
    send(sock,Packet,strlen(Packet),0);
    free(httpbuffer);
    httpbuffer=NULL;
}
```



