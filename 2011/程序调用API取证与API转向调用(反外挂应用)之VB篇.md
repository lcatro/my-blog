先从PE文件开始分析



![图片](pic_temp3\http_imgload1.jpg)



上面就是一个程序的PE文件结构,我们要调用的API入口地址是在FirstThunk中保存着

![图片](pic_temp3\http_imgload2.jpg)



我们要获得API入口地址的话,可以用 LoadLibrary  +  GetProcAddress  来获得
下面给大家一个用法:



```visual basic
Private Declare Function GetProcAddress Lib "kernel32" (ByVal hModule As Long, ByVal lpProcName As String) As Long
Private Declare Function LoadLibrary Lib "kernel32" Alias "LoadLibraryA" (ByVal lpLibFileName As String) As Long

Private Function GetModuleFuncAddr(ByVal ModulePath As String, ByVal FuncStr As String) As Long
Dim hHwnd As Long
hHwnd = LoadLibrary(ModulePath)
Dim APIAddr As Long
APIAddr = GetProcAddress(hHwnd, FuncStr)
GetModuleFuncAddr = APIAddr
End Function
```



接下来给大家两个实例程序(游戏程序和游戏外挂程序)

游戏程序



![图片](pic_temp3\http_imgload3.jpg)



程序代码:



```visual basic
Dim Data As Long


Private Sub Command1_Click()
Data = Data + 10
End Sub

Private Sub Command2_Click()
Data = Data - 10
End Sub

Private Sub Timer1_Timer()   '''  实时显示数据
Label1.Caption = Data
End Sub
```



游戏外挂



![图片](pic_temp3\http_imgload4.jpg)



