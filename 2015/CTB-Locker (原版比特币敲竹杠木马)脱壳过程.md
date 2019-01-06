
  给CTB-locker 查壳,发现是查找不到的

 

![图片](pic_temp3\psb1.png)

 

  然后载入IDE 分析壳的结构,并且发现壳里面包含了很多没有用的代码



![img](pic_temp3\psb2.png)



![img](pic_temp3\psb3.png)

 

  可能很多人都会说直接利用ESP 定律就可以脱这个壳,但是对这个壳并不适合(虽然在最后把代码解出来是符合ESP  定律的),由于无用的FindResourceA充斥了整个壳的部分,所以我们只能继续往下跟踪,发现壳里面有很多这样的递归函数,并且不断调用FindResourceA
 

![img](pic_temp3\psb4.png)
 

  在壳的某个地方向堆申请了内存,为接下来的IAT 加载代码做铺垫,由寄存器窗口可以查看到申请的内存是0x00C80000 ,然后我们设置硬件写入断点,F9 运行
 

![img](pic_temp3\psb5.png)

 

  最后发现停在下面的代码段



![img](pic_temp3\psb6.png)



  此时可以看到堆里面的数据的变化过程

 

![img](pic_temp3\psb7.png)



![img](pic_temp3\psb8.png)



  最后跟踪到这个地方发现已经进入了很多次递归,这个jmp 是非常关键的,接下来它会跳到刚才在堆里面IAT加载代码
 

![img](pic_temp3\psb9.jpg)

 

  此时壳已经被递归了很多次



![img](pic_temp3\psb10.png)



  接下来我们跳过复杂的IAT 加载函数



![img](pic_temp3\psb11.png)
 

  经过几次Shift + F9 ,最后停在了这个位置,OllyDump 之后查壳,脱壳成功
 

![img](pic_temp3\psb12.png)



  就算把壳脱出来了,这个也不是真正的病毒程序,它只不过是充当下载者的方式而已,真正的病毒需要从下面的站点来下载的,而且下载的压缩包还是经过加密的.
 

![img](pic_temp3\psb13.png)

 

![img](pic_temp3\psb14.png)



  最后对下载病毒的分析如下
 

![img](pic_temp3\psb15.png)
 



  程序首先会弹出警告文档,然后伪造成Firefox 浏览器去访问windows 的补丁下载网站,以此来判断是否已经连通网络  .如果可以连通网络的话就从指定的站点下载病毒文件



```assembly
.text:00401C48 ; int __cdecldown_a_file_and_execute_and_delete(LPCVOID lpBuffer, DWORDnNumberOfBytesToWrite)
.text:00401C48down_a_file_and_execute_and_delete proc near ; CODE XREF: start+2EAp
.text:00401C48
.text:00401C48 FileName        = word ptr -404h
.text:00401C48 NumberOfBytesWritten= dwordptr -4
.text:00401C48 lpBuffer        = dword ptr  8
.text:00401C48 nNumberOfBytesToWrite= dwordptr  0Ch
.text:00401C48
.text:00401C48                 push    ebp
.text:00401C49                 mov     ebp, esp
.text:00401C4B                 sub     esp, 404h
.text:00401C51                 push    esi
.text:00401C52                 push    edi
.text:00401C53                 call    ds:GetTickCount
.text:00401C59                 push    eax
.text:00401C5A                 push    ds:temp_path
.text:00401C60                 lea     eax, [ebp+FileName]
.text:00401C66                 push    offset aSD_exe  ; "%s%d.exe"
.text:00401C6B                 push    200h
.text:00401C70                 push    eax
.text:00401C71                 call    ds:wnsprintfW
.text:00401C77                 add     esp, 14h
.text:00401C7A                 xor     edi, edi
.text:00401C7C                 push    edi             ; hTemplateFile
.text:00401C7D                 push    80h             ; dwFlagsAndAttributes
.text:00401C82                 push    2               ; dwCreationDisposition
.text:00401C84                 push    edi             ; lpSecurityAttributes
.text:00401C85                 push    1               ; dwShareMode
.text:00401C87                 push    40000000h       ; dwDesiredAccess
.text:00401C8C                 lea     eax, [ebp+FileName]
.text:00401C92                 push    eax             ; lpFileName
.text:00401C93                 call    ds:CreateFileW
.text:00401C99                 mov     esi, eax
.text:00401C9B                 cmp     esi, 0FFFFFFFFh
.text:00401C9E                 jz      short loc_401CEC
.text:00401CA0                 push    ebx
.text:00401CA1                 push    edi             ; lpOverlapped
.text:00401CA2                 lea     eax, [ebp+NumberOfBytesWritten]
.text:00401CA5                 push    eax             ; lpNumberOfBytesWritten
.text:00401CA6                 push    [ebp+nNumberOfBytesToWrite] ;nNumberOfBytesToWrite
.text:00401CA9                 push    [ebp+lpBuffer]  ; lpBuffer
.text:00401CAC                 push    esi             ; hFile
.text:00401CAD                 call    ds:WriteFile
.text:00401CB3                 push    esi             ; hObject
.text:00401CB4                 mov     ebx, eax
.text:00401CB6                 call   ds:CloseHandle
.text:00401CBC                 cmp     ebx, edi
.text:00401CBE                 pop     ebx
.text:00401CBF                 jz      short loc_401CEC
.text:00401CC1                 push    0Ah
.text:00401CC3                 push    edi
.text:00401CC4                 push    edi
.text:00401CC5                 lea     eax, [ebp+FileName]
.text:00401CCB                 push    eax
.text:00401CCC                 push    edi
.text:00401CCD                 push    edi
.text:00401CCE                 call    ds:ShellExecuteW
.text:00401CD4                 push    10000           ; dwMilliseconds
.text:00401CD9                 call    ds:Sleep
.text:00401CDF                 lea     eax, [ebp+FileName]
.text:00401CE5                 push    eax             ; lpFileName
.text:00401CE6                 call    ds:DeleteFileW
.text:00401CEC
.text:00401CEC loc_401CEC:                             ; CODE XREF:down_a_file_and_execute_and_delete+56j
.text:00401CEC                                         ;down_a_file_and_execute_and_delete+77j
.text:00401CEC                 pop     edi
.text:00401CED                 pop     esi
.text:00401CEE                 leave
.text:00401CEF                 retn
```



  下载完成的文件是被加密的只有,通过decode_string 来把解压包解密采能够正确读取里面的代码
 
