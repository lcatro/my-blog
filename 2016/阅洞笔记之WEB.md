

-- Banner :脑洞有多大,漏洞就有多大





-- 感悟:



1.获取数据最好的方式是黑掉运营商(因为运营商有对接国家查询数据接口和更多的个人敏感信息的储存),教育机构(安全防护比较低也有比较多的个人信息),服务行业(比如:酒店,火车站,社保,都是好数据),大型论坛(我自己看来主要是获取帐号密码去撞库用,个人敏感信息不会有太多)





-- 思路:



猪猪侠的思路:

1.google 搜索 site:qq.com filetype:action ,找JSP 的处理后台,有概率找到远程代码执行漏洞

2.大部分的是通过CMS 的EXP 拿Shell ,特别是Disuz ,很多原因都是因为配置文件没有给保护好给用户访问到敏感数据

3.特别注意弱口令

4.完整的日站过程首先收集和站点相关数据:

 EMail - 查询社工库进后台,一般在中大型企业中各个用户都会有自己在企业中的EMail ,在内网的时候Email 还会和许多东西都有绑定,所以还可以继续渗透,可以尝试下根据Email 的前缀来搜索下相关Email 再试试撞库入后台

 旁站查询 - 本站拿不下就日旁站

 用户,密码 - 你懂的







种田哥很多挖的洞很多都和服务行业有关,比如酒店,医疗,教育.思路如下:

1.很多功能都会有单独的测试,针对不同的功能测试不同的漏洞,比如任意密码重置绕过,SQL 注入,越权查询,上传文件

2.登陆模块的用户名和密码可以尝试SQL 注入测试

<http://www.wooyun.org/bugs/wooyun-2010-037233>

3.目录的收集很重要,特别是后台目录和配置文件的路径

 注意SVN ,web.config ,CMS 配置文件和WEB-INF 的东西,有很大概率会有好东西发现





zeracker (多数漏洞是针对注入的)







针对模块测试思路:



SQL 注入:

1.找SQL 注入最好根据列出爬出来的所有站点和URL 下带有参数的链接来测试注入,一般在主站或者显眼的地方都不怎么会出现注入漏洞,但是要多多考虑旁站和其他隐蔽的URL ,总会有一个能bingo

2.批量抓注入点:<http://www.wooyun.org/whitehats/>%E6%B7%B1%E5%BA%A6%E5%AE%89%E5%85%A8%E5%AE%9E%E9%AA%8C%E5%AE%A4/type/1/page/1

3.大多数情况下对外部访问的站点可能没有注入点,尝试进入后台找后台的注入点,一般这些地方的安全防护没有那么严谨,因为对于用户来说是合法的

4.手工注入常用方法:

 length(database()) 获取数据库名的长度,ascii(substr(database(),1,1)) 获取第一位数据库名的ASCII

 在构造id 的注入测试时,可以利用某个特定的ID 减去数据库名的长度这种方法去测试是否有注入,原理如下:

 <http://example.com/news.aspx?id=400>

 原本页面会根据id 去查询数据库,然后使用下面的测试语句

 http://example.com/news.aspx?id=400-length(database())

 这样有可能id 会访问到一个不是400 的位置,所以可以认定这是一个注入点,length(database())=old_id-new_id=400-跳转到的新id 号

 有了长度,就可以根据substr() 来循环读取database() 的ascii 码

 http://example.com/news.aspx?id=400-ascii(substr(database(),1,1)) -> 根据上面的减法原理获取到数据库名的第一位ASCII 码
 http://example.com/news.aspx?id=400-ascii(substr(database(),2,1)) -> 根据上面的减法原理获取到数据库名的第二位ASCII 码

 ...

 version() 查看数据库版本,user() 获取当前数据库用户,@@version_compile_os 获取系统信息

5.测试登陆模块中是否存在注入点

<http://www.wooyun.org/bugs/wooyun-2015-0146079>

6.其实针对APP 的数据交互来做注入测试也比较容易出漏洞,不过APP 比较多,慢慢找咯

<http://www.wooyun.org/bugs/wooyun-2015-0156597>

