##  起源



1. 无壳和无自检程序的破解



  假设我们要破解一个注册码程序,最便捷就是利用爆破的方式来使得程序被永久性地破解,下面是一段实例代码:



```c++
#include <string.h>
#include <stdio.h>

bool check_input_code(const char* input_string) {
    if (!strcmp("TEST",input_string))
        return true;

    return false;
}

bool regedit_error(void) {
    printf("ERR");
    return true;
}

void regedit_success(void) {
    printf("OK");
}

int main(void) {
    printf("INPUT LINCE CODE:");

    char buffer[10]={0};

    scanf("%s",buffer);

    if (check_input_code(buffer))
        regedit_success();
    else
        regedit_error();
    return 0;
}
```



  启动OD ,查看main() 的代码:



```assembly
00401130 >/> \55            push ebp
00401131  |.  8BEC          mov ebp,esp
00401133  |.  83EC 4C       sub esp,0x4C
00401136  |.  53            push ebx
00401137  |.  56            push esi
00401138  |.  57            push edi
00401139  |.  8D7D B4       lea edi,[local.19]
0040113C  |.  B9 13000000   mov ecx,0x13
00401141  |.  B8 CCCCCCCC   mov eax,0xCCCCCCCC
00401146  |.  F3:AB         rep stos dword ptr es:[edi]
00401148  |.  68 30504200   push crackme_.00425030                   ; /format = "INPUT LINCE CODE:"
0040114D  |.  E8 4E010000   call crackme_.printf                     ; \printf  提示输入
00401152  |.  83C4 04       add esp,0x4
00401155  |.  C645 F4 00    mov byte ptr ss:[ebp-0xC],0x0
00401159  |.  33C0          xor eax,eax
0040115B  |.  8945 F5       mov dword ptr ss:[ebp-0xB],eax
0040115E  |.  8945 F9       mov dword ptr ss:[ebp-0x7],eax
00401161  |.  8845 FD       mov byte ptr ss:[ebp-0x3],al
00401164  |.  8D4D F4       lea ecx,[local.3]
00401167  |.  51            push ecx
00401168  |.  68 2C504200   push crackme_.0042502C                   ; /format = "%s"
0040116D  |.  E8 AE010000   call crackme_.scanf                      ; \scanf  接收输入数据 
00401172  |.  83C4 08       add esp,0x8
00401175  |.  8D55 F4       lea edx,[local.3]
00401178  |.  52            push edx
00401179  |.  E8 8CFEFFFF   call crackme_.0040100A  ;  调用
```



check_input_code 判断验证码是否正确



```assembly
0040117E  |.  83C4 04       add esp,0x4
00401181  |.  25 FF000000   and eax,0xFF
00401186  |.  85C0          test eax,eax  ;  if 判断
```



check_input_code 返回的bool 值



```assembly
00401188  |.  74 07         je Xcrackme_.00401191  ;  如果为0
0040118A  |.  E8 80FEFFFF   call crackme_.0040100F
0040118F  |.  EB 05         jmp Xcrackme_.00401196
00401191  |>  E8 83FEFFFF   call crackme_.00401019
00401196  |>  33C0          xor eax,eax
00401198  |.  5F            pop edi
00401199  |.  5E            pop esi
0040119A  |.  5B            pop ebx
0040119B  |.  83C4 4C       add esp,0x4C
0040119E  |.  3BEC          cmp ebp,esp
004011A0  |.  E8 BB000000   call crackme_._chkesp
004011A5  |.  8BE5          mov esp,ebp
004011A7  |.  5D            pop ebp
004011A8  \.  C3            retn
```



  我们知道,只要把代码:



```assembly
00401188  |.  74 07         je Xcrackme_.00401191
```



  改为nop ,我们就可以输入任意数据通过注册验证



2. 有壳和自检程序的破解



  如果程序带有壳的话,那么得到的汇编代码就不那么容易给破解,以上面的代码为例,利用UPX 加壳,得到的汇编代码为:



