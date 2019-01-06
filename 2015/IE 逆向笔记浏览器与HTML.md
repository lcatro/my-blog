## IE 浏览器和HTML 之间的关系



  图中黑线代表包含关系,红线代表继承关系

 

![图片](pic_temp10\psb1.png)



## IE 浏览器结构



  CHTMLApp 类实例一个浏览器程序对象,CHTMLApp 把所有浏览器的逻辑都放到CDoc 来管理.CDoc 管理多个CMarkup (浏览器标签对象),同时包含CServer 做Windows 窗口消息和其他相关的处理.CMarkup 对象管理整个前端的对象和信息,比如说CMarkup 包含了CWindow 对象(也就是HTML 里面的Window 对象),同时也把CScriptCollection (所有的<script> 脚本对象)和CElementCollection (所有的HTML 元素对象)分开成两个Array 管理.每个CScriptCollection 和CElementCollection 对象里面都保存着CElement (HTML 元素基础对象),只不过CScriptCollection 保存CScriptElement (<script> HTML 脚本对象),CElementCollection 保存CElement (因为HTML 对象的实例各个都是不同的类,但是他们都以CElement 为基类,所以在表示不确定的HTML 元素对象时用HTML 元素对象的基类来表示).继续往下就分析到IE 浏览器的HTML 结构.



## IE 浏览器HTML 结构



1. HTML 元素部分

 

  所有和HTML 相关的对象都继承CBase 类.为了防止XSS 攻击,CBase 有IXSSFilterCallBack (XSS 过滤器验证处理回调),以确保每个HTML 对象都检查XSS 攻击.HTML 元素包含样式,属性,子元素的集合,与之相对应的是,在CElement 类里面包含CStyle (HTML 样式),CAttrArray (HTML 属性集合),CCollectionCache (HTML 子元素收集器,CElementCollection 和CScriptCollection 都继承CCollectionCache ),值得注意的一点是:CCollectionCache 下包含的CElement 并不是以树的关系去储存的,而是以Array 的方式把CElement 以有顺序的方式来保存的(关于这一点建议用console.dir(document.all); 来观察HTMLCollection ).关于HTML 元素,还有一点需要补充,就是CElement 派生出的子元素,有些HTML 元素是文本节点(比如:<p><a> 标签) ,这些元素直接继承CElement ;有些HTML 元素带有滚动条(比如:<textarea> 标签) ,它们同时也继承CTxtSite 对象;有些HTML 元素支持Ole 技术(比如flash) ,它们也继承COleSite 对象,通过这个父类获取到Ole 技术的接口支持.



2. Document 部分



  Document 管理整个HTML 文档,关于HTML 元素部分已经表述完,还有一部分是关于CSS 的结构.CSS 用<style> 标签来表示,在HTML 文档里面会包含一到多个<style> 元素,于是CDocument 对象用CStyleSheetArray 管理当前HTML 文档里面所有的<style> 节点(CStyleSheet 对象).<style> 元素里面会包含多个HTML 元素的样式规则,所以每个CStyleSheet 对象用CStyleSheetRuleArray 来管理包含的HTML 元素样式规则(CStyleSheetRule ),最后这个CStyleSheetRule 通过实例化CStyle 来把样式通过CElement 来展示



3. 关于UAF



  通过上面的得出的结论和一些阅洞得出的心得,发现UAF 出现在Array 储存中的地方比较多.在上次调试JScript.dll 的过程中,发现一个问题



```javascript
var array=new Array[5];
```



  看似分配了大小为5 的数组对象,但是如果如果执行下面的代码,将不会产生错误



```javascript
array[10]=1;
```



  此时JScript.dll 不会产生一个数组溢出错误,而是重现在堆里面分配新的VARNIANT 对象空间,再保存到JS 的Array 对象中,此时Array 中包含的数组索引如下:



```javascript
[0,1,2,3,4,10]
```



  执行下面的代码会产生错误



```javascript
alert(array[6]);
```



  因为Array 中没有6 号索引,所以JScript.dll 读取该数组对象时会找不到索引产生错误(Chrome 下不产生错误,返回undefine )



  假设Array 可以被释放然后再引用被释放的位置时呢?UAF 就产生了,详情查看CVE-2013-8393 (http://www.freebuf.com/vuls/54786.html) ,CVE-2014-1772 (http://bobao.360.cn/learning/detail/88.html) 等经典UAF 分析

 