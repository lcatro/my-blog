
绝大部分的浏览器都是利用Chrome 或者IE 作为浏览器的内核,外部的界面和其他拓展功能只是对浏览器内核的包装.出现稳定的地方就是厂商在包装自己的浏览器时出现的问题.



![img](pic_temp1\1.png)



各种浏览器都有个特权域,在这个域里面会提供些控制浏览器的js 接口,比如下载插件,启动程序等.



外部和特权域交互的方式大致有三种:

XSS (<http://www.wooyun.org/bugs/wooyun-2010-0128245>)

CSRF 

window.onmessage (<http://www.wooyun.org/bugs/wooyun-2015-095664>)



有些接口会用backbone 包装,可以用require 去使用接口:



![img](pic_temp1\2.png)



![img](pic_temp1\3.png)



测试流程如下:

1.先找域下开放了哪些权限较大的接口

2.找window.onmessage

3.找XSS

4.再找其它域