```assembly
00401130    0000            add byte ptr ds:[eax],al
00401132    0000            add byte ptr ds:[eax],al
00401134    0000            add byte ptr ds:[eax],al
00401136    0000            add byte ptr ds:[eax],al
00401138    0000            add byte ptr ds:[eax],al
0040113A    0000            add byte ptr ds:[eax],al
0040113C    0000            add byte ptr ds:[eax],al
0040113E    0000            add byte ptr ds:[eax],al
00401140    0000            add byte ptr ds:[eax],al
00401142    0000            add byte ptr ds:[eax],al
00401144    0000            add byte ptr ds:[eax],al
00401146    0000            add byte ptr ds:[eax],al
00401148    0000            add byte ptr ds:[eax],al
0040114A    0000            add byte ptr ds:[eax],al
0040114C    0000            add byte ptr ds:[eax],al
0040114E    0000            add byte ptr ds:[eax],al
00401150    0000            add byte ptr ds:[eax],al
00401152    0000            add byte ptr ds:[eax],al
00401154    0000            add byte ptr ds:[eax],al
00401156    0000            add byte ptr ds:[eax],al
00401158    0000            add byte ptr ds:[eax],al
0040115A    0000            add byte ptr ds:[eax],al
0040115C    0000            add byte ptr ds:[eax],al
0040115E    0000            add byte ptr ds:[eax],al
00401160    0000            add byte ptr ds:[eax],al
00401162    0000            add byte ptr ds:[eax],al
00401164    0000            add byte ptr ds:[eax],al
00401166    0000            add byte ptr ds:[eax],al
00401168    0000            add byte ptr ds:[eax],al
0040116A    0000            add byte ptr ds:[eax],al
0040116C    0000            add byte ptr ds:[eax],al
0040116E    0000            add byte ptr ds:[eax],al
00401170    0000            add byte ptr ds:[eax],al
00401172    0000            add byte ptr ds:[eax],al
00401174    0000            add byte ptr ds:[eax],al
00401176    0000            add byte ptr ds:[eax],al
00401178    0000            add byte ptr ds:[eax],al
0040117A    0000            add byte ptr ds:[eax],al
0040117C    0000            add byte ptr ds:[eax],al
0040117E    0000            add byte ptr ds:[eax],al
00401180    0000            add byte ptr ds:[eax],al
00401182    0000            add byte ptr ds:[eax],al
00401184    0000            add byte ptr ds:[eax],al
00401186    0000            add byte ptr ds:[eax],al
00401188    0000            add byte ptr ds:[eax],al
0040118A    0000            add byte ptr ds:[eax],al
0040118C    0000            add byte ptr ds:[eax],al
0040118E    0000            add byte ptr ds:[eax],al
00401190    0000            add byte ptr ds:[eax],al
00401192    0000            add byte ptr ds:[eax],al
00401194    0000            add byte ptr ds:[eax],al
00401196    0000            add byte ptr ds:[eax],al
00401198    0000            add byte ptr ds:[eax],al
0040119A    0000            add byte ptr ds:[eax],al
0040119C    0000            add byte ptr ds:[eax],al
0040119E    0000            add byte ptr ds:[eax],al
004011A0    0000            add byte ptr ds:[eax],al
004011A2    0000            add byte ptr ds:[eax],al
004011A4    0000            add byte ptr ds:[eax],al
004011A6    0000            add byte ptr ds:[eax],al
004011A8    0000            add byte ptr ds:[eax],al
```



  这就意味要着代码保存在另外的地方,这里将是代码以后要加载到的地方,对于这种情况,用爆破的方式去破解程序是没有用的,只能通过等待程序加载完成代码之后才可以向0x401188 写NOP 指令



  带有自检的程序破解起来也是很恶心的,如果把main 改写成下面的代码:



```c++

bool program_check(unsigned long process_address,unsigned long code_length,long magic_number) {
    long num=0;

    for (unsigned int index=0;index<code_length;++index)
        num+=*(char*)(process_address+index);
 
    if (num==magic_number)
        return true;

    return false;
}

const long magic_number_main=123;  //  计算出的校验数为123

int main(void) {
    if (!program_check((*(unsigned long*)((unsigned long)main+1))+(unsigned long)&main+5+0x87,2,magic_number_main))
        return -1;

    printf("INPUT LINCE CODE:");

    char buffer[10]={0};

    scanf("%s",buffer);

    if (check_input_code(buffer))
        regedit_success();
    else
        regedit_error();

    return 0;
}

```



  在没有爆破关键判断之前,程序是可以正常执行的

 

