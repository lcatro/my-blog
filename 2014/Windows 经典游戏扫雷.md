

##  游戏运行原理



   此次分析的扫雷游戏是Windows XP 下自带的版本,采用VC++ 7 调试版编译的Win32 程序.整个程序的运行原理如下:



![](pic_temp5\psb1.jpg)



  我们一起结合程序启动时的图片来分析整个消息系统是如何构建和运作的.

 

![图片](pic_temp5\psb2.jpg)



  在Win32 窗口程序里面,程序首先要做的第一步就是注册窗口(在此省略程序的初始化),然后创建窗口,最后显示窗口.当程序最后显示窗口的时候,Windows 会给程序创建指定大小的空白窗口再给该程序发送WM_PRINT 消息,通知它在已经创建好的窗口上绘画控件和图形.我们可以做一个假设:除了菜单栏之外都是图形.也许你会有疑问为什么要这样区分?因为当用户用鼠标左键点击到窗口的时候是发送WM_LBUTTONDOWN 和WM_LBUTTONUP ,但是点击菜单栏却有所不同,此时Windows 是返回WM_COMMAND 给程序处理的.

  这里需要讨论关于鼠标左键的细节问题,也就是Windows 给程序发送WM_LBUTTONDOWN 和WM_LBUTTONUP 会产生什么反应.

 

鼠标单击按下时程序的响应



![图片](pic_temp5\psb3.jpg)



鼠标单击松开时程序的响应



