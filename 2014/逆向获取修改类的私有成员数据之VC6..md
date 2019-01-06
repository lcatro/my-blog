##  类的私有成员



  在研究实际问题时,有很多数据是不应该对外开放的,有一个更好的方法就是提供一些接口去处理这些数据的传入与传出,把所有的实现细节都隐藏在接口的实现处.所以,类的接口难免会不向外提供某些关键的数据,对于这些数据,类要采取保护的原则,于是,我们就把他们作为类的私有成员.



1. 私有成员特性



  作为私有成员,是受到类的保护的,而这个保护也仅仅限于编译时保护(因为语法原因,编译器是不允许我们直接根据类去访问私有成员的).下面是声名含有一个私有变量成员的类.



```c++
 class test_class {
  public:
   test_class(void);
   ~test_class();
  private:
   long data_number;
 }
```



  如果要想修改和获取它是有许多办法的,许多C++ 文献建议我们利用一对get/set 函数去实现.



```c++
 class test_class {
  public:
   test_class(void);
   ~test_class();
   long get_data_number(void) const;
   void set_data_number(long in_data_number);
  private:
   long data_number;
 }
```



  那类的公有成员又是怎么样读写私有成员的呢?



2. 逆向代码



  先看一下test_class 类的析构函数的汇编代码:



```assembly
67:     test_class(void) {
00401140   push        ebp
00401141   mov         ebp,esp
00401143   sub         esp,44h
00401146   push        ebx
00401147   push        esi
00401148   push        edi
00401149   push        ecx
0040114A   lea         edi,[ebp-44h]
0040114D   mov         ecx,11h
00401152   mov         eax,0CCCCCCCCh
00401157   rep stos    dword ptr [edi]
00401159   pop         ecx
0040115A   mov         dword ptr [ebp-4],ecx
68:         data_number=100;
0040115D   mov         eax,dword ptr [ebp-4]
00401160   mov         dword ptr [eax],64h
;  由此可以知道私有变量是存在于类的堆栈中的指针来进行访问的..
;  其实上面这句代码是 this->data_number=100; 的简写..
;  [ebp-4]<=>data_number
69:     };
00401166   mov         eax,dword ptr [ebp-4]
00401169   pop         edi
0040116A   pop         esi
0040116B   pop         ebx
0040116C   mov         esp,ebp
0040116E   pop         ebp
0040116F   ret
```



  然后就是获取私有变量的汇编代码:



```assembly
72:     long get_data_number(void) const {
004011B0   push        ebp
004011B1   mov         ebp,esp
004011B3   sub         esp,44h
004011B6   push        ebx
004011B7   push        esi
004011B8   push        edi
004011B9   push        ecx
004011BA   lea         edi,[ebp-44h]
004011BD   mov         ecx,11h
004011C2   mov         eax,0CCCCCCCCh
004011C7   rep stos    dword ptr [edi]
004011C9   pop         ecx
004011CA   mov         dword ptr [ebp-4],ecx
73:         return data_number;
004011CD   mov         eax,dword ptr [ebp-4]
004011D0   mov         eax,dword ptr [eax]
74:     };
004011D2   pop         edi
004011D3   pop         esi
004011D4   pop         ebx
004011D5   mov         esp,ebp
004011D7   pop         ebp
004011D8   ret
```



  设置私有变量的汇编代码:



```assembly
75:     void set_data_number(long in_data_number) {
004011F0   push        ebp
004011F1   mov         ebp,esp
004011F3   sub         esp,44h
004011F6   push        ebx
004011F7   push        esi
004011F8   push        edi
004011F9   push        ecx
004011FA   lea         edi,[ebp-44h]
004011FD   mov         ecx,11h
00401202   mov         eax,0CCCCCCCCh
00401207   rep stos    dword ptr [edi]
00401209   pop         ecx
0040120A   mov         dword ptr [ebp-4],ecx
76:         data_number=in_data_number;
0040120D   mov         eax,dword ptr [ebp-4]
00401210   mov         ecx,dword ptr [ebp+8]  ;  参数in_data_number
00401213   mov         dword ptr [eax],ecx
77:     };
00401215   pop         edi
00401216   pop         esi
00401217   pop         ebx
00401218   mov         esp,ebp
0040121A   pop         ebp
0040121B   ret          4
```



  由上面可以知道,类的get/set 方法是根据this 指针来访问类的私有数据成员的.现在就根据下面的代码来进行研究:



```c++
#include <stdio.h>

#include "class.h"  //  class_test 的声名都在里面

void main(void) {
    test_class test;
    test.get_data_number();
    test.set_data_number(1234);
}
```



  当然,这个类不是基址,它只是main() 函数里面的一个局部变量,如果当test 类为全局时,情况又会是怎么样呢?下面是要研究的代码:



```c++
#include <stdio.h>
#include "class.h"
test_class test;
 void main(void) {    test.get_data_number();}
```



  此时CE 的分析显示,私有变量是一个基址:0x00428D58 ,这就意味着可以通过这个不变的地址读/写私有数据成员.