![图片](pic_temp2\psb1.png)


  其实自检的原理很简单,把关键判断的代码用计算的方式来得到校验数,然后每次执行到程序的入口点时都要检查.

  至于`(*(unsigned long*)((unsigned long)main+1))+(unsigned long)&main+5+0x87` ,它的意思就是读取main 函数向下偏移0x87 位
 

##  原理



1. 断点



  对于有壳和自检的程序,是不是就没有办法去突破呢?这里我们要介绍内存注册机,任何代码,都是从磁盘上的文件被系统加载到内存之中,然后再执行,既然代码是要被执行的,那么有没有办法使它在某个地方停止下来呢?答案就是利用调试器的断点功能.同样,我们把断点的功能运用在内存注册机中会有意想不到的效果.把程序执行的代码停留在某个关键的位置,然后再实现数据或者代码篡改的功能,以绕过程序原来的保护.



2. 入口点



  任何程序和函数都有入口点,在实现脱壳之前,代码可能是被加密或者是保存在另一地方,如果我们现在往关键位置上写数据,那么壳会把我们写的数据给替换掉或者解析,所以,在入口点设置断点然后修改数据和代码就可以绕过壳的保护,程序自检也可以利用同样的道理来绕过.



##  实现



1. Debug API



  我们接下来要使用Debug API 有:



```c++

BOOL CreateProcess (  //  启动被破解进程
    LPCTSTR lpApplicationName,
    LPTSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes。
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL bInheritHandles,
    DWORD dwCreationFlags,
    LPVOID lpEnvironment,
    LPCTSTR lpCurrentDirectory,
    LPSTARTUPINFO lpStartupInfo,
    LPPROCESS_INFORMATIONlpProcessInformation
);

HANDLE OpenProcess(  //  获取进程所有权
    DWORD dwDesiredAccess,
    BOOL bInheritHandle,
    DWORD dwProcessId
);

BOOL WaitForDebugEvent(  //  等待调试事件
    LPDEBUG_EVENT lpDebugEvent, 
    DWORD dwMilliseconds 
);

BOOL ContinueDebugEvent(  //  继续调试
    DWORD dwProcessId,
    DWORD dwThreadId,
    DWORD dwContinueStatus
);

BOOL GetThreadContext(HANDLE hThread,  //  获取线程的寄存器
                      LPCONTEXT lpContext
);

BOOL SetThreadContext(HANDLE hThread,  //  设置线程的寄存器
                      LPCONTEXT lpContext
);

BOOL WriteProcessMemory(  //  写内存
    HANDLE hProcess,
    LPVOID lpBaseAddress,
    LPVOID lpBuffer,
    DWORD nSize,
    LPDWORD lpNumberOfBytesWritten
);

BOOL ReadProcessMemory(  //  读内存
    HANDLE hProcess,
    LPVOID lpBaseAddress,
    LPVOID lpBuffer,
    DWORD nSize,
    LPDWORD lpNumberOfBytesWritten
);

typedef struct _DEBUG_EVENT {  //  调试事件结构
    DWORD dwDebugEventCode;
    DWORD dwProcessId;
    DWORD dwThreadId;

    union {
       EXCEPTION_DEBUG_INFO      Exception;
       CREATE_THREAD_DEBUG_INFO  CreateThread;
       CREATE_PROCESS_DEBUG_INFO CreateProcessInfo;
       EXIT_THREAD_DEBUG_INFO    ExitThread;
       EXIT_PROCESS_DEBUG_INFO   ExitProcess;
       LOAD_DLL_DEBUG_INFO       LoadDll;
       UNLOAD_DLL_DEBUG_INFO     UnloadDll;
       OUTPUT_DEBUG_STRING_INFO  DebugString;
       RIP_INFO                  RipInfo;
    } u;

} DEBUG_EVENT, *LPDEBUG_EVENT;

```



