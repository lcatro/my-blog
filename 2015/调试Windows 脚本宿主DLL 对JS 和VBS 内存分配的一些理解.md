##  搭建调试环境



1. IE 的架构



  在向大家讲述如何搭建调试环境之前,有必要告诉大家IE 浏览器的一些相关架构信息

 

![图片](pic_temp8\psb1.png)

 

  架构大体分为四部分:调试器,插件管理,渲染器和脚本模块(网络相关模块就省略啦),不同的管理器布置在不同的DLL 中,比如渲染器的DLL 是pdm.dll 和sqmapi.dll ,脚本宿主管理器的DLL 是vbscript.dll 和jscript.dll ,本文着重于脚本原理方向,所以只关注整个体系架构的两个宿主DLL 文件



2. 怎么调试



  IE 进程比较特殊,当我们启用浏览器时,每次创建新的Tab 页面,系统中都会对应产生新的IE 页面进程,在首次启动IE 浏览器时,它会在系统内创建两个进程

 

![图片](pic_temp8\psb2.png)



  启动OD ,选择"文件"->"附加" 菜单,可以发现有两个不同的进程,页面进程是0xBC4 ,主进程是0xD60 ,因为调试的脚本代码和页面有关,各个页面又和进程有关,于是选择附加到指定的页面进程0xD60 (显示Title 的是主进程哦).



![图片](pic_temp8\psb3.png)

 

  IE 在加载没有脚本的页面时,内部不会连带加载脚本DLL ,只有当页面中带有特定的脚本语言时,IE 才会根据这些不同的脚本语言去加载不同的脚本宿主DLL .



  加载带有JavaScript 的页面之前:

 

![图片](pic_temp8\psb4.png)

 

  加载带有JavaScript 的页面之后:

 

![图片](pic_temp8\psb5.png)



3. IDA 反编译



  建议大家调试32 位下的Jscript 和VBscript DLL ,64 位的DLL 太难操作和调试,两个DLL 的路径位于:C:\WINDOWS\system32 下



![图片](pic_temp8\psb6.png) ![图片](pic_temp8\psb7.png)

 

  拖放到32 位的IDA 下,你会惊奇的发现原来DLL 还带有调试段信息,于是基本上所有的类信息都可以一目了然啦..

 

![图片](pic_temp8\psb8.png)



  如果需要自己手工调试宿主环境的堆信息,可以关注下面这几个注意点:



VBScript:



获取变量的值:VAR::PVarGetVarVal [0x73273998]

设置变量的值:AssignVar [0x73271A41]



JScript:



获取变量的值:NameTbl::GetValCore [0x3E3725A5]

设置变量的值:NameTbl::GetValCore [0x3E36A1A6]
 

## 脚本宿主环境变量分配原理



  无论是JavaScript 还是VBScript ,它们在脚本宿主环境中都保存在堆中,宿主环境使用一张名称表把各个变量名和对应的变量信息都联系起来

  

![图片](pic_temp8\psb9.png)

 

  毕竟是脚本语言,无论是JavaScript 还是VBScript ,他们都是弱类型的变量,于是宿主环境会在内存中用标记来区分变量,数组和函数,下面是VBScript 中一个普通变量的值为1234 在堆内存中的表达:

 

![图片](pic_temp8\psb10.png)



  变量的结构如下:



```c++
Struct var {
    VarType type;
    Short is_zero1;
    Short is_zero2;
    Short is_zero3;
    Unsigned long value;
};
```



  如果是普通的数值和布尔值变量,value 则直接储存变量的数据;如果变量是数组或者字符串的话,那么value 则是一个指向他们的指针



  在VBScript 中创建新的变量时,宿主环境会利用到下面的类函数在全局的名字列表中创建:

 

![图片](pic_temp8\psb11.png)


  获取变量和设置变量一样,通过对全局名字列表的遍历查找搜索出目的变量信息(提示:其实变量在宿主环境中是以类的形式去表现的,只不过我们在观测堆内存信息的时候直接看到变量类中的数据成员而已) 

![图片](pic_temp8\psb12.png) ![图片](pic_temp8\psb13.png)



  VBScript 的堆内存其实没有多大的讨论性,不过有一点非常值得告诉大家,就是著名的IE 远程代码执行漏洞(CVE-2014-6332) ,漏洞原理简单向大家介绍下:



  在IE 中执行调用本地Active X 的代码时,IE 往往会弹出用户允许执行Active X 对话框,但是在脚本宿主环境中有一个非常奇特的标志位,如果我们可以把这个标志位设置为0 ,那么接下来要执行的脚本代码将不会提示用户运行执行Active X 对话框而直接执行,在VBScript.DLL 中,该标志位在COleScript + 0x174 ,在JavaScript 中,该标志位在COleScript + 0x184 .

 