<http://www.wooyun.org/bugs/wooyun-2015-0148761>





SSRF 思路:

1.SSRF 是让服务器去访问数据,所以在传入的URL 中只需要提供一个内网的URL 就可以去漫游内网的资源,再拿下

<http://bobao.360.cn/learning/detail/240.html>

2.用SSRF 访问内网时记得把目的站点的DNS 域下所有的域名都查询下,有时会有惊喜哦

<http://www.wooyun.org/bugs/wooyun-2010-026249>

3.SSRF 一般防御是通过解析URL 的域名是否为IP 地址和是否存在端口,比如这样就是一个合法的访问请求

 <http://example.com/index.html>

 但是用IP 去访问就会失败

 <http://10.0.12.23/index.html>

 <http://10.0.0.1:8080/index.html>

 可以在URL 的域名和端口部分做一些混淆去绕过过滤

 http://test@172.16.2.1/admin/

 多做混淆,在WEB 之困里面的URL 混淆有几种,位元法,二进制IP 地址表示法

4.特别注意那种在URL 里面有url 属性的连接,在触发这些URL 时有可能会产生由服务器代理用户去实现资源请求然后达到SSRF 

5.SSRF 对于那种中大型企业和站点的渗透来说比较有用,如果针对小型服务器来说的话没有什么卵用

<http://www.wooyun.org/bugs/wooyun-2010-0113648>





URL 构造:

1.在构造跳转的URL 时记得考虑到这个URL 是由服务器还是浏览器去执行,如果要使用到URL 编码和链接混淆的话只有浏览器可以识别(因为对于这种URL 浏览器是会解码再访问的),而服务器是直接把URL 拿去访问,所以就会访问失败

 example:

  [http://www.baidu.com](http://www.baidu.com/)@qq.com => [http://www.qq.com](http://www.qq.com/)

  http://ⅅʳºℙˢ.ʷººʸⓊⁿ.ºʳℊ =><http://drops.wooyun.org/>

<http://drops.wooyun.org/tips/750>

2.使用这种URL 混淆可以绕过地址检测,在QQ 或者Email 中可以结合这种方式去钓鱼

3.Fuzzing 浏览器的URL 解析可以尝试利用JS 的location.href 做到,如果可以成功跳转就是一个可以被构造的URL ,不能跳转的话一般都会有提示JS 语法错误之类的





XSS 思路 (gainover ,xsser ):

1.活用编码混淆绕过过滤

2.有一些可以由用户传入的HTTP 请求有可能存在XSS

3.有MHTML 协议的地方特别要测试一下XSS

<http://www.wooyun.org/bugs/wooyun-2010-01199>

<http://www.wooyun.org/bugs/wooyun-2010-01288>

<http://www.wooyun.org/bugs/wooyun-2010-01479>

4.要调试输入的字符在页面的输出,然后大胆测试代码能否可以被越界,从用户名到个人信息到上传文件名等地方都可以做储存型XSS ,思路不要给局限

<http://www.wooyun.org/bugs/wooyun-2010-08141>

5.在XSS 中有可能会遇到长度限制,如果需要引用跨站脚本和跳转URL 时,善用短域名去绕过长度限制

6.输入的内容有很多种,可以尝试用附加文件或者其它的可以控制输入的地方来进行XSS 测试

<http://www.wooyun.org/bugs/wooyun-2015-0138419>

7.特别要注意富文本框的XSS 测试,这个地方经常会存在XSS 问题

8.如果有些地方需要检测后缀的话(比如检测图片),可以尝试结合URL 中& 号来绕过

Example :假设判断一个图片

http://ffff.com/im.jpg

然后你需要构造一个CSRF ,可以这样写

http://ffff.com/csrf?param=value&.jpg

<http://www.changwei.me/archives/75>

9.PAKV 心伤的瘦子挖掘XSS 的方法:找可以输出的地方,输出一条特殊的字符串(比如:aaaaaaaaaaa ,qwepoiasdlkjzxcmnb 之类唯一的字符串),然后再去全局HTML 和JS 代码中CTRL+F 搜索特征字符串

10.如果要对CSS 测试XSS ,可以试试下面的混淆:

\+ascii = 字符 => \65=A , \108=l





遍历数据思路:

1.有时候URL 的路径里面会包含有ID ,可以尝试下利用这个ID 去测试能否访问其它用户的信息

 Example:

 <http://weibo.com/u/2515840640/home?wvr=5&lf=reg> 通过修改URL 里面的2515840640 可以访问其他用户的信息

2.有些是通过接口去查询用户信息的,可以直接改ID 去尝试访问

 Example:

 <http://class.sise.com.cn:7001/SISEWeb/pub/course/courseViewAction.do?method=doMainNotDesEncrypter&isclose=Y&studentid=>

 <http://cjxg.gzccc.edu.cn/lyxg/classmgt/studentdetail/studentDetailInfo.do?xh=>

 <http://192.168.6.53:8080/portal_WebService_gzccc/XszpServlet?userid=>

 <http://returns.vancl.com/PublicHandlers/AddrAscx/Handler1.ashx?orderid=212021461664&target=defaultaddr>

3.上传图片和文件到服务器时会保存成为一个随机的文件名,连续上传几个相同的文件然后获取他们的URL 去观察,如果这几个文件名是连续的或者有规律的就尝试往上遍历看看可以Get 到不该访问的数据不





上传漏洞挖掘思路:

1.如果在form 里面可以找到编辑器的upload.php 那就直接上传一句话

2.如果可以进入到网站的后台,里面可能会对文件上传的限制不大,可以直接传马(注意那些传附件的上传点)

3.可以尝试下利用解析漏洞来把图片木马转换成为代码(把一句话嵌入到图片里面),本来上传是一个图片

 xxx/user_file/1.jpg

 改写成

 xxx/user_file/1.jpg/.php

 不过这种方法只能用在nginx 里面

4.有些站点本来是上传图片的,但是可以直接上传文件...

5.编辑器上传漏洞:

 <http://tieba.baidu.com/p/728438658>

 <http://www.518z.com/xinshou/xitonganquan/2013/1211/254764.html>

6.有些编辑器在上传页面里面会带有file 之类的过滤文件类型,可以试试传递一些参数看看能不能绕过原来对文件上传的限制





防火墙绕过思路:

1.尝试自己布置防火墙构造fuzzing 绕过规则





支付模块测试思路:

1.不一定要在前端往后台提交订单的时候修改金额来达到目的,尝试利用修改回调接口欺骗后台订单付款成功

<http://www.wooyun.org/bugs/wooyun-2015-0146510>




整站渗透其它思路:



1.猜管理员用户名和弱口令密码的能力很重要(特别对于中大型企业和国企)

<http://www.wooyun.org/bugs/wooyun-2010-038658>

<http://www.wooyun.org/bugs/wooyun-2010-054540>

<http://www.wooyun.org/bugs/wooyun-2015-0146107>

<http://www.wooyun.org/bugs/wooyun-2015-0147714>

一些弱口令收集:

admin access login manager 123456 111111 hello 1234 guest

 Email 格式:%username%@%host%

  可以推出的弱口令:

   %username%123 (LCatro123),

   %username%%year% (LCatro2015),

   %host% (sise),

   %host%%year% (sise2015)



 记得,对于中大型企业有时侯主站和一些站是会有验证码的,所以可以继续寻找其他可以登陆的站点来测试弱口令,一般这些密码都会在同站下通用的

 还有,可以尝试利用一个固定的密码去测试收集,生成的所有用户名,有纪律bingo ..



2.对于大型站点的漏洞挖掘,建议花时间测试各个模块以及耐心分析业务逻辑来找出漏洞 (呆子不开口)

<http://www.wooyun.org/bugs/wooyun-2010-099486>

<http://www.wooyun.org/bugs/wooyun-2010-0124352>

<http://www.wooyun.org/bugs/wooyun-2010-070454>

3.代理调试数据流向真的很重要,单单的前端代码审计只能作为日站中的一部分来做参考

4.对于站点的渗透最后完全覆盖,多用google 和site: ,inurl: 找到隐蔽的信息

5.rsync 同步备份远程linux 系统到本地,用此方法可以get 到数据库和站点代码

 命令格式:rsync %host%

6.利用DNS 域传送获取站点下其他域名进一步渗透

 Example:

 nslookup

 server %TargetDnsHost%

 ls %TargetSiteHost%

 利用该站的DNS 去访问域下的信息

<http://www.wooyun.org/bugs/wooyun-2010-0745>

7.利用多个搜索引擎找后台URL 和站点信息

8.不要太纠结于前端的代码,多用burp 抓取前台和后台交互的数据包

9.Email 有时候没有办法从站点上爬数据和访问,但是可以尝试用不同种的方式去猜测邮箱名再爆破弱口令,通过mail 服务器渗透内网也比较有效(北京方便面)

10.当数据库,自动化攻击工具和最重要的无任何安全控制的接口融合在一起的时候,问题就出来了(弱口令,越权拖数据)

11.如果能够找到目标站点的移动版就更好,一般对于移动端的站点来说安全性可能没有做得那么好(无验证码限制,注入,越权访问等),如果进不去就尝试改User-agent 

12.网络设备在入侵的过程中也很重要

<http://www.wooyun.org/bugs/wooyun-2010-0105581>

<http://www.wooyun.org/bugs/wooyun-2010-021877>

13.模板,CMS 和OA 系统的漏洞可以尝试下,很多站点都是基于他们之上建立的,就算是大型企业里面也会隐蔽地部署着几台这样的东西

14.渗透中大型企业搞邮件服务器也是一条进内网的路

15.测试redis 未授权访问和struct 系列的漏洞,就算是现在也还有很多这样的概率

16.在实际测试中,遇到.jsp 之类的网站可以不进行详细的测试直接上EXP 

17.如果可以浏览目录,要深入挖掘一下重要文件进行下一步渗透

<http://www.wooyun.org/bugs/wooyun-2015-0148002>

18.看到没有验证码输入的地方一定要用burp 爆破一下





脑洞大的思路:

1.利用referer get key

<http://www.wooyun.org/bugs/wooyun-2010-070454>

<http://www.wooyun.org/bugs/wooyun-2010-0136903>

referer 可以获取上一个网页到访问到这个页面的的URL ,有时候这个URL 回携带着敏感的URL 信息,所以可以根据这个referer 获取到的URL 去访问用户信息,一般以图片的形式插入到这个网页里面(或者XSS 图片)来获取当前页面的URL ..

2.注意跳转页面,有时候在本站下无法完成XSS :\<iframe\> \<img\> 来Get 到referer 的信息,使用同站跳转可以绕过这个限制

3.MVC 架构开发WEB 应用时有时候会利用一个index.php 去做类似于网关转发访问数据的过程,图如下:



 对于这种架构可以尝试下找出系统的文件位置(因为有一部分MVC 架构是CMS ,影响范围很大)所以用开源的CMS 去找文件分部再用这种传递到index.php 去访问本地资源的模式来获取敏感信息

<http://www.wooyun.org/bugs/wooyun-2010-077249>

4.如果可以实现XSS 或者跨站插入图片,可以尝试利用下DNS 劫持攻击,例子如下:



![](C:\Users\Administrator\Desktop\code_file\blog\2016\pic_temp2\1.png)



 然后浏览器会根据这张图片去进行HTTP 访问,然后我们把这个图片去重定向浏览器(302),让浏览器跳到本地的路由器执行CSRF ([http://admin:admin](http://admin:admin/)@192.168.1.1/userRpm/LanDhcpServerRpm.htm?dhcpserver=1&ip1=192.168.1.100&ip2=192.168.1.199&Lease=120&gateway=0.0.0.0&domain=&dnsserver=&dnsserver=54.248.102.5&dnsserver2=8.8.8.8&Save=%25B1%25A3+%25B4%25E6)

<http://www.wooyun.org/bugs/wooyun-2010-048499>