2. CrackMe 程序



  CrackMe 分两个版本:带壳和自检,下载地址:http://vdisk.weibo.com/s/B-JCvBWBzp-dc



3. 内存注册机--针对带壳



  首先查找程序的真实入口点和要爆破的位置:

 

![图片](pic_temp2\psb2.png)



OD 指向的入口点



![图片](pic_temp2\psb3.png)



由于是UPX 加壳,直接往下找就可以知道真实的OEP (程序入口点) 



![图片](pic_temp2\psb4.png)



程序还没有被加载到OEP 



![图片](pic_temp2\psb5.png)



UPX 壳把程序解压OEP



![图片](pic_temp2\psb6.png)



需要爆破的地方



![图片](pic_temp2\psb7.png)





  有了这些依据,我们就可以编写代码:



```c++
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <windows.h>
#include <winnt.h>

#define BREAK_ADDRESS_SHELL_JMP 0x4307CC
#define BREAK_ADDRESS_JE        0x401188
#define BREAK_FLAG 0xCC

const char* set_break(HANDLE process,LPVOID set_address,unsigned int set_length=1) {  //  设置断点
    if (!set_length) return NULL;

    char* break_flag_buffer=(char*)malloc(set_length);
    memset(break_flag_buffer,BREAK_FLAG,set_length);
    DWORD write_length=0;

    char* old_code_buffer=(char*)malloc(set_length);
    DWORD read_length=0;
    ReadProcessMemory(process,set_address,(LPVOID)old_code_buffer,set_length,&read_length);  //  原理是通过写0xCC 来实现软中断

    WriteProcessMemory(process,set_address,(LPVOID)break_flag_buffer,set_length,&write_length);
    free(break_flag_buffer);

    return old_code_buffer;
}

bool remove_break(HANDLE process,LPVOID remove_address,const char* rewrite_code_buffer,unsigned int remove_length) {  //  删除断点
    DWORD write_length=0;

    return (bool)WriteProcessMemory(process,remove_address,(LPVOID)rewrite_code_buffer,remove_length,&write_length);
}

HANDLE open_process(DWORD processid) {
    return OpenProcess(PROCESS_ALL_ACCESS,FALSE,processid);
}

int main(int argc,char** argv) {
    STARTUPINFO process_startupinfo={0};
    PROCESS_INFORMATION process_info={0};
    CreateProcess(NULL,"crackme_shell.exe",NULL,NULL,FALSE,DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE,NULL,NULL,&process_startupinfo,&process_info);  //  启动调试程序
    printf("CreateProcessId=%X\n",process_info.dwProcessId);

    HANDLE process=open_process(process_info.dwProcessId);

    set_break(process,(void*)BREAK_ADDRESS_SHELL_JMP,1);  //  在UPX 解压结束的长跳转中设置中断

    DEBUG_EVENT debug_event={0};
    debug_event.dwProcessId=process_info.dwProcessId;

    while (WaitForDebugEvent(&debug_event,INFINITE)) {  //  等待系统发来中断事件
        switch (debug_event.dwDebugEventCode) {
            case EXCEPTION_DEBUG_EVENT: {
                switch (debug_event.u.Exception.ExceptionRecord.ExceptionCode) {
                    case EXCEPTION_BREAKPOINT: {
                        CONTEXT regesit={0};
                        regesit.ContextFlags=CONTEXT_FULL;
                        GetThreadContext(process_info.hThread,&regesit);  //  主要是读取EIP
                        printf("EIP=%X\n",regesit.Eip);

                        switch (--regesit.Eip) {
                            case BREAK_ADDRESS_SHELL_JMP: {
                                SetThreadContext(process_info.hThread,&regesit);
                                char jmp_code[1]={0xE9};
                                remove_break(process,(void*)BREAK_ADDRESS_SHELL_JMP,jmp_code,sizeof(jmp_code));  //  消除断点替换0xE9 (JMP 的指令码)

                                char nop_code[2]={0x90,0x90};
                                remove_break(process,(void*)BREAK_ADDRESS_JE,nop_code,sizeof(nop_code));  //  往JE 写两个NOP

                            }
                        }
                    }
                }
            }
        }
        if (EXIT_PROCESS_DEBUG_EVENT==debug_event.dwDebugEventCode)  //  程序退出
            break;
        ContinueDebugEvent(process_info.dwProcessId,process_info.dwThreadId,DBG_CONTINUE);

    }
    printf("OK!\n");
    
    return 0;
}
```



  程序运行实例:

 

