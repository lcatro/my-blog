双进程保护原理:

当一方的程序被关闭时(主进程或子进程方),立即重新运行该进程(主进程或子进程)

 

图示:



![](pic_temp2\http_imgload1.jpg)

 

如果



![](pic_temp2\http_imgload2.jpg)



然后



![](pic_temp2\http_imgload3.jpg)

 

反过来也是同样的道理

下面是一个aProc的模块



```visual basic
 Private Declare Function Process32First Lib "kernel32" (ByVal hSnapShot As Long, lppe As PROCESSENTRY32) As Long
 Private Declare Function Process32Next Lib "kernel32" (ByVal hSnapShot As Long, lppe As PROCESSENTRY32) As Long
 Private Declare Function CloseHandle Lib "Kernel32.dll" (ByVal Handle As Long) As Long
 Private Declare Function CreateToolhelp32Snapshot Lib "kernel32" (ByVal dwFlags As Long, ByVal th32ProcessID As Long) As Long
 Private Type PROCESSENTRY32
       dwSize  As Long
       cntUsage  As Long
       th32ProcessID  As Long
       th32DefaultHeapID  As Long
       th32ModuleID  As Long
       cntThreads  As Long
       th32ParentProcessID  As Long
       pcPriClassBase  As Long
       dwFlags  As Long
       szExeFile  As String * 260
 End Type
 Private Const TH32CS_SNAPPROCESS = &H2

Public Function TestProcIsRunning(ByVal ProcName As String) As Boolean      ''''  测试,当指定的进程存在时,则返回True
  Dim hFindProc As Long
  hFindProc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)
  Dim ProcData As PROCESSENTRY32
  ProcData.dwSize = Len(ProcData)
  Process32First hFindProc, ProcData
While (Process32Next(hFindProc, ProcData) = 1)
  Dim sPro As String
  sPro = Left(UCase(ProcData.szExeFile), InStr(UCase(ProcData.szExeFile), Chr(0)) - 1)   '''算得上是关键代码,由于进程名不满260位
                                                                                                                                  '''长度,最后一位自动填为Chr(0),应该去除
  If sPro = UCase(ProcName) Then      '''  采用大小写不区分的方法,有时候.exe后缀会变成大写,就像.EXE
   CloseHandle hFindProc
   TestProcIsRunning = True
   Exit Function
  End If
Wend
 CloseHandle hFindProc
 TestProcIsRunning = False
End Function
```

 

设Main进程为V1,exe,Other进程(Main子进程)为V2.exe



```visual basic
Private Sub Timer1_Timer()    '''  Main.exe Code
On Error Resume Next
If TestProcIsRunning("V2.exe") = False Then Shell App.Path & "\V2.exe", vbHide
End Sub

 

Private Sub Timer1_Timer()    '''  Other.exe Code
On Error Resume Next
If TestProcIsRunning("V1.exe") = False Then Shell App.Path & "\V1.exe", vbHide
End Sub
```



 