程序代码:   '''  程序中含有一个读写内存的类,在<<CE 修改内存--VB篇>>中有详细的代码



```visual basic
Private Sub Command1_Click()
SetMemory &H150484, GetMemory(&H150484) + 100
End Sub

Private Sub Command2_Click()
SetMemory &H150484, GetMemory(&H150484) - 100
End Sub

Private Sub Form_Load()
InitModuleA "Form1"
End Sub

Private Sub Timer1_Timer()
Label1.Caption = GetMemory(&H150484)
End Sub
```

 

怎么看这两段代码都好像没有什么特别之处, 但是我们细细地来分析,**当一个程序它在调用到API的时候,IAT(Import Address Table, 导入地址表)中就会加载这个函数的地址**,下面是从CE中得到的数据(以一个Beep函数程序为例子)当程序还没有调用Beep函数时:



![图片](pic_temp3\http_imgload5.jpg)



当程序成功调用Beep函数时:



 ![图片](pic_temp3\http_imgload6.jpg)

 

 其中的 4032DC 就是 Beep 函数在IAT中的内存地址,经过一些测试,这些地址属于**静态地址,但是这些地址中的值可能会发生变化**

  附送该程序调用Beep()的关键代码(VB反汇编):



```assembly
401CEF  push 00005DCh
401CF4  push 000001F4h
401CF9  Beep(%x1,%x2)
```



  到此,再看看上面的游戏外挂程序,我用详细的流程图来表示一个外挂程序是如何运行的



   ![图片](pic_temp3\http_imgload7.jpg)



 其中GetMaxPrilivageToProcess()的代码为



```visual basic
Private Function GetMaxPrilivageToProcess(ByVal ProcessID As Long) As Long  '''  返回一个进程的最大打开权限
GetMaxPrilivageToProcess = OpenProcess(PROCESS_ALL_ACCESS, False, ProcessID)
End Function    '''  PROCESS_ALL_ACCESS=&H1F0FFF
```



读者们看到这儿,就会发现外挂都是调用这几个函数:

**FindWindow**

**GetWindowThreadProcessId**

**OpenProcess**

**WriteProcessMemory**

至于ReadProcessMemory和CloseHandle这两个函数可有可无,因为有一些程序是没有这些函数的(ReadProcessMemory有一些G是没有的,而CloseHandle没有的话就不怎么正常了,所以这应该是一个编G新手所遗忘的)

再去分析这些函数的API,有



| 函数名                   | IAT地址  | 函数地址   | DLL      |
| ------------------------ | -------- | ---------- | -------- |
| FindWindow               | &H403340 | 2010284769 | kernel32 |
| GetWindowThreadProcessId | &H40334C | 2010221184 | kernel32 |
| OpenProcess              | &H403364 | 2088962537 | kernel32 |
| WriteProcessMemory       | &H40337C | 2088772115 | kernel32 |



有了这些数据,我们就可以利用它们来实现一个API的调用取证程序

**程序的原理:**

**Whlie 循环查看本机的程序用 ReadProcessMemory 去读取程序中的基址,若基址中的数值为API的地址的话,就可以判断它调用了这个函数**

所需API和API常量和:



```visual basic
Private Declare Function OpenProcess Lib "kernel32" (ByVal dwDesiredAccess As Long, ByVal bInheritHandle As Long, ByVal dwProcessId As Long) As Long
Private Declare Function ReadProcessMemory Lib "kernel32" (ByVal hProcess As Long, ByVal pvAddressRemote As Any, ByVal pvBufferLocal As Any, ByVal dwSize As Long, ByVal pdwNumBytesRead As Long) As Long
Private Declare Function WriteProcessMemory Lib "kernel32" (ByVal hProcess As Long, ByVal lpBaseAddress As Any, ByVal lpBuffer As Long, ByVal nSize As Long, lpNumberOfBytesWritten As Long) As Long

Private Declare Function GetProcAddress Lib "kernel32" (ByVal hModule As Long, ByVal lpProcName As String) As Long
Private Declare Function LoadLibrary Lib "kernel32" Alias "LoadLibraryA" (ByVal lpLibFileName As String) As Long

Private Declare Function Process32First Lib "kernel32" (ByVal hSnapShot As Long, lppe As PROCESSENTRY32) As Long
Private Declare Function Process32Next Lib "kernel32" (ByVal hSnapShot As Long, lppe As PROCESSENTRY32) As Long
Private Declare Function CreateToolhelp32Snapshot Lib "kernel32" (ByVal dwFlags As Long, ByVal th32ProcessID As Long) As Long
Private Declare Function CloseHandle Lib "kernel32.dll" (ByVal hObject As Long) As Long

 

Private Const PROCESS_ALL_ACCESS = &H1F0FFFPrivate Const TH32CS_SNAPPROCESS = &H2

 Private Type PROCESSENTRY32
       dwSize  As Long
       cntUsage  As Long
       th32ProcessID  As Long       th32DefaultHeapID  As Long
       th32ModuleID  As Long
       cntThreads  As Long
       th32ParentProcessID  As Long
       pcPriClassBase  As Long
       dwFlags  As Long
       szExeFile  As String * 260
 End Type
```



几个附加函数:



```visual basic
Private Function GetModuleFuncAddr(ByVal ModulePath As String, ByVal FuncStr As String) As Long           
Dim hHwnd As Long
hHwnd = LoadLibrary(ModulePath)
Dim APIAddr As Long
APIAddr = GetProcAddress(hHwnd, FuncStr)
GetModuleFuncAddr = APIAddr
End Function

Private Function GetWriteProcessMemoryAddr() As Long
GetWriteProcessMemoryAddr = GetModuleFuncAddr("C:\WINDOWS\system32\kernel32.dll", "WriteProcessMemory")
End Function
Private Function GetFindWindowAddr() As Long
GetFindWindowAddr = GetModuleFuncAddr("C:\WINDOWS\system32\user32.dll", "FindWindowA")
End Function
Private Function GetGetWindowThreadProcessIdAddr() As Long
GetGetWindowThreadProcessIdAddr = GetModuleFuncAddr("C:\WINDOWS\system32\user32.dll", "GetWindowThreadProcessId")
End Function
Private Function GetOpenProcessAddr() As Long
GetOpenProcessAddr = GetModuleFuncAddr("C:\WINDOWS\system32\kernel32.dll", "OpenProcess")
End Function   

Private Function GetMaxPrilivageToProcess(ByVal ProcessID As Long) As Long
GetMaxPrilivageToProcess = OpenProcess(PROCESS_ALL_ACCESS, False, ProcessID)
End Function

这两个最有用的读写内存函数:

Private Function GetMemory(ByVal ProcToken As Long, ByVal MemoryBaseAddr As Long) As Long
Dim GetValue As Long
 ReadProcessMemory ProcToken, MemoryBaseAddr, VarPtr(GetValue), 4, 0
GetMemory = GetValue
End Function

Private Function SetMemory(ByVal ProcToken As Long, ByVal MemoryBaseAddr As Long, ByVal Value As Long) As Boolean
If ProcToken > 0 And WriteProcessMemory(ProcToken, MemoryBaseAddr, VarPtr(Value), 4, 0) <> 0 Then
SetMemory = True
Else
SetMemory = False
End If
End Function

接下来就是关键的监控代码:

Private Sub Timer1_Timer()
  List1.Clear                                                                                                      '''  用一个List 来显示当前的调用这几个API的进程列表     
  Dim hFindProc As Long                                                                                  '''  储存一个进程搜索句柄
  hFindProc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)        ''''  获得一个进程搜索句柄
  Dim ProcData As PROCESSENTRY32                                                           ''''   定义一个搜索进程结构
  ProcData.dwSize = Len(ProcData)                                                                 ''''  必须要这样
  Process32First hFindProc, ProcData                               ''''  获取第一个进程的信息,一般这是system进程,所以它不可能是G程序
While (Process32Next(hFindProc, ProcData) = 1)             '''''  循环获取当前系统进程的列表
  Dim ProcMaxToken As Long                                            ''''  用来储存打开进程的最大权限
  ProcMaxToken = GetMaxPrilivageToProcess(ProcData.th32ProcessID)
  If (GetMemory(ProcMaxToken, &H40337C) = GetWriteProcessMemoryAddr And _                ''''   判断这个IAT中是否是这个API的地址
  GetMemory(ProcMaxToken, &H40334C) = GetGetWindowThreadProcessIdAddr And _
  GetMemory(ProcMaxToken, &H403364) = GetOpenProcessAddr And _
  GetMemory(ProcMaxToken, &H403340) = GetFindWindowAddr) Then                                ''''   如果条件符合
  List1.AddItem (ProcData.szExeFile)                                                                                       ''''  添加到列表中                                 
  End If