![图片](pic_temp2\psb8.png)



crackme_shell 程序被破解



![图片](pic_temp2\psb9.png)



4. 内存注册机--针对自检



  先来分析程序是如何进行自检:

 

![图片](pic_temp2\psb10.png)



```assembly
00410FC8  |.  6A 7B         push 0x7B
00410FCA  |.  6A 02         push 0x2
00410FCC  |.  B8 32104000   mov eax,crackme_.00401032                ;  入口地址
00410FD1  |.  8B48 01       mov ecx,dword ptr ds:[eax+0x1]
00410FD4  |.  81C1 32104000 add ecx,crackme_.00401032                ;  入口地址
00410FDA  |.  81C1 8C000000 add ecx,0x8C
00410FE0  |.  51            push ecx  ;  传递数据 [0x401032+0x1]+0x401032+0x8C ,计算出来的地址为0x411037
00410FE1  |.  E8 5600FFFF   call crackme_.0040103C  ;  调用自检函数
00410FE6  |.  83C4 0C       add esp,0xC  ;  程序带有三个参数
00410FE9  |.  25 FF000000   and eax,0xFF
00410FEE  |.  85C0          test eax,eax  ;  根据bool 返回值判断
00410FF0  |.  75 05         jnz Xcrackme_.00410FF7
00410FF2  |.  83C8 FF       or eax,0xFFFFFFFF
00410FF5  |.  EB 50         jmp Xcrackme_.00411047  ;  退出程序
```



  每次判断结束,程序必然会执行这句汇编:



```assembly
00410FF0  |.  75 05         jnz Xcrackme_.00410FF7 
```



  在这儿设置中断



```assembly
00410FF0  |.  CC 05         jnz Xcrackme_.00410FF7 
```



  然后就可以修改JE 代码,程序也不会退出,下面是crack 代码:





```c++
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <windows.h>
#include <winnt.h>

#define BREAK_ADDRESS_CHECK_JMP 0x410FF0
#define BREAK_ADDRESS_JE        0x411037
#define BREAK_FLAG 0xCC

const char* set_break(HANDLE process,LPVOID set_address,unsigned int set_length=1) {
    if (!set_length) return NULL;

    char* break_flag_buffer=(char*)malloc(set_length);
    memset(break_flag_buffer,BREAK_FLAG,set_length);
    DWORD write_length=0;
    char* old_code_buffer=(char*)malloc(set_length);
    DWORD read_length=0;
    
    ReadProcessMemory(process,set_address,(LPVOID)old_code_buffer,set_length,&read_length);
    WriteProcessMemory(process,set_address,(LPVOID)break_flag_buffer,set_length,&write_length);

    free(break_flag_buffer);

    return old_code_buffer;
}

bool remove_break(HANDLE process,LPVOID remove_address,const char* rewrite_code_buffer,unsigned int remove_length) {
    DWORD write_length=0;

    return (bool)WriteProcessMemory(process,remove_address,(LPVOID)rewrite_code_buffer,remove_length,&write_length);
}

HANDLE open_process(DWORD processid) {
    return OpenProcess(PROCESS_ALL_ACCESS,FALSE,processid);
}

int main(int argc,char** argv) {
    STARTUPINFO process_startupinfo={0};
    PROCESS_INFORMATION process_info={0};
    
    CreateProcess(NULL,"crackme_check.exe",NULL,NULL,FALSE,DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE,NULL,NULL,&process_startupinfo,&process_info);

    printf("CreateProcessId=%X\n",process_info.dwProcessId);

    HANDLE process=open_process(process_info.dwProcessId);

    set_break(process,(void*)BREAK_ADDRESS_CHECK_JMP,1);

    DEBUG_EVENT debug_event={0};
    debug_event.dwProcessId=process_info.dwProcessId;

    while (WaitForDebugEvent(&debug_event,INFINITE)) {
        switch (debug_event.dwDebugEventCode) {
            case EXCEPTION_DEBUG_EVENT: {
                switch (debug_event.u.Exception.ExceptionRecord.ExceptionCode) {
                    case EXCEPTION_BREAKPOINT: {
                        CONTEXT regesit={0};
                        regesit.ContextFlags=CONTEXT_FULL;
                        GetThreadContext(process_info.hThread,&regesit);
                        printf("EIP=%X\n",regesit.Eip);

                        switch (--regesit.Eip) {
                            case BREAK_ADDRESS_CHECK_JMP: {
                                SetThreadContext(process_info.hThread,&regesit);
                                char jmp_code[1]={0x75};  //  注意jnz 的机器码为0x75
                                remove_break(process,(void*)BREAK_ADDRESS_CHECK_JMP,jmp_code,sizeof(jmp_code));
                                char nop_code[2]={0x90,0x90};
                                remove_break(process,(void*)BREAK_ADDRESS_JE,nop_code,sizeof(nop_code));
                            }
                        }
                    }
                }
            }
        }

        if (EXIT_PROCESS_DEBUG_EVENT==debug_event.dwDebugEventCode)
            break;

       ContinueDebugEvent(process_info.dwProcessId,process_info.dwThreadId,DBG_CONTINUE);
    }

   printf("OK!\n");

   return 0;
}
```



  程序运行效果:

 

![图片](pic_temp2\psb11.png)



![图片](pic_temp2\psb12.png)



 

5. 如果都结合在一起呢?



  如果把壳和自检结合在一起,其实就是多设置一个断点的事情而已,思路如下:在UPX 壳跳转的地方设置断点到程序自检的判断地址,然后在自检的地方引发中断然后把关键判断爆破即可完成任务,程序和完整代码都有给出来,大家自行完成吧. 



##  后话



  本来想自己写壳来做实例的,但是默认情况下代码段是不能被写的,所以只完成了一大半的工作,只要在unpack 里面添加对VirtualProcect 的调用大概也就可以写数据了吧,这个壳是简单的加密壳,还是上代码吧:



```c++
#include <memory.h>
#include <stdio.h>

#include <windows.h>

#define UNPACK_CODE_OFFSET 0x400

#define BUFFER_LENGTH 0x24000

char shell_unpack_code[]={0x60,0xB2,0xFF,0xBB,0xFF,0xFF,0xFF,0xFF,0xB9,0xFF,0xFF,0xFF,0xFF,0x8B,0xC3,0x03,0xC1,0x48,0x49,0x53,0x8A,0x18,0x32,0xDA,0x88,0x18,0x5B,0x85,0xC9,0x75,0xEE,0x61,0xE9,0xFF,0xFF,0xFF,0xFF};  //  壳的汇编代码,其中0xFF 就是以后需要填数据的地方

void unpack() {  //  unsigned long address,unsigned int _length,unsigned char magic_number) {
    __asm {
        pushad;  //  保存现场

        mov dl,0xFF;         //  XOR 种子
        mov ebx,0xFFFFFFFF;  //  起始地址
        mov ecx,0xFFFFFFFF;  //  代码长度

_LOOP:  //  循环实现xor 运算,代码意义和下面的的pack 函数一样(XOR 意义上是相同的运算)
        mov eax,ebx;
        add eax,ecx;
        dec eax;
        dec ecx;
        push ebx;
        mov bl,byte ptr [eax];
        xor bl,dl;
        mov byte ptr [eax],bl;
        pop ebx;
        test ecx,ecx;
        jnz _LOOP;

        popad;
        _emit 0xE9;  //  直接写jmp 命令有问题
        _emit 0xFF;  //  起始地址
        _emit 0xFF;
        _emit 0xFF;
        _emit 0xFF;
    }
}

void pack(char* buffer,unsigned long buffer_length,unsigned char magic_number) {
    while (buffer_length)
        buffer[--buffer_length]^=magic_number;
}

void main(int argc,char** argv) {
    HANDLE pack_file=CreateFile(argv[1],GENERIC_WRITE | GENERIC_READ,NULL,NULL,OPEN_EXISTING,NULL,NULL);

    if (INVALID_HANDLE_VALUE!=pack_file) {
        printf("open file ok!\n");

        char buffer[BUFFER_LENGTH]={0};
        DWORD read_length=0;
        DWORD file_point=0;
        ReadFile(pack_file,buffer,sizeof(IMAGE_DOS_HEADER),&read_length,NULL);  //  先读DOS 头
        IMAGE_DOS_HEADER hander_dos={0};
        memcpy(&hander_dos,buffer,sizeof(hander_dos));
        file_point=hander_dos.e_lfanew;
        SetFilePointer(pack_file,file_point,NULL,FILE_BEGIN);

        ReadFile(pack_file,buffer,sizeof(IMAGE_NT_HEADERS),&read_length,NULL);  //  再读NT 头
        IMAGE_NT_HEADERS hander_nt={0};
        memcpy(&hander_nt,buffer,sizeof(IMAGE_NT_HEADERS));
        file_point=hander_nt.OptionalHeader.AddressOfEntryPoint;  //  程序OEP (注意是在文件中的相对地址)
        unsigned long oep=hander_nt.OptionalHeader.ImageBase+hander_nt.OptionalHeader.AddressOfEntryPoint;  //  这个才是在内存中的真实OEP
        printf("OEP=%X\n",oep);
        SetFilePointer(pack_file,file_point,NULL,FILE_BEGIN);
        unsigned long length=hander_nt.OptionalHeader.SizeOfCode-hander_nt.OptionalHeader.AddressOfEntryPoint;  //  要加密的数据长度
        //  注意

SizeOfCode 刚好是.text 段的大小
        ReadFile(pack_file,buffer,length,&read_length,NULL);  //  读取代码
        if (read_length<=BUFFER_LENGTH) {  //  在这里长度是有限制的,简单点写嘛,可以用malloc 来不受栈限制
            unsigned char magic_number=0x55;
            pack(buffer,hander_nt.OptionalHeader.SizeOfCode,magic_number);
            memcpy(&shell_unpack_code[2],&magic_number,sizeof(magic_number));  //  填充unpack 代码
            memcpy(&shell_unpack_code[4],&oep,4);
            memcpy(&shell_unpack_code[9],&length,4);
            oep=hander_nt.OptionalHeader.AddressOfEntryPoint-UNPACK_CODE_OFFSET;  //  计算jmp 的跳转偏移
            memcpy(&shell_unpack_code[33],&oep,4);

            SetFilePointer(pack_file,UNPACK_CODE_OFFSET,NULL,FILE_BEGIN);  //  在0x400 处写unpack 代码
            WriteFile(pack_file,shell_unpack_code,sizeof(shell_unpack_code),&read_length,NULL);
            oep=hander_nt.OptionalHeader.AddressOfEntryPoint;
            SetFilePointer(pack_file,oep,NULL,FILE_BEGIN);  //  写入加密代码
            WriteFile(pack_file,buffer,length,&read_length,NULL);
            SetFilePointer(pack_file,hander_dos.e_lfanew,NULL,FILE_BEGIN);  //  改写NT 头,设置新OEP 到unpack
            hander_nt.OptionalHeader.AddressOfEntryPoint=UNPACK_CODE_OFFSET;
            WriteFile(pack_file,&hander_nt,sizeof(hander_nt),&read_length,NULL);

            CloseHandle(pack_file);
            printf("new oep=%X\n",hander_nt.OptionalHeader.ImageBase+hander_nt.OptionalHeader.AddressOfEntryPoint);
            printf("pack OK!\n");
        } else {
            printf("buffer length small!\n");
        }
    } else
        printf("open file err!");
}
```



