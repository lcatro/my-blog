

## Win7 下如何设置DPI (Dots Per Inch,每英寸像素数) 以及DPI 的作用



在"桌面" 点击"鼠标右键" 选择"个性化" 后,再点击左下角的"显示" .



![图片](pic_temp5\psb1.jpg)

 

这个选项是为了方便老年用户希望窗口和字体能够放大一些以便更加容易看到.



![图片](pic_temp5\psb2.jpg)

 

![图片](pic_temp5\psb3.jpg)



修改了它实际上就是修改了系统的DPI ,DPI 将会改变系统字体和其它选项的大小.在不同大小的DPI 下,系统将会对程序的窗口做一些微小的调整,这一调,设计出来的程序就恶心了



## 界面显示错位



1. 开发实例



这是一个在系统默认缩放下显示正常的程序:



![图片](pic_temp5\psb4.jpg)

 

然后换到中等缩放再显示:

 

![图片](pic_temp5\psb5.jpg)

 

你会惊奇的发现,"怎么图片都不是按照原来在设计界面上显示的?","图片怎么会一大一小一正常?",这就是DPI 和VB 的默认长度计量单位缇(Twip) 惹的祸.



2. 解决方法



测试了几次后,发现在中等缩放下的程序界面才会变得不正常.据我所知系统DPI 只取96 和120 这两个值,于是就先来定义它们.



```visual basic
Private Const DPI_BIG = 120
Private Const DPI_SMALL = 96
```


在DPI 为96 的情况下,Twip 和Pixel (像素)的换算为15:1 ,但在DPI 为120 的情况下,它们之间的换算为12:1 .这就是界面显示出错的原因所在(长度单位换算搞错了).于是再定义一下它们之间的换算.



```visual basic
Private Const DPI_BIG_TWIP = 12
Private Const DPI_SMALL_TWIP = 15

Private Function CalcuDPIBigPixel(ByVal Pixel As Long) As Long
CalcuDPIBigPixel = Pixel * DPI_BIG_TIWP
End Function


Private Function CalcuDPISmallPixel(ByVal Pixel As Long) As Long
CalcuDPISmallPixel = Pixel * DPI_SMALL_TIWP
End Function

''    通过在程序初始化中获取DPI 值来进行界面的重新排版(假定我们的程序是在96 DPI 下设计的)
If (DPI_BIG = GetSystemDPI) Then
    Me.Height = CalcuDPIBigPixel(PIXEL_BACKGOUND_HEIGHT)  ''  重新对背景图片进行调整
    Me.Width = CalcuDPIBigPixel(PIXEL_BACKGOUND_WIDTH)
 
    Dim Control As Control
    For Each Control In Me  ''  对每一个控件进行调整
        Control.Top = CalcuDPIBigPixel(Control.Top / DPI_SMALL_TIWP)
        Control.Left = CalcuDPIBigPixel(Control.Left / DPI_SMALL_TIWP)
    Next
End If
```