Wend
  CloseHandle hFindProc
End Sub
```



上面的代码就是一个取证过程,那么对于这个外挂我们怎么去对付呢,一般情况下我们都会将这个外挂程序关闭或者是游戏程序自身关闭(TC游戏就是一个例子),而下面介绍的就是 **API转向调用** 原理

读者们有没有想到,当我们修改了这个内存地址的数据时会发现些什么呢?

由于这个内存地址是给程序来调用的,那么我们只需要将这个地址值改写后,它就会自动调用到我们所修改的新API地址去了

程序实例(再以Beep()程序为例子):

 代码如下:



```visual basic
Private Declare Function Beep Lib "kernel32" (ByVal dwFreq As Long, ByVal dwDuration As Long) As Long
Private Sub Command1_Click()
Beep 2500, 1500
End Sub
```



由上面得出,4032DC就是我们要修改的地址,我们将**4032DC**修改到**MessageBoxA**



| 函数名                  | 函数地址   |
| ----------------------- | ---------- |
| MessageBoxA(user32.dll) | 2010449898 |



修改内存的关键代码为:



```visual basic
SetMemory &H4032DC, GetModuleFuncAddr("C:\WINDOWS\system32\user32.dll", "MessageBoxA")
```



后面就是一个大家都很熟悉的效果图

 

![图片](pic_temp3\http_imgload8.jpg)



这是Beep的关键代码



```assembly
401CEF  push 00005DCh
401CF4  push 000001F4h
401CF9  Beep(%x1,%x2)
```



由于**0x000005DC**这个内存区域是被保护的,所以当程序去读取这个地址的话就会抛出这个错误

还有一种可能就是,程序去调用这个函数的时候,缺少了几个参数向这个函数传递而导致的错误

Beep是两个参数,MessageBoxA是四个参数输入的

如果将MessageBoxA换成输入一个参数的Sleep呢



```visual basic
SetMemory &H4032DC, GetModuleFuncAddr("C:\WINDOWS\system32\kernel32.dll", "Sleep")
```



输入的效果显著,没有产生内存调用错误,在另一程序中调用Beep时,程序没有响应(只是单纯的暂停一下)

就上面的结论,在我们的反外挂程序中也是同一样的道理,只不过是将&H40337C(WriteProcessMemory)的值修改成Sleep的地址

下面是一个导入地址的函数:



```visual basic
Private Sub TurnToNewAPIFun(ByVal ProcToken As Long, ByVal SteTheMemoryAddr As Long, ByVal APIAddr As Long)
SetMemory ProcToken, SteTheMemoryAddr, APIAddr
End Sub
```



将它添加到



```visual basic
  If (GetMemory(ProcMaxToken, &H40337C) = GetWriteProcessMemoryAddr And _                ''''   判断这个IAT中是否是这个API的地址
  GetMemory(ProcMaxToken, &H40334C) = GetGetWindowThreadProcessIdAddr And _
  GetMemory(ProcMaxToken, &H403364) = GetOpenProcessAddr And _
  GetMemory(ProcMaxToken, &H403340) = GetFindWindowAddr) Then                                ''''   如果条件符合

   **TurnToNewAPIFun ProcMaxToken, &H40337C, GetModuleFuncAddr("C:\WINDOWS\system32\kernel32.dll", "Sleep")** 

  List1.AddItem (ProcData.szExeFile)                                                                                       ''''  添加到列表中                                 

  End If
```

