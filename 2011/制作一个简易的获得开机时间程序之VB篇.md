其实程序的原理很简单的,也不需要很多的技术,关键是这个API



```visual basic
DWORD timeGetTime(VOID);

Private Declare Function timeGetTime Lib "winmm.dll" () As Long
```



当然,为了让程序有简洁的代码和较高性能的运行,还需要一个辅助函数



```visual basic
Private Type GetTime
Min As Long
Sec As Long
End Type

Private Function Chackmin(ByVal GetSec As Long) As GetTime
Dim Min As Long
While (1)
If GetSec >= 60 Then
GetSec = GetSec - 60
Min = Min + 1
Else
Chackmin.Min = Min
Chackmin.Sec = GetSec
Exit Function
End If
Wend

End Function
```



这个函数的用意是,给函数一个秒数,它就会自己计算这个秒数是多少分钟和多少秒

主要过程:



```visual basic
Private Sub Form_Load()
Label1.Caption = "本机开机时间为:" & Chackmin(timeGetTime \ 1000).Min & "分" & Chackmin(timeGetTime \ 1000).Sec & "秒"
End Sub
```



开机启动程序:



```visual basic
Private Sub Command1_Click()
Shell "reg add HKCU\Software\Microsoft\Windows\CurrentVersion\Run /v ShowComputerStartTimeApp /t REG_SZ /d " & """" & App.Path & "\" & App.EXEName & ".exe" & """" & " /f"
End Sub
```

