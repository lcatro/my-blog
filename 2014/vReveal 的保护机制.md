  首先用PEiD 查看vReveal 是否有壳:



 ![图片](pic_temp4\psb1.jpg) 



  很明显,PEiD 显示的该程序没有壳,而且代码还是用.NET 的C# / VB.NET 来写的(C# 和VB.NET 基本等价).



  然后用.NET Reflector 来反编译,得到Main 函数的代码:



```visual basic
Private Shared Function Main() As Integer
    Dim exitCode As Integer = 0
    If Program.ProcessVideoFromCommandLine(exitCode) Then
        Return exitCode
    End If
    Dim process As Process
    For Each process In Process.GetProcessesByName("vRevealR")
        If (process.MainWindowHandle <> IntPtr.Zero) Then
            New Arguments() With { _
                .files = Environment.GetCommandLineArgs _
            }.Save
            Win32API.SetForegroundWindow(process.MainWindowHandle)
            If Win32API.IsIconic(process.MainWindowHandle) Then
                Win32API.ShowWindowAsync(process.MainWindowHandle, 9)
            End If
            Dim strArray As String() = Environment.GetCommandLineArgs
            If ((strArray.Length > 1) AndAlso (strArray(1) <> "/reopen")) Then
                Win32API.PostMessage(process.MainWindowHandle, &H401, 0, 0)
            End If
            Return 1
        End If
    Next
    Dim commandLineArgs As String() = Environment.GetCommandLineArgs
    If (commandLineArgs.Length <= 1) Then
        Return 1
    End If
    If (commandLineArgs(1) <> "/launch") Then
        Return 1
    End If
    If Not Directory.Exists(CustomConstants.LocalApplicationData) Then
        Directory.CreateDirectory(CustomConstants.LocalApplicationData)
    End If
    If Not Directory.Exists(CustomConstants.ApplicationData) Then
        Directory.CreateDirectory(CustomConstants.ApplicationData)
    End If
    Program.Restore10Database
    Dim premium As Boolean = False
    If (Protection.State = Type.MDSP_REGISTERED) Then
        premium = True
    End If
    CarmelSplashForm.ShowSplashScreen(premium)
    Application.EnableVisualStyles
    Application.SetCompatibleTextRenderingDefault(True)
    AddHandler AppDomain.CurrentDomain.UnhandledException, New UnhandledExceptionEventHandler(AddressOf Program.CurrentDomain_UnhandledException)
    Application.SetUnhandledExceptionMode(UnhandledExceptionMode.ThrowException)
    AddHandler Application.ThreadException, New ThreadExceptionEventHandler(AddressOf Program.Application_ThreadException)
    Program.myCultureInfo = Program.DesktopConfigFromIni.culture
    Do
        Common.CheckResult
        Program.restart = False
        Dim info As New CultureInfo(Program.myCultureInfo)
        Thread.CurrentThread.CurrentUICulture = info
        Thread.CurrentThread.CurrentCulture = info
        Application.CurrentCulture = info
        Dim form As New MainForm
        FontPatcher.SetSystemFont(form)
        Application.Run(form)
        Protection.RefreshArmadillo
    Loop While Program.restart
    Return 0
End Function
```



  注意用粗体表示的那几行代码:1.它们表示程序必须要传入/launch 才可以启动 2.只有当保护机制的状态为注册时,程序使用才是合法的



  这里有个问题,Protection 在哪儿获取了状态呢?看来有些东西给隐藏了,这个就要问IDA了 :



![图片](pic_temp4\psb2.jpg) 



  原来是.NET Reflector 的问题,没有把它解析出来.



  其实到了这里,只需要把brtrue.s loc_36c16 注释掉就能够破解了,不过我们还想继续往下去发掘



  先看看Protection.Type 里面有哪些枚举:



![图片](pic_temp4\psb3.jpg)



  在左边的类库里面没有看到Protecton 有什么有用的函数,但是它里面有个ImportedFunctions 类,原来里面都是一些从mdspproc.dll 里面导入的函数:



![图片](pic_temp4\psb4.jpg)



  IDA 打开看看,在导入表里面点击wGetProtectionState ,转到代码段:



![图片](pic_temp4\psb5.jpg)



  这里什么都没有,大家稍微动动脑,其实代码还没有映射到这个位置,它们还在DLL 文件的.text 段里面,所以会有点偏差,于是启动OD 运行这个DLL 获取到了它的代码:



![图片](pic_temp4\psb6.jpg)



  翻译过来,意思是:



```c++
bool wGetProtectionState(ProtectionType* out_state) {
    out_state=(ProtectionType*)0x101F9078;  //  这是一个全局变量
    return true;
}
```



  先看看程序是怎么样注册的:



![图片](pic_temp4\psb7.jpg)



  由于这里是用EAX 来调用函数的,所以必须要在调试模式下进去看看EAX 跳转到哪个位置,先在这个函数头设置软中断.



![图片](pic_temp4\psb8.jpg)



![图片](pic_temp4\psb9.jpg)



  看到这里EAX 是0x05DD3D60 ,F7 进去,发现就是验证码校验函数.



![图片](pic_temp4\psb10.png)



  要实现该函数的算法逆向,我们需要认真看看注册界面给出的信息:



```txt
  Subject:vReveal Activation Code Request    [邮件主题为vReveal 激活码请求]
  Your license key                                            [你的许可码]
  Your registered email address                      [你的注册邮箱地址]
  Hardware fingerprint:5F92-7D9D                  [你的硬件标识]
```



  在实现许可码和激活码的算法对比中我们可以做这样的推测:



```visual basic
  if Activation_code=Resolve_Calculate(License_key,Hardware_Finger_key) then
      Registe Success
  Else
      ERROR
  End if
```



   那么为什么邮箱地址没有被用到呢?原因是客户端程序是不会向服务器请求该许可码的绑定邮箱地址,这个邮件地址实际上是软件开发作者用数据库来保存许可码和邮箱地址的绑定.以方便他来管理这些合法用户.



  结合上面来总结下vReveal 的保护机制:



  在vReveal 的目录下有两个可执行文件



![图片](pic_temp4\psb11.png)



  桌面的快捷方式是指向vReveal.exe 的,但是真实使用的程序却是vRevealR.exe .然后我们抛弃掉vReveal.exe 直接去启动vRevealR.exe ,发现不能正常启动.在.NET 反编译中可知,vRevealR.exe 是需要参数才可以进入的(至于使用什么参数可以用任务管理器来看[在选择列里面指定命令行]),于是在cmd 下用vRevealR.exe /launch 即可直接启动这个程序



![图片](pic_temp4\psb12.jpg)



![图片](pic_temp4\psb13.png)



  那么vReveal.exe 的意义是什么呢?在接下来的调试中,我是直接从vReveal.exe 启动vRevealR.exe 的,然后用OD 直接附加到vRevealR.exe 的时候程序卡死,但是在OD 下直接用参数启动vRevealR.exe 的时候却可以进入调试状态,由此可以推断vReveal.exe 是保护程序,防止主程序被调试.



  在反编译vRevealR.exe 时,作者使用了部分符号隐藏,使得.NET Reflector 不能正确显示部分代码,由Protection 类可知,所有的保护机制代码(读取合法状态,验证激活码)都保存在mdspproc.dll 中.mdspproc.dll 中有两处地方是修改全局变量0x101F9078 的(合法用户认证标志,DWORD 类型),一处是初始化时对它设置3 ,还一处是多线程互斥量保护修改函数.如果不希望在这些地方做JMP 指令绕过保护判断的话,那么就得把全部的注意力放到wInstallKey() 的逆向算法的解析.在此必须要注意Protection 类中导入该函数的格式[VB6 格式]:



```visual basic
Public Declare Function **wInstallKey** **Lib "mdspproc.dll"** **Alias** **"InstallKey"** (ByVal **licenseKey** As String, ByVal **activationCode** As String) As Integer 
```



  最后我们需要把Activation_code,License_key,Hardware_Finger_key 联系起来,进入到call eax 中慢慢地调试这个函数就能得出算法了.



  其实要实现破解vReveal 的过程是很简单的,我们可以把



```visual basic
If (Protection.State = Type.MDSP_REGISTERED) Then
    premium = True
End If 
```



注释掉,让它只剩下premium=True,那么接下来程序就默认它是合法的版本啦.