##  第三方程序的进程内存访问



  一般情况下普通的进程是不可以访问物理内存的,那么要访问其它进程的内存就需要用到ReadProcessMemory/WriteProcessMemory 函数.



1. 变量与指针



  回归定义,变量和指针都是和数据的操作有关,但不同之处是,变量根据标识符来进行数据操作,指针根据保存在标识符里面的地址来进行数据操作.用个简单的例子来说明这些微妙的差距:



```assembly
9:      int  number=100;
00401168   mov         dword ptr [ebp-4],64h  ;  直接把100 写入到栈
10:     int* point_number=&number;
0040116F   lea         eax,[ebp-4]  ;  把number 的地址保存到point_number 中
00401172   mov         dword ptr [ebp-8],eax
11:     *point_number=10;
00401175   mov         ecx,dword ptr [ebp-8]  ;  读取point_number 保存的地址
00401178   mov         dword ptr [ecx],0Ah  ;  把10 写到这个地址中去
```



2. 简单的游戏框架



```c++
class user_data {

 public:
  user_data(void) {
      mon=0;
      exp=0;
  };
  ~user_data() {};

  long get_mon(void) const {
      return mon;
  };
  long get_exp(void) const {
      return exp;
  };
  void set_mon(long in_number) {
      mon=in_number;
  };
  void set_exp(long in_number) {
      exp=in_number;
  };

 private:
  long mon;
  long exp;
};

class game {

 public:
  game(void) {
      player=1;
  };
  ~game() {
  };

  long get_player(void) const {
      return player;
  };
  
  long get_mon(void) const {
      return point_user_data.get_mon();
  };
  long get_exp(void) const {
      return point_user_data.get_exp();
  };
  void set_mon(long in_number) {
      point_user_data.set_mon(in_number);
  };
  void set_exp(long in_number) {
      point_user_data.set_exp(in_number);
  };

 private:
  user_data point_user_data;
  long player;
};
```



  对于游戏来说,每个用户都会有属于他的数据,诚然,上面的类架构对于真正的游戏是远远不够的,但是作为简单的实例,这也就足够了.



  注意,在这里要提示一下,在game 类的数据堆栈中会出现这种状况:



```assembly
  mon     [ebp - 4]
  exp      [ebp - 8]
  player  [ebp - 16]
```



  它产生的原因是类把它组合的所有的对象的数据成员和它声名的数据成员都保存在它的堆栈中.也就是说,game 类组合了user_data 类,user_data 类里面有两个数据成员:mon ,exp ,由于在game 的数据成员声名中user_data 类比player 更要靠前,所以堆栈中user_data::mon 和user_data::exp 都在game::player 的前面.



##  实例



  下面以非常出名的游戏<<植物大战僵尸>> 为实例.



1. 有用的几个内存地址



  阳光地址:[[0x006A9EC0]+0x768]+5560

  怪物数上限地址:[[0x006A9EC0]+0x768]+94

  金币地址:[[0x006A9EC0]+0x82C]+0x28




  注意:[] 的意思是指从这个地址读取数据.更详细的内存数据请点链接:<http://tieba.baidu.com/f?z=870532241&ct=335544320&lm=0&sc=0&rn=30&tn=baiduPostBrowser&word=%D6%B2%CE%EF%B4%F3%D5%BD%BD%A9%CA%AC&pn=0>



  由这么多数据可以知道,[[0x006A9EC0]+0x768] 保存着游戏场景和用户的相关数据,[[0x006A9EC0]+0x82C] 保存着游戏设置.



  这些内存地址告诉了我们什么东西呢?首先,地址0x006A9EC0 是关系到整个游戏数据的,但是它可能不是游戏程序的基址(因为电脑没有PVF 所以没有办法调试程序的基址).根据类的私有数据成员特性可知,[[0x006A9EC0]+0x768] 和[[0x006A9EC0]+0x82C] 是this 指针,而且0x006A9EC0  可能就是游戏类的全局变量.据此可得PVF 的简单系统架构图:
 

![](pic_temp1\psb1.png)

  

  上面已经得到结论:类中组合的类的私有数据成员地址可能会与原来的类共存在同一堆栈下.也就是说,game 类中堆栈可以直接访问game_setting 的私有数据成员,不过,由上面的地址数据可以知道,要访问game_setting 和game_data 是要偏移一定位置的指针的,对此合理的解释就是"game 类的组合类game_setting 和game_data 是类的指针",这样就可以解释为什么访问类的私有数据成员需要进行第一次数据偏移了(比如:game_data 类的this 指针:[0x006A9EC0]+0x768 ,game_setting 类的this 指针:[0x006A9EC0]+0x82C ),进而访问类内的数据成员时,就需要对this 指针进行操作了(对阳光数据读写:[[0x006A9EC0]+0x768]+5560 <=> this->suns [此时this 的game_data 的类指针]).