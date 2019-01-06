  最近几天在弄AT 指令的AT+CMGS 命令,发觉当AT+CMGF=1 时,AT+CMGS 的返回是OK 的,但信息却没有被另一台手机接收到,于是百度一下,人们都是在AT+CMGF=0 的状态下发送消息的(其实失败的原因很简单,没有用AT+CSCA 设置短信中心的号码),此时发送的消息并不是AT+CMGF=1 时的字符串信息,而是经过编码的PDU 字符串,而且这条PDU 里面还包含有短信中心的号码和目的手机的号码和信息主体,如果你描述觉得不太直观,可以看看下面的对比图:

 

![图片](pic_temp3\psb1.jpg)



![图片](pic_temp3\psb2.jpg)



  当AT+CMGF=1 时的AT+CMGS 的命令格式为:



```txt
  AT+CMGS="targe_phone"<CR>
  message_caption<ESC>
```



  注:\<CR\> 代表回车的ASCII 码,\<ESC\> 代表Esc 键的ASCII 码(在超级终端下直接点键盘就可以,在其它的串口调试工具下需要用特殊的程序从文本框导出这个字符然后复制到缓冲区中)



  当AT+CMGF=0 时的AT+CMGS 的命令格式为:



```txt
  AT+CMGS=pdu_length<CR>
  pdu_code<ESC>
```



  由于PDU 在发送和接收时会有所不同,在此先介绍发送时的PDU 编码规则:



```txt
编码过的短信中心号码长度 + 编码过的短信中心号码 \+ 文件头字节 + 信息类型 + 编码过的接收手机号码长度 + 编码过的接收手机号码 + 短信协议数据 + 短信长度 + 短信数据
```



  例子:



  前提数据: 短信中心号码:8613800200500  接收短信手机:8618819489626 短信数据:信息测试

  编码后的PDU 数据:0891683108200005F011000D91688118499826F6000800084FE1606F6D4B8BD5




  这些数据都是怎么被编码的呢?让我们一起来分析一下:



##  短信发送PDU 编码方式



1. 短信中心号码:8613800200500 与编码后的0891683108200005F0



  或许细心的你已经注意到,08 是代表短信中心的号码长度,表明后面91683108200005F0 是编码过后的短信中心号码,那么我们再细心观察,也许你会发现683108200005F0 与8613800200500 好像很眼熟,其实当你思考一下,把683108200005F0 的奇位与偶位对换,就得到了8613800200500F ,根据推测,最后一位F 很有可能是充当凑够字节长度的(因为:8613800200500 是十三位长的,如果要根据字节来保存到内存的话,必然会剩下一位空余的,于是用F 来填充这里).那么剩下来的91 是代表什么意思呢?根据编码后的短信中心号码和编码后的短信接收号码来推断,91 很有可能代表着号码的类型(比如标识某种号码的属性等)和号码在PDU 字符串中的起始标志位.



2. 以91 为突破点,向下推断接收短信手机号码



  在编码后的短信中心号码的后面,依旧是一串复杂的字符串,由于91 代表这号码的类型,那么我们可以直接在后面搜寻91 那就可以找到接收手机的号码啦.这时,剩下的字符串是11000D91688118499826F6000800084FE1606F6D4B8BD5 ,嗨,第五和第六位不就是91 吗?然后后面跟着的字符串中也存在F ,那么可以断定这个是接收端号码,把它提取出来,就是688118499826F6 ,再按照上面的方法就可以把号码推理出来,得8618819489626 .对了,是不是有些什么东西给遗忘了?那个就是接收端号码的长度,号码的长度是15 ,也就是十六进制的0D ,根据多年软件设计经验,可变长的数据的长度一般是放在数据的前面,那么11000D 的第三位就是接收端数据的长度了.



  可能你会觉得不对劲,为什么接收端的号码和短信中心的号码一样长,而一个是0D 另一个是08 ?其实换个角度来看,08 也就是代表字节的意思,是10 ,刚好是91683108200005F0 的长度(记得要注意,两位十六进制的数据表示一字节,无论是十六进制的数字表示还是字节表示).



  上面就是解密明文数据包的一般思路,多对比,认真观察,吸收经验,很多数据包的格式都可以解密出来的.