![图片](http://r.photo.store.qq.com/psb?/9707bb18-5c9b-4b76-9222-ec0fdbbff7eb/5BpCMk*CH4G1VkM81MyFlqixpBRq*QtxsMX9eoirBuY!/o/dCuP4nSwIgAA&ek=1&kp=1&pt=0&bo=qgD*AKoA*wADACU!&tl=1&su=0152759601&tm=1546765200&sce=0-12-12&rf=2-9)



鼠标单击松开时程序的响应

 

![图片](pic_temp5\psb5.jpg)



  由上面的测试可推导出结论:鼠标按下时程序单纯做动态显示(其实还有刷新地图),鼠标松开时判断选中该点是否踩到雷. 



  接下来开始对扫雷程序进行深度分析:



  在反汇编的时候MainWndProc 有下面的代码:



```assembly
01001FAE  |.  FF75 14       push [arg.4]
01001FB1  |.  E8 56F4FFFF   call winmine.0100140C
01001FB6  |.  85C0          test eax,eax
01001FB8  |.^ 0F85 A0FCFFFF jnz winmine.01001C5E          ;  如果点击到笑脸就跳转..
01001FBE  |.  841D 00500001 test byte ptr ds:[0x1005000],bl
01001FC4  |.  0F84 DF010000 je winmine.010021A9
01001FCA  |.  8B45 10       mov eax,[arg.3]
01001FCD  |.  24 06         and al,0x6
01001FCF  |.  F6D8          neg al
01001FD1  |.  1BC0          sbb eax,eax
01001FD3  |.  F7D8          neg eax
01001FD5  |.  A3 44510001   mov dword ptr ds:[0x1005144],eax
```



  注意代码中的`call winmine.0100140C` ,程序给它传递的第四个参数是单击窗口的鼠标位置(这个其实就是LPARAM ,里面的数据格式为y<<16+x ).我们再进去到winmine.0100140C 查看代码.



```assembly
0100140C  /$  55            push ebp
0100140D  |.  8BEC          mov ebp,esp
0100140F  |.  83EC 2C       sub esp,0x2C
01001412  |.  8B45 08       mov eax,[arg.1]
01001415  |.  0FB7C8        movzx ecx,ax                             ;  取低16 位作x 坐标..
01001418  |.  C1E8 10       shr eax,0x10                             ;  取高16 位作y 坐标..
0100141B  |.  8945 EC       mov [local.5],eax
0100141E  |.  A1 2C5B0001   mov eax,dword ptr ds:[0x1005B2C]
01001423  |.  53            push ebx
01001424  |.  83C0 E8       add eax,-0x18
01001427  |.  56            push esi
01001428  |.  FF75 EC       push [local.5]                           ; /Point.Y
0100142B  |.  8B35 E8100001 mov esi,dword ptr ds:[<&USER32.PtInRect>>; |user32.PtInRect
01001431  |.  D1F8          sar eax,1                                ; |
01001433  |.  8945 F0       mov [local.4],eax                        ; |
01001436  |.  83C0 18       add eax,0x18                             ; |
01001439  |.  8945 F8       mov [local.2],eax                        ; |
0100143C  |.  51            push ecx                                 ; |Point.X
0100143D  |.  8D45 F0       lea eax,[local.4]                        ; |
01001440  |.  33DB          xor ebx,ebx                              ; |
01001442  |.  50            push eax                                 ; |pRect
01001443  |.  43            inc ebx                                  ; |
01001444  |.  894D E8       mov [local.6],ecx                        ; |
01001447  |.  C745 F4 10000>mov [local.3],0x10                       ; |
0100144E  |.  C745 FC 28000>mov [local.1],0x28                       ; |
01001455  |.  FFD6          call esi                                 ; \PtInRect
01001457  |.  85C0          test eax,eax
01001459  |.  0F84 85000000 je winmine.010014E4                      ;  判断是否为点中笑脸..
0100145F  |.  57            push edi                                                      ;  初始化扫雷地图
01001460  |.  FF35 245B0001 push dword ptr ds:[0x1005B24]            ; /hWnd = 0004086C ('扫雷',class='扫雷')
01001466  |.  FF15 E4100001 call dword ptr ds:[<&USER32.SetCapture>] ; \SetCapture
0100146C  |.  6A 04         push 0x4
0100146E  |.  E8 A0140000   call winmine.01002913
01001473  |.  6A 02         push 0x2                                 ; /nPoints = 0x2
01001475  |.  8D45 F0       lea eax,[local.4]                        ; |
01001478  |.  50            push eax                                 ; |pPoints
01001479  |.  6A 00         push 0x0                                 ; |hWndTo = NULL
0100147B  |.  FF35 245B0001 push dword ptr ds:[0x1005B24]            ; |hWndFrom = 0004086C ('扫雷',class='扫雷')
01001481  |.  FF15 E0100001 call dword ptr ds:[<&USER32.MapWindowPoi>; \MapWindowPoints
01001487  |.  BF 00020000   mov edi,0x200
0100148C  |>  6A 01         /push 0x1                                ; /RemoveMsg = PM_REMOVE
0100148E  |.  68 0D020000   |push 0x20D                              ; |MsgFilterMax = WM_XBUTTONDBLCLK
01001493  |.  57            |push edi                                ; |MsgFilterMin
01001494  |.  FF35 245B0001 |push dword ptr ds:[0x1005B24]           ; |hWnd = 0004086C ('扫雷',class='扫雷')
0100149A  |.  8D45 D4       |lea eax,[local.11]                      ; |
0100149D  |.  50            |push eax                                ; |pMsg
0100149E  |.  FF15 DC100001 |call dword ptr ds:[<&USER32.PeekMessage>; \PeekMessageW
010014A4  |.  85C0          |test eax,eax
010014A6  |.^ 74 E4         |je Xwinmine.0100148C
010014A8  |.  8B45 D8       |mov eax,[local.10]
010014AB  |.  2BC7          |sub eax,edi
010014AD  |.  74 3B         |je Xwinmine.010014EA
010014AF  |.  48            |dec eax
010014B0  |.  48            |dec eax
010014B1  |.^ 75 D9         \jnz Xwinmine.0100148C
010014B3  |.  85DB          test ebx,ebx
010014B5  |.  5F            pop edi
010014B6  |.  74 23         je Xwinmine.010014DB
010014B8  |.  FF75 EC       push [local.5]
010014BB  |.  8D45 F0       lea eax,[local.4]
010014BE  |.  FF75 E8       push [local.6]
010014C1  |.  50            push eax
010014C2  |.  FFD6          call esi
010014C4  |.  85C0          test eax,eax
010014C6  |.  74 13         je Xwinmine.010014DB
010014C8  |.  8325 60510001>and dword ptr ds:[0x1005160],0x0
010014CF  |.  6A 00         push 0x0
010014D1  |.  E8 3D140000   call winmine.01002913
010014D6  |.  E8 9F210000   call winmine.0100367A
010014DB  |>  FF15 D8100001 call dword ptr ds:[<&USER32.ReleaseCaptu>; [ReleaseCapture
010014E1  |.  33C0          xor eax,eax
010014E3  |.  40            inc eax
010014E4  |>  5E            pop esi
010014E5  |.  5B            pop ebx
010014E6  |.  C9            leave
010014E7  |.  C2 0400       retn 0x4
```



  在我们点到笑脸的时候,程序刷新整个地图,最后使MainWndProc 返回0 .下面分析点击到地图时程序的响应(在处理完switch WM_LBUTTONDOWN 最后的时候有一个跳转到这里的指令):



```assembly
0100205F  |> \FF75 08       push [arg.1]                             ; /hWnd
01002062  |.  FF15 E4100001 call dword ptr ds:[<&USER32.SetCapture>] ; \SetCapture
01002068  |.  830D 18510001>or dword ptr ds:[0x1005118],0xFFFFFFFF
0100206F  |.  830D 1C510001>or dword ptr ds:[0x100511C],0xFFFFFFFF
01002076  |.  53            push ebx
01002077  |.  891D 40510001 mov dword ptr ds:[0x1005140],ebx
0100207D  |.  E8 91080000   call winmine.01002913                    ;  设置地图象素..
01002082  |.  8B4D 14       mov ecx,[arg.4]
01002085  |>  393D 40510001 cmp dword ptr ds:[0x1005140],edi
0100208B  |.  74 34         je Xwinmine.010020C1
0100208D  |.  841D 00500001 test byte ptr ds:[0x1005000],bl
01002093  |.^ 0F84 54FFFFFF je winmine.01001FED
01002099  |.  8B45 14       mov eax,[arg.4]
0100209C  |.  C1E8 10       shr eax,0x10
0100209F  |.  83E8 27       sub eax,0x27
010020A2  |.  C1F8 04       sar eax,0x4
010020A5  |.  50            push eax
010020A6  |.  0FB745 14     movzx eax,word ptr ss:[ebp+0x14]
010020AA  |.  83C0 04       add eax,0x4
010020AD  |.  C1F8 04       sar eax,0x4
010020B0  |.  50            push eax
010020B1  |>  E8 1E110000   call winmine.010031D4                    ;  在地图上画空框..
010020B6  |.  E9 EE000000   jmp winmine.010021A9                    ;  退出消息处理
```



  接下来开始分析鼠标左键松开时的代码:



```assembly
01001FDF  |> \33FF          xor edi,edi                             ;  Cases 202 (WM_LBUTTONUP),205 (WM_RBUTTONUP),208 (WM_MBUTTONUP) of switch 01001F5F
01001FE1  |.  393D 40510001 cmp dword ptr ds:[0x1005140],edi
01001FE7  |.  0F84 BC010000 je winmine.010021A9
01001FED  |>  893D 40510001 mov dword ptr ds:[0x1005140],edi
01001FF3  |.  FF15 D8100001 call dword ptr ds:[<&USER32.ReleaseCapt>; [ReleaseCapture   恢复该点到原来的状态
01001FF9  |.  841D 00500001 test byte ptr ds:[0x1005000],bl
01001FFF  |.  0F84 B6000000 je winmine.010020BB
01002005  |.  E8 D7170000   call <winmine.判断这里是否有雷..>               ;  只要点击到该窗口采可以调用这个函数来响应..
0100200A  |.  E9 9A010000   jmp winmine.010021A9
```



  这个switch case 的情况比较复杂,由于它是同时处理三个鼠标的松开事件,但是在这里程序没有写怎么样去应对每个不同的事件.在实际测试程序的过程中只有,WM_LBUTTONUP 和WM_MBUTTONUP 被处理,继续向下分析



```assembly
010037E1 >/$  A1 18510001   mov eax,dword ptr ds:[0x1005118]
010037E6  |.  85C0          test eax,eax
010037E8  |.  0F8E C8000000 jle winmine.010038B6
010037EE  |.  8B0D 1C510001 mov ecx,dword ptr ds:[0x100511C]
010037F4  |.  85C9          test ecx,ecx
010037F6  |.  0F8E BA000000 jle winmine.010038B6
010037FC  |.  3B05 34530001 cmp eax,dword ptr ds:[0x1005334]
01003802  |.  0F8F AE000000 jg winmine.010038B6
01003808  |.  3B0D 38530001 cmp ecx,dword ptr ds:[0x1005338]
0100380E  |.  0F8F A2000000 jg winmine.010038B6
01003814  |.  53            push ebx
01003815  |.  33DB          xor ebx,ebx
01003817  |.  43            inc ebx
01003818  |.  833D A4570001>cmp dword ptr ds:[0x10057A4],0x0
0100381F  |.  75 4A         jnz Xwinmine.0100386B                   ;  判断是否开始..
01003821  |.  833D 9C570001>cmp dword ptr ds:[0x100579C],0x0
01003828  |.  75 41         jnz Xwinmine.0100386B
0100382A  |.  53            push ebx
0100382B  |.  E8 BD000000   call winmine.010038ED
01003830  |.  FF05 9C570001 inc dword ptr ds:[0x100579C]
01003836  |.  E8 7AF0FFFF   call winmine.010028B5
0100383B  |.  6A 00         push 0x0                                ; /Timerproc = NULL
0100383D  |.  68 E8030000   push 0x3E8                              ; |Timeout = 1000. ms
01003842  |.  53            push ebx                                ; |TimerID
01003843  |.  FF35 245B0001 push dword ptr ds:[0x1005B24]           ; |hWnd = 0004086C ('扫雷',class='扫雷')
01003849  |.  891D 64510001 mov dword ptr ds:[0x1005164],ebx        ; |
0100384F  |.  FF15 B4100001 call dword ptr ds:[<&USER32.SetTimer>]  ; \扫雷用时计算器
01003855  |.  85C0          test eax,eax
01003857  |.  75 07         jnz Xwinmine.01003860
01003859  |.  6A 04         push 0x4
0100385B  |.  E8 F0000000   call winmine.01003950
01003860  |>  A1 18510001   mov eax,dword ptr ds:[0x1005118]
01003865  |.  8B0D 1C510001 mov ecx,dword ptr ds:[0x100511C]
0100386B  |>  841D 00500001 test byte ptr ds:[0x1005000],bl
01003871  |.  5B            pop ebx
01003872  |.  75 10         jnz Xwinmine.01003884                   ;  应该是一些初始化一些设置..
01003874  |.  6A FE         push -0x2
01003876  |.  59            pop ecx
01003877  |.  8BC1          mov eax,ecx
01003879  |.  890D 1C510001 mov dword ptr ds:[0x100511C],ecx
0100387F  |.  A3 18510001   mov dword ptr ds:[0x1005118],eax
01003884  |>  833D 44510001>cmp dword ptr ds:[0x1005144],0x0
0100388B  |.  74 09         je Xwinmine.01003896                    ;  判断是否为鼠标中键松开..
0100388D  |.  51            push ecx
0100388E  |.  50            push eax
0100388F  |.  E8 23FDFFFF   call winmine.010035B7
01003894  |.  EB 20         jmp Xwinmine.010038B6
01003896  |>  8BD1          mov edx,ecx
01003898  |.  C1E2 05       shl edx,0x5
0100389B  |.  8A9402 405300>mov dl,byte ptr ds:[edx+eax+0x1005340]
010038A2  |.  F6C2 40       test dl,0x40
010038A5  |.  75 0F         jnz Xwinmine.010038B6
010038A7  |.  80E2 1F       and dl,0x1F
010038AA  |.  80FA 0E       cmp dl,0xE
010038AD  |.  74 07         je Xwinmine.010038B6
010038AF  |.  51            push ecx
010038B0  |.  50            push eax
010038B1  |.  E8 5CFCFFFF   call winmine.01003512                   ;  开始根据地图数据来判断是否踩雷..
010038B6  |>  FF35 60510001 push dword ptr ds:[0x1005160]
010038BC  |.  E8 52F0FFFF   call winmine.01002913                   ;  在地图上面显示位图..
010038C1  \.  C3            retn
010038C2  /$  6A 40         push 0x40
010038C4  |.  6A 00         push 0x0
010038C6  |.  6A 00         push 0x0
010038C8  |.  FF15 68110001 call dword ptr ds:[<&WINMM.PlaySoundW>] ;  winmm.PlaySoundW
010038CE  |.  F7D8          neg eax
010038D0  |.  1BC0          sbb eax,eax
010038D2  |.  F7D8          neg eax
010038D4  |.  40            inc eax
010038D5  |.  40            inc eax
010038D6  \.  C3            retn
```



  在这里我们对扫雷是如何工作很有兴趣,于是进去继续查看代码:



```assembly
01003512  /$  8B4424 08     mov eax,dword ptr ss:[esp+0x8]
01003516  |.  53            push ebx
01003517  |.  55            push ebp
01003518  |.  56            push esi
01003519  |.  8B7424 10     mov esi,dword ptr ss:[esp+0x10]
0100351D  |.  8BC8          mov ecx,eax
0100351F  |.  C1E1 05       shl ecx,0x5
01003522  |.  8D9431 405300>lea edx,dword ptr ds:[ecx+esi+0x1005340>;  获取点击的位置(是在地图里面的位置)..
01003529  |.  F602 80       test byte ptr ds:[edx],0x80
0100352C  |.  57            push edi
0100352D      74 66         je Xwinmine.01003595                    ;  判断是否踩到了雷
0100352F  |.  833D A4570001>cmp dword ptr ds:[0x10057A4],0x0
01003536  |.  75 50         jnz Xwinmine.01003588
01003538  |.  8B2D 38530001 mov ebp,dword ptr ds:[0x1005338]
0100353E  |.  33C0          xor eax,eax
01003540  |.  40            inc eax
01003541  |.  3BE8          cmp ebp,eax
01003543  |.  7E 6B         jle Xwinmine.010035B0
01003545  |.  8B1D 34530001 mov ebx,dword ptr ds:[0x1005334]
0100354B  |.  BF 60530001   mov edi,winmine.01005360
01003550  |>  33C9          /xor ecx,ecx                            ;  应该是循环输出整个地图的情况..
01003552  |.  41            |inc ecx
01003553  |.  3BD9          |cmp ebx,ecx
01003555  |.  7E 0B         |jle Xwinmine.01003562
01003557  |>  F6040F 80     |/test byte ptr ds:[edi+ecx],0x80
0100355B  |.  74 0F         ||je Xwinmine.0100356C
0100355D  |.  41            ||inc ecx
0100355E  |.  3BCB          ||cmp ecx,ebx
01003560  |.^ 7C F5         |\jl Xwinmine.01003557
01003562  |>  40            |inc eax
01003563  |.  83C7 20       |add edi,0x20
01003566  |.  3BC5          |cmp eax,ebp
01003568  |.^ 7C E6         \jl Xwinmine.01003550
0100356A  |.  EB 44         jmp Xwinmine.010035B0
0100356C  |>  FF7424 18     push dword ptr ss:[esp+0x18]
01003570  |.  C1E0 05       shl eax,0x5
01003573  |.  8D8408 405300>lea eax,dword ptr ds:[eax+ecx+0x1005340>
0100357A  |.  C602 0F       mov byte ptr ds:[edx],0xF
0100357D  |.  8008 80       or byte ptr ds:[eax],0x80
01003580  |.  56            push esi
01003581  |.  E8 FEFAFFFF   call winmine.01003084
01003586  |.  EB 28         jmp Xwinmine.010035B0
01003588  |>  6A 4C         push 0x4C
0100358A  |.  50            push eax
0100358B  |.  56            push esi
0100358C  |.  E8 1AF9FFFF   call winmine.01002EAB
01003591  |.  6A 00         push 0x0
01003593  |.  EB 16         jmp Xwinmine.010035AB
01003595  |>  50            push eax
01003596  |.  56            push esi
01003597  |.  E8 E8FAFFFF   call winmine.01003084
0100359C  |.  A1 A4570001   mov eax,dword ptr ds:[0x10057A4]
010035A1  |.  3B05 A0570001 cmp eax,dword ptr ds:[0x10057A0]
010035A7  |.  75 07         jnz Xwinmine.010035B0                           ;  判断游戏是否结束
010035A9  |.  6A 01         push 0x1
010035AB  |>  E8 CCFEFFFF   call winmine.0100347C                  ;  显示所有地图数据
010035B0  |>  5F            pop edi
010035B1  |.  5E            pop esi
010035B2  |.  5D            pop ebp
010035B3  |.  5B            pop ebx
010035B4  \.  C2 0800       retn 0x8
```



  我们在`01003529  |.  F602 80       test byte ptr ds:[edx],0x80` 处下断点,发现地图上的第一个位置的内存地址是0x1005361 ,于是用CE 进去详细查看内存分布情况:

 

![图片](pic_temp5\psb6.jpg)



  大家仔细观察,周围有一条用0x10 围起来的边框,但是左边却没有,是不是地图的内存数据起始地址不是0x1005361 而是0x1005360 呢?让我们再次读取内存数据:



![图片](pic_temp5\psb7.jpg)



  这样一来,0x10 就是只地图的边缘啦.那么地图中的0x0F 是什么呢?我们可以选中其中一个位置来测试.
 

![图片](pic_temp5\psb8.jpg)



  然后我们再测试一下点击0x8F .

 

![图片](pic_temp5\psb9.jpg)



  由这两次测试可以推断,0x8F 是指地雷,0x0F 是指什么都没有.此时我们再查看一下内存数据.

 

![图片](pic_temp5\psb10.jpg)



内存数据转换为ASCII 码



![图片](http://r.photo.store.qq.com/psb?/9707bb18-5c9b-4b76-9222-ec0fdbbff7eb/UJ7dp04ir0OqrNup2HpMG.*SV7vX4mJsLoapPVZYhpQ!/o/dDpWrnPXIgAA&ek=1&kp=1&pt=0&bo=ZgC3AGYAtwADACU!&tl=1&su=0223974513&tm=1546765200&sce=0-12-12&rf=2-9)



  由此可知,@ 是指附近八个格子没有地雷,A 是指存在1 个地雷,B 是指存在2 个地雷,以此类推.



##  利用游戏原理实现作弊



  有了上面的探索,那么我们可以根据这些结论来写作弊软件啦.



1. 无视踩雷



  在这里我们利用爆破的方式把其中的两个关键跳转点修改即可实现一次点击地图然后游戏胜利.



```assembly
0100352D      74 66         je Xwinmine.01003595                    ;  判断是否踩到了雷
010035A7      75 07         jnz Xwinmine.010035B0                   ;  判断游戏是否结束
```



  当我们踩到了地雷,代码



```assembly
01003529  |.  F602 80       test byte ptr ds:[edx],0x80
```



  就会使ZF 标志位置0 ,此时je 跳转失效,继续向下执行,在此我们要希望突破保护,应该把je 跳转改写为jmp .



```assembly
0100352D     /EB 66         jmp Xwinmine.01003595                    ;  踩到了雷都要往下跳..
```



  在函数最后的地方有以下代码:



```assembly
010035A1  |.  3B05 A0570001 cmp eax,dword ptr ds:[0x10057A0]
010035A7      75 07         jnz Xwinmine.010035B0                      ;  判断游戏是否结束
010035A9  |.  6A 01         push 0x1                                             ;  传递true 给下面的函数
010035AB  |>  E8 CCFEFFFF   call winmine.0100347C             ;  显示所有地图数据
010035B0  |>  5F            pop edi                                                ;  平衡堆栈和恢复寄存器
010035B1  |.  5E            pop esi
010035B2  |.  5D            pop ebp
010035B3  |.  5B            pop ebx
010035B4  \.  C2 0800       retn 0x8
```



  于是在此只需要把jnz 改写为nop 使代码默认执行显示所有地图数据即可,先来测试一下.

 

![图片](pic_temp5\psb12.jpg)



2. 地雷位置读取器



 细心的读者可能会发现地图的第一行边界与第二行边界之间的数据地址相差0x17 .此时的内存数据分布如下

 

![图片](pic_temp5\psb13.png)



  大家可以清楚地看到,地址0x01005381-0x01005369=0x17 .那么我们就可以根据这个结论来写地图数据读取器啦



```c++
#define MINE_MAP_LINE        9
#define MINE_MAP_COMULATE    9
#define MAP_BASE_ADDRESS     0x1005361

void get_buffer_data(memory process,unsigned char* output_buffer) {
    unsigned long read_address=MAP_BASE_ADDRESS;
    unsigned char* write_mine_buffer=output_buffer;

    for (int i=0;i<MINE_MAP_LINE;++i,read_address+=0x17)
        for (int o=0;o<MINE_MAP_COMULATE;++o,read_address++)
            *write_mine_buffer++=get_memory_char(process,read_address);
}
```





  可能读者们会感到疑惑,ReadProcessMemory 去哪儿了?其实在写代码的时候已经把他们封装到文件里去了,关于ReadProcessMemory 我们就先省略吧.



  获得了地图数据,下一步就是输出.



```c++
#include <stdio.h>
#include "ui_window.h"

static void output_flag(unsigned char in_flag_num) {
    switch (in_flag_num) {
        case 0x10:
            printf("| ");
            break;
        case 0x40:
            printf("  ");
            break;
        case 0x41:
            printf("1 ");
            break;
        case 0x42:
            printf("2 ");
            break;
        case 0x43:
            printf("3 ");
            break;
        case 0x44:
            printf("4 ");
            break;
        case 0x45:
            printf("5 ");
            break;
        case 0x0F:
            printf("_ ");
            break;
        case 0x8E:
            printf("+ ");
            break;
        case 0x8F:
            printf("* ");
            break;
    }
}

void output_print(mine_buffer in_buffer) {
    if (in_buffer.line && in_buffer.comulate) {
        printf("\n\n\n\n");
        unsigned char* output_data_point=in_buffer.data_buffer;
        for (unsigned long line=0;line<in_buffer.line;++line) {
            for (unsigned long comulate=0;comulate<in_buffer.comulate;++comulate,++output_data_point)
                output_flag(*output_data_point);
            printf("\n");
        }
    } else
        printf("ERR!");
}
```



  最后把它们组合在一起.



```c++
    memory mine_memory=create_memory_by_form_name(WINDOW_TITEL);
    unsigned char* mine_buffer_data=(unsigned char*)malloc(MINE_MAP_SIZE);
    if (mine_memory!=INVALID_MEMORY && mine_buffer_data!=INVALID_MEMORY) {
        memset(mine_buffer_data,0,MINE_MAP_SIZE);
        mine_buffer new_cheat;
        new_cheat.line=MINE_MAP_LINE;
        new_cheat.comulate=MINE_MAP_COMULATE;
        new_cheat.data_buffer=mine_buffer_data;
        get_buffer_data(mine_memory,mine_buffer_data);
        output_print(new_cheat);
        free(mine_buffer_data);
        destory_memory(mine_memory);
    } else
        printf("no mine game!");
```



  运行结果.

 

![图片](pic_temp5\psb14.jpg)



3. 自动点击



  在前面的消息调试时,我们发现扫雷程序是在处理**WM_LBUTTONUP** 时才判断是否踩到地雷的,但是处理**WM_LBUTTONUP** 之前又需要处理**WM_LBUTTONDOWN** ,那么这就为接下来的代码编写提供了思路



  首先作弊程序需要一个分析函数:



```c++
unsigned long analysis(unsigned char* input_buffer,unsigned long find_index,unsigned long buffer_length) {
    for (;find_index<buffer_length;++find_index)
        if (*(input_buffer+find_index)==MINE_FLAG_NOMINE)
            return find_index;
    return ANALYSIS_ERROR;
}
```



  分析函数的返回值是当前检索的地图位置,然后我们根据这个位置来想扫雷程序发送**WM_LUBTTONUP** 消息:



```c++
void mouse_left_click_up(window click_window,point x,point y) {
    if (INVALID_WINDOW!=click_window) {
        long click_point=y;
        click_point<<=16;
        click_point+=x;
        SendMessage((HWND)click_window,WM_LBUTTONUP,0,click_point);
    }
}
```



  把它们组合起来就是这样的.



```c++
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include "io_message.h"
#include "io_memory.h"
#include "ui_window.h"

#define WINDOW_TITEL         "扫雷"
#define MAP_BASE_ADDRESS     0x1005361
#define MINE_MAP_LINE        9
#define MINE_MAP_COMULATE    9
#define MINE_MAP_SIZE        MINE_MAP_LINE*MINE_MAP_COMULATE
#define MINE_MAP_TO_LEFT     20
#define MINE_MAP_TO_TOP      64
#define MINE_WIDTH           16
#define MINE_HEIGHT          MINE_WIDTH
#define MINE_FLAG_NOMINE     0x0F

#define ANALYSIS_ERROR       0xFFFFFFFF

void get_buffer_data(memory process,unsigned char* output_buffer) {
    unsigned long read_address=MAP_BASE_ADDRESS;
    unsigned char* write_mine_buffer=output_buffer;
    for (int i=0;i<MINE_MAP_LINE;++i,read_address+=0x17)
        for (int o=0;o<MINE_MAP_COMULATE;++o,read_address++)
            *write_mine_buffer++=get_memory_char(process,read_address);

}

unsigned long analysis(unsigned char* input_buffer,unsigned long find_index,unsigned long buffer_length) {
    for (;find_index<buffer_length;++find_index)
        if (*(input_buffer+find_index)==MINE_FLAG_NOMINE)
            return find_index;

    return ANALYSIS_ERROR;
}

void main(void) {
    memory mine_memory=create_memory_by_form_name(WINDOW_TITEL);
    window mine_window=create_window(WINDOW_TITEL);
    unsigned char* mine_buffer_data=(unsigned char*)malloc(MINE_MAP_SIZE);
    if (mine_memory!=INVALID_MEMORY && mine_window!=INVALID_WINDOW && mine_buffer_data!=INVALID_MEMORY) {
        memset(mine_buffer_data,0,MINE_MAP_SIZE);
        
        mine_buffer new_cheat;
        new_cheat.line=MINE_MAP_LINE;
        new_cheat.comulate=MINE_MAP_COMULATE;
        new_cheat.data_buffer=mine_buffer_data;
        get_buffer_data(mine_memory,mine_buffer_data);

        output_print(new_cheat);

        unsigned long find_index=0;

        while (ANALYSIS_ERROR!=(find_index=analysis(mine_buffer_data,find_index,MINE_MAP_SIZE))) {
            unsigned long y=find_index/MINE_MAP_LINE;
            unsigned long x=find_index%MINE_MAP_LINE;
            
            y=y*MINE_HEIGHT+MINE_MAP_TO_TOP;
            x=x*MINE_WIDTH+MINE_MAP_TO_LEFT;
            mouse_left_click_down(mine_window,x,y);
            mouse_left_click_up(mine_window,x,y);
            get_buffer_data(mine_memory,mine_buffer_data);
        }

        output_print(new_cheat);
        free(mine_buffer_data);
        destory_memory(mine_memory);

    } else
        printf("no mine game!");
}
```



  运行结果:



![](pic_temp5\psb15.jpg)

 

4. 后话



  在文章的最后,提一下对重要的判断一定要做好防御和隐藏工作是很重要的



  其实上面爆破关键的判断就是很好的例子,在最后的爆破我们可以什么工作都不用就直接获取胜利,如果这个判断是判断密码呢?大家可以参考JPEG LOCKER 密码判断的爆破,此时我们随意输入任意密码就可以实现对图片的解密:



![](pic_temp5\psb16.jpg)