![图片](pic_temp8\psb14.png)

 

  那么应该以怎么样的方式去把这个特殊的标志位给清0 呢?VBScript 的Redim 提供了一个很精妙的思路:



  "如果你的VBScript 数组太小了,但是你又很想把数据追加到它的末尾,最好不要通过Redim 去修改这个数组",这是VB 开发中最常听到的声音,因为Redim 关键字会在原来的数组基础上强制往后移动,无论后面是否存在正在使用的数据,这个用Redim 修饰的变量都可以跨上标去访问.于是通过这种方法一直在堆内搜寻VBScript 的COleScript 类的特征信息,最后再定位到Safemode 标志位然后写0 即可绕过检测.有兴趣的话可以访问(http://xteam.baidu.com/?p=104)



##  JavaScript -- 任何东西都是对象



  对于单个变量的储存,JavaScript 和VBScript 都是相同的

 

![图片](pic_temp8\psb15.png)

 

  问题是假如JavaScript 也要像VBScript 一样通过数组上标溢出去越界访问其它内存可不可以呢?其实是不可以的,为什么?因为在JavaScript 中,任何东西都是对象.下面是关于MainWindow 的例子:

 

![图片](pic_temp8\psb16.png)



  关于JavaScript 变量



```javascript
  var var1=1234;
```



  大家可能会想到,脚本环境在解析到这段代码时会直接把1234 从文本转换为数字再保存到var1 中,其实在真正的执行环境中,1234 是先保存到脚本环境下一个专门保存右边需要赋值到左边的变量,然后再通过这个变量赋值到var1 变量中去,这个赋值变量对编写JS 代码的程序员来说是隐藏的,但是它却真实存在在脚本环境中

 

![图片](pic_temp8\psb17.png)

 

  好了,在证实JavaScript 中任何东西都是对象的事实之后(就算undefine 也是对象),是时候要探讨JavaScript 的Array 特性啦




  下面是一段JS 数组测试代码:



```javascript
var test_array=new Array(5);

test_array[0]=5;
test_array[1]=4;
test_array[2]=3;
test_array[3]=2;
test_array[4]=1;
test_array[6]=66;

alert(test_array[100]);
```



  Array 将会在内存中创建一个ArrayObj 的实例,注意,此时ArrayObj 对象只是知道了他初始时定义的长度,但是ArrayObj 是空的对象,里面什么都没有,直到我们执行代码test_array[0]=5; ,此时JScript 会在堆中创建一个var 变量对象,然后在Array 中登记索引为0 的项目指向到新创建的var 变量对象,最后才保存数值5 到test_array[0] 变量对象中.可能你会感到困惑,但是你可以换个角度去看Array 和变量之间的关系,就像是C++ 中的泛型一样,Array 只是一个像Vector 一样的容器,容器本身只是存储索引,没有存储真实数据,但是我们可以根据容器中的索引去寻找真实的数据,这个真实的数据就是Array 中索引数组的实体.



![图片](pic_temp8\psb18.png)


  在堆中的表示如下: 



![](pic_temp8\psb19.webp.jpg)



  现在请大家关于OverFlow PoC 中指向的堆数据中,因为原来的Array 指定的大小是5 个数组变量,起始索引为0 (不知道为什么在内存中显示是1 ).于是尝试越界去写数据(Array 的上标为4 ,我们往Array[6] 中写数据),可以看到Array[5] 中的堆没有任何数据.但是可以成功往Array[6] 中写数据到堆,那么我们来一个大胆的推论,可不可以通过近似与无限的越界写0 循环把SafeMode 清0 呢?推理如下:



![](pic_temp8\psb20.webp.jpg)



  结果Array 的变量对象+0xC 会覆盖掉SafeMode 标志位,恰好ArrayVarObj + 0xC 是指向堆的值,所以无法往这里填充0 ,但是我就不信了,特么就是要写个代码去看看真实情况会不会真的把SafeMode 清0 (因为在堆中分配内存都是随机的,所以还是有一定的概率可以成功),于是就有了下文,

为什么在声明Array 指定大小的时候可以超过它的上标去赋值但是不能够访问?

 

![图片](pic_temp8\psb21.png)

  

![图片](pic_temp8\psb22.png)

 

  前面已经探讨过Array 的索引问题,因为索引5 在Array 中是不存在的,于是才会访问失败,但是索引6 已经被Array 创建好了新的实体,所以可以正确访问,假如执行下面的代码,在堆中会发生什么情况呢?



```javascript
var i=0;
var array_1=new Array(5);

for (i=0;i<=100;++i)
	array_1[i]=0;
```



   由于时间太迟了,所以没办法一张一张图截给大家看,过程如下:



  Array 对象创建完成,进入for 循环添加Array Var 对象,在起初的索引0-6 时都是按照上图那样在堆中线性分布的,但是在执行到:



```javascript
array[7]=0;
```



 宿主环境会重新分配新的Var 变量给Array 的7 号索引,原来的线性分布已经被打乱,于是继续往后执行赋值,在堆中的不同位置出现Var 变量就越多



  最后关于Array 的连接做个简单的介绍,如果执行下面的代码,会发生什么情况呢?



```javascript
var i=0;
var array_1=new Array(100);
var array_2=new Array(1000);

for (i=0;i<=100;++i)
	array_1[i]=0;

for (i=0;i<=100;++i)
	array_2[-i]=0;

var array_link_add=array_1+array_2;
var array_link_cat=array_1.concat(array_2);
```



  第一句 `var array_link_add=array_1+array_2;` 没有找到合适的断点去观测堆信息,所以详细的过程还无从得知

  第二句 `var array_link_cat=array_1.concat(array_2);` JScript 会先创建新的Array ,然后设置新的Array 的大小为1102 个Array Var Obj 长,找不到图这就先省略吧