3. 短信信息:084FE1606F6D4B8BD5



  怎么看,我们要发送的数据("信息测试")是以怎么样的形式在短信数据里面被编码的呢?其实,在短信协议数据里面就已经被指定为以UC32 (也就是标准版的Unicode )编码来保存到PDU 的.第一位08 的意思就是短信信息的长度(一个汉字占两字节内存),接下来的4FE1606F6D4B8BD5 就是短信的数据.我们况且把"信息测试" 转换成Unicode -- E14F6F604B6DD58B ,和4FE1606F6D4B8BD5 来对比,刚好又是奇位互换的结果.



  据此,我们已经明白了发送短信的PDU 是如何编码的,以下就是VB6 的实现代码:



```visual basic
Private Function CreateMessage(ByVal MessageCenterNumber As String, ByVal MessageReceiver As String, ByVal MessageData As String) As String

If Len(MessageCenterNumber) >= 100 Or Len(MessageReceiver) >= 100 Or Len(MessageData) >= 100 Then Exit Function

Dim MessageCenterNumberResolve As String

If InStr(MessageCenterNumber, "+") Then  ''  由于有些号码会添加"+" 号的,所以需要去处它
    MessageCenterNumberResolve = Mid(MessageCenterNumber, 2)
Else
    MessageCenterNumberResolve = MessageCenterNumber
End If

If Not Len(MessageCenterNumberResolve) Mod 2 Then _  ''  F 的意义是对齐字节
    MessageCenterNumberResolve = MessageCenterNumberResolve & "F"
MessageCenterNumber = ""

For ChangeIndex = 0 To Len(MessageCenterNumberResolve) - 1 Step 2  ''  奇偶互换
    MessageCenterNumber = MessageCenterNumber & Left(Mid(MessageCenterNumberResolve, 2), 1)
    MessageCenterNumber = MessageCenterNumber & Left(MessageCenterNumberResolve, 1)
    MessageCenterNumberResolve = Mid(MessageCenterNumberResolve, 3)
Next
MessageCenterNumberResolve = "91" & MessageCenterNumber  ''  加上91 在奇偶互换的号码之前
MessageCenterNumberResolve = CStr(Format(Len(MessageCenterNumberResolve) / 2, "00")) & MessageCenterNumberResolve

Dim MessageReceiverResolve As String

If InStr(MessageReceiver, "+") Then
    MessageReceiverResolve = Mid(MessageReceiver, 2)
Else
    MessageReceiverResolve = MessageReceiver
End If

If Not Len(MessageReceiverResolve) Mod 2 Then _
    MessageReceiverResolve = MessageReceiverResolve & "F"
    MessageReceiver = ""

                For ChangeIndex = 0 To Len(MessageReceiverResolve) - 1 Step 2
    MessageReceiver = MessageReceiver & Left(Mid(MessageReceiverResolve, 2), 1)
    MessageReceiver = MessageReceiver & Left(MessageReceiverResolve, 1)
    MessageReceiverResolve = Mid(MessageReceiverResolve, 3)
Next
            
MessageReceiverResolve = MessageReceiver

Dim MessageDataResolve As String
Dim MessageDataUnicode() As Byte

MessageDataUnicode = MessageData  ''  由于VB 是Unicode 保存字符串的,所以直接把字符串转换到字节流就可以获取Unicode 码
For CopyIndex = 0 To UBound(MessageDataUnicode) Step 2
    MessageDataResolve = MessageDataResolve & Hex(MessageDataUnicode(CopyIndex + 1))
    MessageDataResolve = MessageDataResolve & Hex(MessageDataUnicode(CopyIndex))
Next

Dim MessageReceiverLength As String
MessageReceiverLength = Hex(IIf(InStr(MessageReceiverResolve, "F"), Len(MessageReceiverResolve) - 1, Len(MessageReceiverResolve)))  ''  计算编码后的接收端号码长度
MessageReceiverLength = Right("00" & MessageReceiverLength, 3 - Len(MessageReceiverLength))
Dim MessageDataLength As String
MessageDataLength = Hex(UBound(MessageDataUnicode) + 1)  ''  短信数据长度
MessageDataLength = Right("00" & MessageDataLength, 3 - Len(MessageDataLength))
CreateMessage = MessageCenterNumberResolve & "1100" & MessageReceiverLength & "91" & MessageReceiverResolve & "000800" & MessageDataLength & MessageDataResolve
End Function
```





