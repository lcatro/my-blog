更新原理:



1.



![](pic_temp6\psb1.jpg)



2.



![](pic_temp6\psb2.jpg)



1. 读取远程文件:程序会从远程文件中读取出现在官方网站更新的最新程序版本,读取完成后本地程序再把读取结果和本地程序做比较

2. 程序下载更新:在第二步用URLDownloadToFile() 和InternetReadFile() 系列函数直接下载VB程序到本地的结果是:应用程序结构出现问题.URLDownloadToFile() 下载,VB程序以控制台程序的形式打开,没有任何反应
   InternetReadFile() 系列函数下载,VB程序根本不能打开,Windows提示原因"这不是有效的Win32应用程序"



除了下载应用程序不能打开,下载其它的文件暂时未发现问题



于是更新设计便变得略麻烦点,需要用到RAR解压下载后的文件



应用程序主体代码很简洁,没用利用Form_Load()事件运行,直接在Sub Main()下进行更新检测,没有利用窗体,节省时间和内存:



```visual basic
Sub Main()
If ChackUpdate Then
   If (MsgBox("程序可以更新,需要吗?..", vbYesNo) = vbYes) Then
   DownLoadFile UpdateRAR, App.Path & "\" & SaveToLocalPath
   RARExecute OP_EXTRACT, App.Path & "\" & SaveToLocalPath, App.Path
   Shell App.Path & "\Update.exe"
   Shell App.Path & "\DeleteEXEFile.exe " & App.Path & "\" & SaveToLocalPath
   Shell App.Path & "\DeleteEXEFile.exe " & App.Path & "\" & App.EXEName & ".exe" & " " & App.EXEName & ".exe"
   MsgBox "程序更新完成.."
   End If
End If
End Sub
```



代码注释:



1. 函数ChackUpdate() 检测应用程序是否需要更新

2. 函数DownLoadFile () 利用URLDownloadToFile() 下载文件到本地

3. 函数RARExecute() 解压RAR文件



```visual basic
Shell App.Path & "\DeleteEXEFile.exe " & App.Path & "\" & SaveToLocalPath
Shell App.Path & "\DeleteEXEFile.exe " & App.Path & "\" & App.EXEName & ".exe" & " " & App.EXEName & ".exe"
```



DeleteEXEFile.exe 是一个外部程序,主要作用:删除.EXE/.RAR 文件



由于应用程序不能在运行的时候自我删除,所以就利用这个DeleteEXEFile.exe 配合删除自己



为什么不用.ini 文件保存程序下载地址和更新文件读取URL地址:



之前讯雷有一个漏洞,在修改某一个文件里面的一项数据为一个木马URL之后,将要下载的文件被重定向到下载这个木马,上面的.ini 文件也是同样的道理,修改更新下载地址让更新程序下载了带有病毒代码的程序再运行,所以在程序里面用一个常量保存下载地址和更新文件读取URL地址会比较安全,只要保证目标文件和这两个URL没有差错的话,程序还是可以正常运行的



于是比较应用程序版本就用InternetReadFile() 系列函数实现 [这功能写在InternetIO 模块,工程代码里面有]



检测更新函数ChackUpdate()



```visual basic
Function ChackUpdate() As Boolean
InternetIO.InitWinInet
InternetIO.OpenURL UpdateFileURL

Dim Ver As String
Ver = Trim(InternetIO.ReadData(10))

Dim Main As Long
Dim Minor As Long
Dim Revision As Long

ResoltVersionString Ver, Main, Minor, Revision

If Main > App.Major Or Minor > App.Minor Or Revision > App.Revision Then
ChackUpdate = True
Exit Function
End If
InternetIO.CloseURL
InternetIO.CloseWinInet
ChackUpdate = False
End Function
```



代码注释:



```txt
InternetIO 模块:
初始化模块:
Sub InternetIO.InitWinInet()
打开URL:
Function InternetIO.OpenURL(ByVal URL As String) As Boolean 
读取URL数据
Function InternetIO.ReadData(ByVal Size As Long) As String
关闭URL:
Sub InternetIO.CloseURL()
关闭模块:
Sub InternetIO.CloseWinInet()
函数ResoltVersionString() 用来解析最新版本文件中读取的数据到主版本,次版本,修改版本
Sub ResoltVersionString(ByVal VersionString As String, Optional MainVer As Long, Optional MinorVer As Long, Optional RevisionVer As Long)
ResoltVersionString Ver, Main, Minor, Revision
VersionString 一般为Ver=x.x.x
```



关于DeleteEXEFile.exe:



这是一个用VC++6编写的控制台程序,目的是为了删除.EXE/.RAR文件,也许你会有一个疑问,为什么不用VB里面的Kill() 函数呢,原因有两个:



1. 假设让更新程序删除文件,那么在实现自我删除的时候会变得很麻烦,例如:创建批处理实现自我删除等..

2. 也可以按照这样把程序所需要做的分开到另一个程序去,这样的好处就是方便,不过VB在提取程序外部传递参数不够灵活,使得DeleteEXEFile.exe 参数提取代码会变得亢长,于是VC++6 便一个很好的选择



那它靠什么来等待应用程序是否被关闭了呢?



Windows有一个等待对象函数WaitForSingleObject() 



```c++
DWORD WaitForSingleObject(
　　HANDLE hHandle,
　　DWORD dwMilliseconds
　　);
```



更加详细请到百度百科查看该函数的使用:http://http://baike.baidu.com/view/908866.htm

代码如下:



```c++
// DeleteEXEFile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <memory.h>
#include <string.h>
#include <windows.h>
#include <Tlhelp32.h>

HANDLE GetRunningProcHandle(char *ProcName);

//DeleteEXEFile %EXEPathAndEXEName% [%WaitStopEXE%]

int main(int argc, char* argv[])
{
if (argc==3)
{
   HANDLE hProc=GetRunningProcHandle(argv[2]);
   WaitForSingleObject (hProc,INFINITE);
   CloseHandle(hProc); 
}
DeleteFile((LPCTSTR)argv[1]);
return 0;
}

HANDLE GetRunningProcHandle(char *ProcName)  //获取目标进程的句柄
{
HANDLE hFind=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
PROCESSENTRY32  ProcData;
memset(&ProcData,0,sizeof(PROCESSENTRY32));
ProcData.dwSize=sizeof(PROCESSENTRY32);
Process32First (hFind,&ProcData);
while(Process32Next(hFind,&ProcData))
{
if (strcmp((const char *)ProcName,(const char *)ProcData.szExeFile)==0)
{ 
CloseHandle(hFind);
return OpenProcess(PROCESS_ALL_ACCESS,FALSE,ProcData.th32ProcessID);
}
}
CloseHandle(hFind);
return INVALID_HANDLE_VALUE;
}
```

