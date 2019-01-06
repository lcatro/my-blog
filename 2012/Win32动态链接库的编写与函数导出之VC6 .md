我们通过下面的过程来创建一个Win32的动态链接库



![图片](pic_temp1\psb1.jpg)

 

![图片](pic_temp1\psb2.jpg)



创建完成后,VC++会自动生成一个DLLMain函数



```C++
#include "stdafx.h"


BOOL APIENTRY DllMain( HANDLE hModule, 

                       DWORD  ul_reason_for_call, 

                       LPVOID lpReserved

) {
    return TRUE;
}
```



DLLMain函数中的参数的意思:



```C++
DllMain(
    HANDLE hModule,      // DLL句柄,也就是DLL加载到内存的基址
    DWORD  ul_reason_for_call, // 程序DLL的调用方式,接下来就介绍
    LPVOID lpReserved          // 一般保留不用
)
```



一般,在DLLMain中我们只需要关注 ul_reason_for_call 这个参数



有下面这些值



+ DLL_PROCESS_ATTACH -- 当进程第一次调用DLL的时候,必定要先将DLL映射到内存中,这个意思也就是告诉DLLMain,你被进程加载了

+ DLL_PROCESS_DETACH -- 当进程关闭的时候,需要销毁(说好听一点就是清理)自己曾经使用过的句柄和内存空间,这个常量也就是告诉DLLMain,调用你的进程正在关闭中,你也该做些什么了吧

+ DLL_THREAD_ATTACH -- 与DLL_PROCESS_ATTACH 差不多,不过这个常量告诉DLLMain ,是一个新的(还没有加载过这个DLL的)线程把你给加载的

+ DLL_THREAD_DETACH -- 与DLL_PROCESS_DETACH差不多



那么,对于不同的情况,DLLMain 需要不同的处理方法,一般用switch语句来写处理方法



下面的代码就没有考虑这个处理问题,只是给出了简单的例子:



```c++
BOOL APIENTRY DllMain( HANDLE hModule, 

 DWORD  ul_reason_for_call, 

 LPVOID lpReserved

 )
{
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		printf("\nprocess attach of dll");      //  这个对于非控制台程序很危险.以Win32 程序为例子(Win32 Application)
		//  没有控制台,printf也就不能输出到控制台,由于控制台也有一个标准句柄的[*GetStdHandle*]获得
		//  当printf引用了这个空句柄后,就会有错误
		break;
	case DLL_THREAD_ATTACH:
		printf("\nthread attach of dll");
		break;
	case DLL_THREAD_DETACH:
		printf("\nthread detach of dll");
		break;
	case DLL_PROCESS_DETACH:
		printf("\nprocess detach of dll");
		break;
	}
	return TRUE;
}
```



其实有时候的DLLMain函数可以省略的,比如说获得一些系统的信息,做一些Hook API的函数,或者是资源DLL....



下面就编写一些属于自己写的函数了



```c++
int add(int x,int y)
{
	return x + y;
}
```



一般导出DLL中的函数有两种方法,一种就是利用 __declspec(dllexport) 修饰符,还有一种是写.DEF文件(DLL模块定义文件)



利用**__declspec(dllexport)** 修饰符的话呢,上面的函数就应该改写为



```c++
__declspec(dllexport) int add(int x,int y)
{
	return x + y;
}
```



这样就可以导出一个DLL函数啦,怎么样,简单吧,,.[在函数声明的前面加上一个**__declspec(dllexport)]**



那么函数从DLL中导出了,怎么让第三方程序调用呢,这里就用到库文件



在工作区中的"Header Files"中创建一个新的库文件,命名TestWin32DLL.h (其实什么名字都可以,用这个名字只是为了好记,这个库文件是从哪一个DLL中导入函数的)  [注意导入和导出是不同的..]



TestWin32DLL库代码:



```c++
#ifndef TestWin32DLL_H
#define TestWin32DLL_H
#pragma comment(lib,"TestWin32DLL.lib")  // 先与和DLL一起编译出来的.Lib文件来连接,里面有DLL编译过后的一些索引数据

// 下面就把需要从TestWin32DLL.lib导出的函数写在这儿

extern "C" int __cdecl add(int x,int y);  // 用标准C约定来导出,注意 __cdecl 是参数进栈方式,VC++默认是 __cdecl ,这个不能省略

#endif
```



VC++调用TestWin32DLL中的add函数的代码:



```c++
#include "stdafx.h"
#include <stdio.h>
#include "TestWin32DLL.h"

int main(int argc, char* argv[]) {
	printf("Calculate Result :%d",add(1,2)); 

	return 0;
}
```



运行效果如下:



![图片](pic_temp1\psb3.jpg)





 

使用这种方法有一种缺陷,就是不能通过GetProcAddress来导出函数,也就是说,需要从函数入口点来调用函数是不可以的,VB等的编程语言也就不能实现跨语言调用了



但是接下来的写.DEF文件来导出函数是可以改变这样一个问题的[请注意,这个文件需要自己来创建] 



还是原来的DLL代码**TestWin32DLL.dll**



```c++
#include "stdafx.h"
#include "TestWin32DLL.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
) {
    return TRUE;
}

int add(int x,int y)   //  <- 注意这里,没有用 __declspec(dllexport)) 
{
	return x + y;
}
```



那么对应的.DEF文件导出代码就是



```txt
LIBRARY LIB
EXPORTS
add @ 1  ; 这个 1 代表的是函数的顺序,不影响程序的调用
```



好了,验证一下.DEF文件的导出



VB6 代码:



```visual basic
Private Declare Function add Lib "TestWin32DLL.dll" (ByVal v2 As Long, ByVal v2 As Long) As Long

Private Sub Form_Load()
MsgBox add(1, 2)
End Sub
```



VC++代码:



```c++
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>

typedef int (* fAdd)(int,int);

int main(int argc, char* argv[])
{
HINSTANCE hDll;  
fAdd addFun; 
    
hDll = LoadLibrary("TestWin32DLL.dll");
if (hDll != NULL)
{
 addFun = (fAdd)GetProcAddress(hDll,"add");  // 从DLL中找 add 函数,再转型
 if(addFun!=NULL) //  当GetProcAddress在DLL中找不到函数时就返回一个空的函数指针
 {
	int result = addFun(2,3); // 调用函数      
	printf("Calculate Result :%d",result);
 }
 else printf("Err");
	FreeLibrary(hDll);
}
}
```



到此,请注意一下VB6的代码,怎么看这些VB代码都是正确的,但是如果将它们(VC++的DLL,VB的程序)一起编译,VB6的程序运行后会有一个错误的窗口



![图片](pic_temp1\psb4.jpg)



Q:VB为什么会调用错误,而VC++不会呢?



**A:由于VB的DLL调用约定是以 __stdcall 的方式让参数从右向左进栈后再调用函数的,但是VC++的默认是以 __cdecl 方式来声明函数**



于是乎,上面的函数就应该改为:



```c++
int __stdcall add(int x,int y) //对应到库文件也需要修改为 extern "C" int __stdcall add(int x,int y);而不是 extern "C" int __cdecl add(int x,int y);
{
	return x + y;
}
```