二  短信接收PDU 编码方式



  这是前天晚上用舍友的卡接收的PDU 数据:

  0891683108700505F0040D91685110052365F200083160312245642306202620262026



  此时,PDU 的格式为:

编码过的短信中心号码长度(占一字节) \+ 编码过的短信中心号码 \+ 接收参数(占一字节) + 编码过的发送手机号码长度

(占一字节)\+ 编码过的发送手机号码 + 短信数据编码协议 \+ 时间(占七字节) \+ 短信长度(占一字节) \+ 短信数据




  那么解码上面的PDU 数据就有:

  短信中心号码:861380075050 发送手机号码:1501503256  编码协议:UC32 时间:31603122456423 短信数据:………



  这里的时间也是奇偶互换的,换过来是13061322544632 ,转换过来就是2013-06-13 22:54:46 ,最后一位可以忽略,那个是时区.



  对应的解码VB 代码:



```visual basic
Private Function ResolveMessage(ByVal PDUCode As String, Optional ByRef MessageReceiver As String, Optional ByRef MessageData As String, Optional ByRef MessageReceiveTime As String, Optional ByRef MessageCenterNumber As String) As Boolean
Dim MessageCenterNumberLength As Long

MessageCenterNumberLength = CLng(Left(PDUCode, 2))
PDUCode = Mid(PDUCode, 3)

MessageCenterNumber = Left(PDUCode, MessageCenterNumberLength * 2)
MessageCenterNumber = Mid(MessageCenterNumber, 3)
PDUCode = Mid(PDUCode, MessageCenterNumberLength * 2 + 1)

Dim MessageCenterResolve As String

For ChangeIndex = 0 To Len(MessageCenterNumber) - 1 Step 2
    MessageCenterResolve = MessageCenterResolve & Left(Mid(MessageCenterNumber, 2), 1)
    MessageCenterResolve = MessageCenterResolve & Left(MessageCenterNumber, 1)
    MessageCenterNumber = Mid(MessageCenterNumber, 3)
Next

If InStr(MessageCenterResolve, "F") Then
    MessageCenterNumber = Left(MessageCenterResolve, Len(MessageCenterResolve) - 1)
Else
    MessageCenterNumber = MessageCenterResolve
End If

Dim MessageRecviverLength As Long
Dim MessageReceiverResolve As String

MessageReceiverResolve = Left(PDUCode, InStr(PDUCode, "91") - 1)
MessageRecviverLength = CLng("&H" & Left(Mid(MessageReceiverResolve, Len(MessageReceiverResolve) - 1), 1)) * 16
MessageRecviverLength = MessageRecviverLength + CLng("&H" & Mid(MessageReceiverResolve, Len(MessageReceiverResolve) - 1))
MessageRecviverLength = IIf(MessageRecviverLength Mod 2, MessageRecviverLength + 1, MessageRecviverLength) / 2

PDUCode = Mid(PDUCode, InStr(PDUCode, "91") + 2)
MessageReceiverResolve = Left(PDUCode, MessageRecviverLength * 2)
PDUCode = Mid(PDUCode, MessageRecviverLength * 2 + 1)

For ChangeIndex = 0 To MessageRecviverLength - 1
    MessageReceiver = MessageReceiver & Left(Mid(MessageReceiverResolve, 2), 1)
    MessageReceiver = MessageReceiver & Left(MessageReceiverResolve, 1)
    MessageReceiverResolve = Mid(MessageReceiverResolve, 3)
Next

If InStr(MessageReceiver, "F") Then
    MessageReceiver = Left(MessageReceiver, Len(MessageReceiver) - 1)
Else
    MessageReceiver = MessageReceiver
End If

Dim MessageEncodeType As String

MessageEncodeType = Left(PDUCode, 4)
PDUCode = Mid(PDUCode, 5)

Dim MessageTimeResolve As String

MessageTimeResolve = Left(PDUCode, 14)
For ChangeIndex = 0 To Len(MessageTimeResolve) - 1 Step 2
    MessageReceiveTime = MessageReceiveTime & Left(Mid(MessageTimeResolve, 2), 1)
    MessageReceiveTime = MessageReceiveTime & Left(MessageTimeResolve, 1)
    MessageTimeResolve = Mid(MessageTimeResolve, 3)
Next

MessageReceiveTime = "20" & Left(MessageReceiveTime, 2) & "-" & Left(Mid(MessageReceiveTime, 3), 2) & "-" & Left(Mid(MessageReceiveTime, 5), 2) & " " & Left(Mid(MessageReceiveTime, 7), 2) & ":" & Left(Mid(MessageReceiveTime, 9), 2) & ":" & Left(Mid(MessageReceiveTime, 11), 2)
PDUCode = Mid(PDUCode, 15)

Dim MessageDataResolve() As Byte
Dim MessageDataResolveString As String
Dim MessageDataResolveLength As Long

MessageDataResolveLength = Left(PDUCode, 1) * 10 + Left(Mid(PDUCode, 2), 1)

Dim PDUMessageData As String

PDUMessageData = Mid(PDUCode, 3)

For CopyIndex = 0 To MessageDataResolveLength
    MessageDataResolveString = MessageDataResolveString & Left(Mid(PDUMessageData, 3), 2)
    MessageDataResolveString = MessageDataResolveString & Left(PDUMessageData, 2)
    PDUMessageData = Mid(PDUMessageData, 5)
Next

ReDim MessageDataResolve(Len(MessageDataResolveString))
For CopyIndex = 0 To MessageDataResolveLength - 1
    Dim Code As Integer
    Code = CLng("&H" & Left(MessageDataResolveString, 1)) * 16
    Code = Code + CLng("&H" & Left(Mid(MessageDataResolveString, 2), 1))
    MessageDataResolve(CopyIndex) = Code
    MessageDataResolveString = Mid(MessageDataResolveString, 3)
Next

MessageData = MessageDataResolve

ResolveMessage = True

Err:
ResolveMessage = False
End Function
```



