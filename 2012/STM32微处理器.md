关于Cortex内核(以下选摘自百度百科-Cortex:<http://baike.baidu.com/view/1745516.htm>):



Cortex™-A 系列 - 开放式操作系统的高性能处理器

Cortex™-A 系列广泛运行于手机,PDA,掌上电脑等需要高频率时钟处理的产品

Cortex™-R 系列 - 面向实时应用的卓越性能 

Cortex™-M 系列 - 面向具有确定性的微控制器应用的成本敏感型解决方案  [比如山寨手机..]



STM32系列处理器使用ARM Cortex-M3内核.Cortex-M3代表的是ARM9架构



![图片](pic_temp4\psb1.jpg) 



不同型号的STM32处理器引脚数和性能也有所不同,下面是STM32F10x系列:



![图片](pic_temp4\psb2.jpg)



上图横轴代表芯片的引脚类型,纵轴代表芯片的内部代码FLASH(即ROM)大小



由于每一种的STM32F10x系列处理器都会有不同的附加性质,下面就给出来让大家了解一下

 

![图片](pic_temp4\psb3.jpg)



下面用STM32F107VCT6来和AT89C51-24PU来作一个对比(其实根本是不能相比的,一个是微处理器[72MHZ],一个是单片机[24MHZ],只不过是为了让大家更能理解一些实质性的东西)



外观上的差异是最容易理解的,STM32F107VCT6采用的是LQFP-100Pin 封装,AT89C51采用DIP-40Pin封装(Pin是指引脚数,DIP也就是电子元件里面的直插元件,贴片元件也有很多种,其中LQFP封装就是一个例子) 



![图片](pic_temp4\psb4.jpg)



LQFP-100Pin,封装引脚图



![图片](pic_temp4\psb5.jpg)



STM32F107VCT6 实物图片



![图片](pic_temp4\psb6.jpg)

 

AT89C51和三种同类型DIP封装单片机[C52内部ROM为8K,S51可以ISP直接调试代码]

 

从代码储存来比较,STM32F107VCT6有512K字节的内部ROM,而AT89C51却只有4K,89C52只有8K,但是可以通过外部PROM/EEPROM来读取



代码,AT89C51可以寻址0x0000-0xFFFF,也就是16K拓展ROM.STM32F107VCT6也可以实现外部拓展代码储存,用NAND FLASH



从随机储存器比较,STM32F107VCT6有64K RAM,AT89C51有512B RAM



从指令集来比较,89Cxx类使用MCU-51指令集,STM32使用ARM指令集,ARM7系列和ARM9系列也各使用不同的指令集



控制STM32的通用IO端口的代码和AT89C51大有不同,对于STM32的各个端口还需要先配置,后使用,AT89C51就只需要一句



**sbit CtrlPort=Px^x;**  或者 **Px=0x??;**  [Px^x代表Px端口的第x位IO引脚,0x??代表一个两位的十六进制的数]



就可以使用这个IO引脚,对一个IO引脚输出高电平和输出低电平只需简单的



**CtrlPort=1**  输出高电平  **CtrlPort=0**  输出低电平



STM32对于一个将要输出高电平再低电平的端口需要先对它进行初始化,以STM32的PA3引脚为例



```c
void PA3_config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;   //这个是控制GPIO引脚的结构

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO , ENABLE);  //对PA的端口进行AFIO的时钟处理

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  //第3个引脚
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //输出50MHz
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //开漏输出
  GPIO_Init(GPIOA, &GPIO_InitStructure); //对PA端口的Pin3引脚进行初始化
}
```



这个就是初始化,后面让IO端口进行高低电平输出



```c
void PA3_Turn_Low(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_3);  //设置PA3为低电平
}


void PA3_Turn_Hige(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_3);   //设置PA3为高电平
}
```



用一个STM32的引脚挺麻烦的,不过处理器主频也弥补函数过多带来的增多额外时钟处理的一些缺陷