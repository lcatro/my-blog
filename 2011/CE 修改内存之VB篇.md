

先由一个简单的示例程序开始手把手演示如何用CE修改内存

首先自己先编写好一个示例程序



![](pic_temp1\http_imgload1.png)

 

程序代码为:

 

```visual basic
Dim a As Long
Private Sub Form_Load()  '''初始化变量
a = 0
End Sub

Private Sub Command1_Click() '''变量自加
a = a + 11
End Sub

Private Sub Timer1_Timer() '''为了能让变量立即显示，应设置为10ms
Label1.Caption = a
End Sub
```





至此，一个简单的示例程序就制作完成了

那么，怎么去查找变量的地址呢，这里用CE(Cheat Engine)



  运行示例程序和CE



![](pic_temp1\http_imgload2.png)




然后用CE来找变量的内存

打开进程



![](pic_temp1\http_imgload3.png)



查找数据



![](pic_temp1\http_imgload4.png)



点图中的 First Scan 即可

结果



![](pic_temp1\http_imgload5.png)



在这么多的内存地址中，到底哪一个才是我们要找的呢?

我们可以通过查看哪个内存地址中的数据被改变来确定我们需要的地址

(搜索需要时间和细心...)

最后找到的地址是:1C804C

![](pic_temp1\http_imgload6.png)

然后重新启动程序，发现程序的内存地址并没有改变，那么这就是一个静态的内存地址

![](pic_temp1\http_imgload7.png)

下一步，开始自己编写代码来读取和修改这个程序的变量数据

为了方便，我已经事先编写好了一个读内存的VB类,名曰:ReadMemory

 

```visual basic
Private Declare Function FindWindow Lib "user32" Alias "FindWindowA" (ByVal lpClassName As String, ByVal lpWindowName As String) As Long
Private Declare Function GetWindowThreadProcessId Lib "user32" (ByVal Hwnd As Long, lpdwProcessId As Long) As Long
Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long
Private Declare Function OpenProcess Lib "kernel32" (ByVal dwDesiredAccess As Long, ByVal bInheritHandle As Long, ByVal dwProcessId As Long) As Long
Private Const PROCESS_ALL_ACCESS = &H1F0FFF '全权打开进程
Private Declare Function ReadProcessMemory Lib "kernel32" ( _
 ByVal hProcess As Long, _
 ByVal pvAddressRemote As Any, _
 ByVal pvBufferLocal As Any, _
 ByVal dwSize As Long, _
 ByVal pdwNumBytesRead As Long) As Long
Private Declare Function WriteProcessMemory Lib "kernel32" (ByVal hProcess As Long, ByVal lpBaseAddress As Any, ByVal lpBuffer As Long, ByVal nSize As Long, lpNumberOfBytesWritten As Long) As Long

 

Dim ProcToken As Long ''''指定的窗口进程最大权限

Function InitModuleA(ByVal WindowTitle As String) As Boolean
Dim Hwnd As Long
Hwnd = FindWindow(vbNullString, WindowTitle)
If Hwnd > 0 Then
Dim PID As Long
GetWindowThreadProcessId Hwnd, PID
 If InitModuleB(PID) Then
 InitModuleA = True
 Else
 InitModuleA = False
 End If
Else
InitModuleA = False
End If
End Function

Function InitModuleB(ByVal WindowPID As Long) As Boolean
Dim Token As Long
Token = GetMaxPrilivageToProcess(WindowPID)
If Token > 0 Then
ProcToken = Token
InitModuleB = True
Else
InitModuleB = False
End If
End Function

Sub ClearModule()
ProcToken = 0
End Sub

Private Function GetMaxPrilivageToProcess(ByVal ProcessID As Long) As Long
GetMaxPrilivageToProcess = OpenProcess(PROCESS_ALL_ACCESS, False, ProcessID)
End Function

Public Function GetMemory(ByVal MemoryBaseAddr As Long) As Long
Dim GetValue As Long
 ReadProcessMemory ProcToken, MemoryBaseAddr, VarPtr(GetValue), 4, 0
GetMemory = GetValue
End Function

Public Function SetMemory(ByVal MemoryBaseAddr As Long, ByVal Value As Long) As Boolean
If ProcToken > 0 And WriteProcessMemory(ProcToken, MemoryBaseAddr, VarPtr(Value), 4, 0) <> 0 Then
SetMemory = True
Else
SetMemory = False
End If
End Function
```





简单的修改内存工具代码如下:



![](pic_temp1\http_imgload8.png)

 

```visual basic
Private Sub Form_Load()
ReadMemory.InitModuleA "1"
End Sub

Private Sub Command1_Click()
Dim m As Long
m = InputBox("输入一个数字")
ReadMemory.SetMemory &H1C804C, m
End Sub

Private Sub Timer1_Timer()
Label1.Caption = "当前的数据: " & ReadMemory.GetMemory(&H1C804C)
End Sub
```



效果如下:


![](pic_temp1\http_imgload9.png)



![](pic_temp1\http_imgload10.png)



至此，已经制作出可以修改内存的小工具了，那么如何去制作外挂呢?其实单机的外挂跟修改内存差不多

只不过在修改内存时多了内存偏移而已，以后有机会再跟大家分享.....

 