##  小测试



1. 发送PDU



短信中心号码:8613800200500 接收手机号码:8618819489626 短信数据:信息测试



```visual basic
Text1.Text = CreateMessage("+8613800200500", "+8618819489626", "信息测试") 
```



![图片](http://a4.qpic.cn/psb?/9707bb18-5c9b-4b76-9222-ec0fdbbff7eb/JLPnr2BYcKvovM5ckWSIa0r3RWOptfZs0LNYel*82d0!/b/dKDfWaY.JAAA&ek=1&kp=1&pt=0&bo=PAJ.AAAAAAABAGc!&t=5&tl=3&su=019400417&tm=1546754400&sce=0-12-12&rf=2-9)



2. 接收PDU



  0891683108200505F0840D91683196032930F0000830302180635480064F60597D0021



```visual basic
  Dim Sender As String
  Dim Center As String
  Dim Time As String
  Dim Data As String

  ResolveMessage Text2.Text, Sender, Data, Time, Center
  Text2.Text = Center & " " & Sender & " " & Time & " " & Data
```


 

![图片](http://a3.qpic.cn/psb?/9707bb18-5c9b-4b76-9222-ec0fdbbff7eb/Q40K6V4xFaUdsTiTxeIQkxPvd4sLzcCTHFvaVYzAd.U!/b/dI.uuaX*JQAA&ek=1&kp=1&pt=0&bo=PAJ.AAAAAAABAGc!&t=5&tl=3&su=057094001&tm=1546754400&sce=0-12-12&rf=2-9)

