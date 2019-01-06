
  现在以一个最简单的代码来开始我们的逆向旅程,为了方便学习,所有的代码编译和分析都在http://remix.ethereum.org/# 上进行.默认IDE 选项是关闭代码优化(Enable Optimization)和使用0.4.25 版本的solidity 编译器编译.
  
```solidity

    pragma solidity ^0.4.24;

    contract test {

        function a() {
            uint a = 123;
        }

    }
    
```

  在Remix 上输入上述代码,点击`Start to Compile` 对代码进行编译,可以在`Details` 按钮里面获取编译出来的结果.结果包含如下:

```txt

    METADATA  编译器元数据,包含:编译器版本,编译器设置,源码信息等
    BYTECODE  合约编译完整的字节码结果
    ABI  应用程序接口,用于标识合约提供了哪些函数给外部调用
    WEB3DEPLOY  Web3js版合约部署代码
    METADATAHASH  元数据哈希
    GASESTIMATES  编译器计算函数调用需要消耗的Gas表
    RUNTIME BYTECODE  合约运行时字节码
    ASSEMBLY  字节码反汇编
    
```

  读者们会注意到,编译结果里面有`ByteCode` 和`Runtime Bytecode` ,分别表示什么意思呢,在此先略过这个疑问,我们关注`Runtime Bytecode` 中返回的字节码.对代码结构分析和字节码分析的标注后所有代码如下:

```txt

    000 PUSH1 80
    002 PUSH1 40
    004 MSTORE             ;  把0x80 写到[0x40 ,0x40 + 0x20] 这块内存里面,因为内存是空的,这会创建新的内存
    005 PUSH1 04
    007 CALLDATASIZE       ;  获取CALLDATA 的长度
    008 LT                 ;  LT 和PUSH1 0x4 对应,判断CALLDATA 的长度是否小于0x4
    009 PUSH1 3f
    011 JUMPI              ;  如果小于0x4 就往下跳转到0x3F
    012 PUSH1 00
    014 CALLDATALOAD       ;  CALLDATALOAD 0x0 ,PUSH1 0x0 是给CALLDATALOAD 的参数,意思要获取CALLDATA 数据的偏移位置
    015 PUSH29 0100000000000000000000000000000000000000000000000000000000
    045 SWAP1
    046 DIV                ;  DIV 和PUSH29 对应,意思是把上面的数据向左移28 字节,剩下4 字节是调用合约函数名的哈希
    047 PUSH4 ffffffff
    052 AND                ;  AND 和PUSH4 0xFFFFFFFF 对应,保留低位4 字节数据,高位去处
    053 DUP1
    054 PUSH4 0dbe671f     ;  这个是合约有的函数名,经过sha3() 精简过的
    059 EQ                 ;  判断传递过来的函数调用名是否相等
    060 PUSH1 44
    062 JUMPI              ;  如果两值相等就往下跳转到0x44
    063 JUMPDEST           ;  空指令
    064 PUSH1 00
    066 DUP1
    067 REVERT             ;  没有匹配到相应的函数就撤销所有操作,Revert(0,0)
    068 JUMPDEST
    069 CALLVALUE          ;  获取用户转帐数额
    070 DUP1
    071 ISZERO             ;  如果用户转帐数额为0
    072 PUSH1 4f
    074 JUMPI              ;  转帐数额不为0 则跳到0x4F,否则就退出
    075 PUSH1 00
    077 DUP1
    078 REVERT             ;  因为调用函数a() 是不需要附加转帐金额的,所以检测到带有附加金额的函数调用就退出,参考payable 关键字
    079 JUMPDEST
    080 POP
    081 PUSH1 56
    083 PUSH1 58
    085 JUMP               ;  跳转到地址88
    086 JUMPDEST
    087 STOP               ;  停止执行
    088 JUMPDEST
    089 PUSH1 00
    091 PUSH1 7b
    093 SWAP1
    094 POP
    095 POP
    096 JUMP               ;  跳转到地址86
    097 STOP
    ----  合约代码结束分界线  ----
    098 LOG1
    099 PUSH6 627a7a723058
    106 SHA3
    107 MUL
    108 PUSH15 5fd8c2f6fe4103dba9baf9c48c052e
    124 CALLDATALOAD
    125 INVALID
    126 PUSH1 d9
    128 INVALID
    129 INVALID
    130 TIMESTAMP
    131 STATICCALL
    132 INVALID
    133 INVALID
    134 DUP13
    135 INVALID
    136 TIMESTAMP
    137 INVALID
    138 NUMBER
    139 STOP
    140 INVALID
    
```

  标注信息把合约代码的结构和执行过程的思路都理清了,但是读者们会发现以下的问题:
  
```txt

    1.为什么会有合约代码结束分界线,多出来的代码究竟是什么
    2.为什么会有很多多余的跳转
    3.JUMPDEST 是无意思的字节码,为什么会多次出现
    
```

  要解决这些疑问,那么就需要深入到solidity 编译器的源码分析.在https://github.com/ethereum/solidity 中找到solc 编译器的源码,定位到`libsolidity\Codegen\ContractCompiler.cpp` 文件的`ContractCompiler::compileContract()` 函数,对该函数的分析如下:

```cpp

    void ContractCompiler::compileContract(
        ContractDefinition const& _contract,
        std::map<const ContractDefinition*, eth::Assembly const*> const& _contracts
    )  //  合约编译
    {
        // ...
        
        initializeContext(_contract, _contracts);  //  初始化执行环境上下文
        appendFunctionSelector(_contract);         //  根据合约内使用到的函数进行汇编构造
        appendMissingFunctions();                  //  链接不公开的函数(非public 声名)
    }
    
```
    
  `initializeContext()` 函数主要功能是初始化执行环境上下文,并把初始化的机器码输出到字节码缓存.
    
```cpp
    
    void ContractCompiler::initializeContext(
        ContractDefinition const& _contract,
        map<ContractDefinition const*, eth::Assembly const*> const& _compiledContracts
    )
    {
        m_context.setExperimentalFeatures(_contract.sourceUnit().annotation().experimentalFeatures);
        m_context.setCompiledContracts(_compiledContracts);
        m_context.setInheritanceHierarchy(_contract.annotation().linearizedBaseContracts);
        CompilerUtils(m_context).initialiseFreeMemoryPointer();  //  初始化EVM 内存指针
        registerStateVariables(_contract);
        m_context.resetVisitedNodes(&_contract);
    }
    
    const size_t CompilerUtils::freeMemoryPointer = 64;
    const size_t CompilerUtils::zeroPointer = CompilerUtils::freeMemoryPointer + 32;
    const size_t CompilerUtils::generalPurposeMemoryStart = CompilerUtils::zeroPointer + 32;
    
    void CompilerUtils::initialiseFreeMemoryPointer()
    {
        m_context << u256(generalPurposeMemoryStart);  //  generalPurposeMemoryStart 的值为0x80,输出0x80 到字节码缓存
        storeFreeMemoryPointer();
    }

    void CompilerUtils::storeFreeMemoryPointer()
    {
        m_context << u256(freeMemoryPointer) << Instruction::MSTORE;  //  freeMemoryPointer 的值为0x40 ,输出0x40 和指令MSTORE 到字节码缓存
    }
    
```

  根据上面的代码可以知道,`ContractCompiler::initializeContext()` 会输出`MSTORE 0x40,0x80` 到合约字节码的头部,也就是我们常看到合约机器码的开头部分:`6080604052` .`appendFunctionSelector()` 函数是把合约里面编译好的函数和合约初始化检测的代码组合在一起,如果没有深入了解`appendFunctionSelector()` 的代码生成过程,那就很难理解solc 为什么会这样生成字节码.

```cpp

    void ContractCompiler::appendFunctionSelector(ContractDefinition const& _contract)
    {
        map<FixedHash<4>, FunctionTypePointer> interfaceFunctions = _contract.interfaceFunctions();  //  合约中声名的公开函数列表
        map<FixedHash<4>, const eth::AssemblyItem> callDataUnpackerEntryPoints;  //  函数代码入口点

        if (_contract.isLibrary())  //  判断合约是否为库代码
        {
            solAssert(m_context.stackHeight() == 1, "CALL / DELEGATECALL flag expected.");
        }

        FunctionDefinition const* fallback = _contract.fallbackFunction();
        eth::AssemblyItem notFound = m_context.newTag();    //  创建新的汇编Tag ,Tag 的意义是用来标注汇编代码块声名和跳转到某一段汇编用的
        // directly jump to fallback if the data is too short to contain a function selector
        // also guards against short data
        m_context << u256(4) << Instruction::CALLDATASIZE << Instruction::LT;  //  判断CALLDATA 内容长度是否大于等于4 字节
        m_context.appendConditionalJumpTo(notFound);    //  插入条件跳转,LT 判断不通过就跳转到notFound 代码块

        // retrieve the function signature hash from the calldata
        if (!interfaceFunctions.empty())
            CompilerUtils(m_context).loadFromMemory(0, IntegerType(CompilerUtils::dataStartOffset * 8), true);  //  从CALLDATA 中提取要调用的函数哈希
            //  构造代码PUSH 0x00 ,CALLDATALOAD ,PUSH29 0100000000000000000000000000000000000000000000000000000000 ,SWAP1 ,DIV ,PUSH4 0xFFFFFFFF ,AND

        // stack now is: <can-call-non-view-functions>? <funhash>
        for (auto const& it: interfaceFunctions)
        {
            callDataUnpackerEntryPoints.insert(std::make_pair(it.first, m_context.newTag()));  //  对函数入口创建新汇编代码块声名
            m_context << dupInstruction(1) << u256(FixedHash<4>::Arith(it.first)) << Instruction::EQ;  //  构造代码:DUP1 ,PUSH4 函数哈希 ,EQ
            m_context.appendConditionalJumpTo(callDataUnpackerEntryPoints.at(it.first));  //  如果数值相同,则跳转到目的函数地址,对应PUSH + JUMPI 指令
        }
        m_context.appendJumpTo(notFound);  //  没有找到的话就跳转到notFound 触发revert(0) 退出

        m_context << notFound;  //  声名notFound 的代码段
        if (fallback)
        {
            solAssert(!_contract.isLibrary(), "");
            if (!fallback->isPayable())
                appendCallValueCheck();

            solAssert(fallback->isFallback(), "");
            solAssert(FunctionType(*fallback).parameterTypes().empty(), "");
            solAssert(FunctionType(*fallback).returnParameterTypes().empty(), "");
            fallback->accept(*this);
            m_context << Instruction::STOP;
        }
        else
            // TODO: error message here?
            m_context.appendRevert();
        //  对notFound 的代码进行填充,因为fallback=fakse ,执行m_context.appendRevert() ,所以notFound 的代码序列是 PUSH1 0x00 ,DUP1 ,REVERT .意思是revert(0x0)
        
        //  上面是构造合约执行数据检测的代码,下面是对各个公开调用(指public 声名)的函数进行入口点构造
        for (auto const& it: interfaceFunctions)
        {
            FunctionTypePointer const& functionType = it.second;
            solAssert(functionType->hasDeclaration(), "");
            CompilerContext::LocationSetter locationSetter(m_context, functionType->declaration());

            m_context << callDataUnpackerEntryPoints.at(it.first);
            if (_contract.isLibrary() && functionType->stateMutability() > StateMutability::View)  //  库函数且关键字声名不是pure 和view 的函数
            {
                // If the function is not a view function and is called without DELEGATECALL,
                // we revert.
                m_context << dupInstruction(2);
                m_context.appendConditionalRevert();
            }
            m_context.setStackOffset(0);
            // We have to allow this for libraries, because value of the previous
            // call is still visible in the delegatecall.
            if (!functionType->isPayable() && !_contract.isLibrary())  //  如果函数没有启用Payable 关键字或者这是库函数的话,都不支持接收合约调用携带转帐金额
                appendCallValueCheck();    //  添加对转帐金额检测代码

            // Return tag is used to jump out of the function.
            eth::AssemblyItem returnTag = m_context.pushNewTag();  //  对函数创建返回代码段声名
            if (!functionType->parameterTypes().empty())  //  如果函数有参数的话
            {
                // Parameter for calldataUnpacker
                m_context << CompilerUtils::dataStartOffset;  //  CompilerUtils::dataStartOffset 指的是函数参数数据在TXDATA 里的偏移
                m_context << Instruction::DUP1 << Instruction::CALLDATASIZE << Instruction::SUB;  //  计算函数参数的大小
                CompilerUtils(m_context).abiDecode(functionType->parameterTypes());    //  从TXDATA 中获取参数
            }
            m_context.appendJumpTo(m_context.functionEntryLabel(functionType->declaration()));  //  调转到函数入口点
            m_context << returnTag;  //  声名函数返回的代码段
            // Return tag and input parameters get consumed.
            m_context.adjustStackOffset(
                CompilerUtils(m_context).sizeOnStack(functionType->returnParameterTypes()) -
                CompilerUtils(m_context).sizeOnStack(functionType->parameterTypes()) -
                1
            );
            // Consumes the return parameters.
            appendReturnValuePacker(functionType->returnParameterTypes(), _contract.isLibrary());  //  构造函数返回值处理
        }
    }
    
```

  上面的代码分析可能看起来有些晦涩难懂,作者把上面分析到的编译过程一一对应到编译结果并标注,汇编代码如下:

```txt

    -----  initializeContext() -> initialiseFreeMemoryPointer()  ----
    000 PUSH1 80         ;  initialiseFreeMemoryPointer()  ,  m_context << u256(0x80)
    002 PUSH1 40         ;  storeFreeMemoryPointer()
    004 MSTORE           ;  storeFreeMemoryPointer()  , m_context << u256(0x40) <<  Instruction::MSTORE;
    -----  initialiseFreeMemoryPointer()  -----
    -----  appandFunctionSelector() Create Code Start ---
    005 PUSH1 04         ;  /--
    007 CALLDATASIZE     ;  |
    008 LT               ;  |  检测CallData 是否合法,CallData 会带有4 字节函数哈希
    009 PUSH1 3f         ;  |
    011 JUMPI            ;  \--
    012 PUSH1 00         ;  /--
    014 CALLDATALOAD     ;  |
    015 PUSH29 0100000000000000000000000000000000000000000000000000000000
    045 SWAP1            ;  |
    046 DIV              ;  |  处理CallData 里面带入的函数哈希.默认读出来数据是在高位,现在处理成低位
    047 PUSH4 ffffffff   ;  |
    052 AND              ;  \--
    053 DUP1             ;  /--
    054 PUSH4 0dbe671f   ;  |
    059 EQ               ;  |
    060 PUSH1 44         ;  |  根据函数哈希找入口
    062 JUMPI            ;  |
    063 JUMPDEST         ;  \--
    064 PUSH1 00         ;  /--
    066 DUP1             ;  |  notFound 代码填充
    067 REVERT           ;  \--
    -----  appandFunctionSelector() 会给每个函数根据参数调用来分配函数头入口初始化检测代码 ---
    068 JUMPDEST         ;  Address 0x44 , function a() Entry ..
    069 CALLVALUE        ;  /--
    070 DUP1             ;  |
    071 ISZERO           ;  |
    072 PUSH1 4f         ;  |
    074 JUMPI            ;  |  solc  /libsolidity/Codegen/ContractCompiler.cpp:appendCallValueCheck()
    075 PUSH1 00         ;  |
    077 DUP1             ;  |
    078 REVERT           ;  \--
    -----  Tag function_A_pre_JumpTo_function_A_Tag_Code  ----
    -----  m_context.appendJumpTo(m_context.functionEntryLabel(functionType->declaration()));
    079 JUMPDEST         ;  JUMPDEST 是无意义的代码,它的唯一意义是用来标识这是一个Label 起始头
    080 POP
    081 PUSH1 56         ;  eth::AssemblyItem returnTag = m_context.pushNewTag();  0x56 is Return Address
    083 PUSH1 58         ;  /
    085 JUMP             ;  \  m_context.appendJumpTo(m_context.functionEntryLabel(functionType->declaration()));
    -----  Tag function_A_return_Code  ----
    086 JUMPDEST         ;  Address 0x56
    087 STOP             ;  ContractCompiler::appendReturnValuePacker();
    -----  Tag function_A_Main_Code  ----
    -----  CompilerContext 是保存每一个函数编译好的代码
    088 JUMPDEST         ;  Address 0x58 , m_context.functionEntryLabel("a"); ..
    089 PUSH1 00
    091 PUSH1 7b
    093 SWAP1            ;  0x7B ,0x00  == SWAP => 0x00 ,0x7B .意义是对栈进行平衡
    094 POP
    095 POP
    096 JUMP              ;  Jump 0x56
    097 STOP              ;
    -----  appandFunctionSelector() Create Code End ---
    098 LOG1
    099 PUSH6 627a7a723058
    106 SHA3
    107 MUL
    108 PUSH15 5fd8c2f6fe4103dba9baf9c48c052e
    124 CALLDATALOAD
    125 INVALID
    126 PUSH1 d9
    128 INVALID
    129 INVALID
    130 TIMESTAMP
    131 STATICCALL
    132 INVALID
    133 INVALID
    134 DUP13
    135 INVALID
    136 TIMESTAMP
    137 INVALID
    138 NUMBER
    139 STOP
    140 INVALID

```

  结合编译器的编译过程和编译出来的结果来阅读理解代码之后,可以知道合约汇编代码的结构:
  
```txt

    1.初始化EVM memory
    2.检测TXDATA 里面是否带有合法的函数哈希
    3.函数跳转
    4.函数预校验代码
    5.函数参数获取代码
    6.函数返回代码
    7.函数主体代码

```  
  
  读者可能会有一个疑问,为什么在汇编代码`097 STOP` 的后面还有多余的代码呢,这些代码的意义何在.我们来阅读`CompilerStack::compileContract()` 的代码:
  
```cpp

    void CompilerStack::compileContract(
        ContractDefinition const& _contract,
        map<ContractDefinition const*, eth::Assembly const*>& _compiledContracts
    )
    {
        
        // ...
        
        shared_ptr<Compiler> compiler = make_shared<Compiler>(m_evmVersion, m_optimize, m_optimizeRuns);
        compiledContract.compiler = compiler;
        
        string metadata = createMetadata(compiledContract);  //  创建元数据
        compiledContract.metadata = metadata;
        
        bytes cborEncodedMetadata = createCBORMetadata(  //  生成CBOR 元数据
            metadata,
            !onlySafeExperimentalFeaturesActivated(_contract.sourceUnit().annotation().experimentalFeatures)
        );
        
        try
        {
            // Run optimiser and compile the contract.
            compiler->compileContract(_contract, _compiledContracts, cborEncodedMetadata);  //  编译合约
        }
        catch(eth::OptimizerException const&)
        {
            solAssert(false, "Optimizer exception during compilation");
        }
        
        // ...
        
    }

```

  可以看到,编译合约的时候`cborEncodedMetadata` 的数据也带入`compileContract()` ,compileContract() 代码如下:

```cpp

    void Compiler::compileContract(
        ContractDefinition const& _contract,
        std::map<const ContractDefinition*, eth::Assembly const*> const& _contracts,
        bytes const& _metadata
    )
    {
        ContractCompiler runtimeCompiler(nullptr, m_runtimeContext, m_optimize);  //  初始化合约编译类
        runtimeCompiler.compileContract(_contract, _contracts);    //  编译合约代码
        m_runtimeContext.appendAuxiliaryData(_metadata);   //  把CBOR 元数据附加在编译之后合约代码末端

        // ...
    }

```

  那么回来阅读`createCBORMetadata()` 的代码,发现它其实是使用了元数据来构造出的数据.

```cpp

    bytes CompilerStack::createCBORMetadata(string _metadata, bool _experimentalMode)
    {
        bytes cborEncodedHash =
            // CBOR-encoding of the key "bzzr0"
            bytes{0x65, 'b', 'z', 'z', 'r', '0'}+
            // CBOR-encoding of the hash
            bytes{0x58, 0x20} + dev::swarmHash(_metadata).asBytes();
        bytes cborEncodedMetadata;
        if (_experimentalMode)
            cborEncodedMetadata =
                // CBOR-encoding of {"bzzr0": dev::swarmHash(metadata), "experimental": true}
                bytes{0xa2} +
                cborEncodedHash +
                bytes{0x6c, 'e', 'x', 'p', 'e', 'r', 'i', 'm', 'e', 'n', 't', 'a', 'l', 0xf5};
        else
            cborEncodedMetadata =
                // CBOR-encoding of {"bzzr0": dev::swarmHash(metadata)}
                bytes{0xa1} +
                cborEncodedHash;
        solAssert(cborEncodedMetadata.size() <= 0xffff, "Metadata too large");
        // 16-bit big endian length
        cborEncodedMetadata += toCompactBigEndian(cborEncodedMetadata.size(), 2);
        return cborEncodedMetadata;
    }

```
  
  我们从Bytecode 中提取出CBOR 编码,数据为`a165627a7a723058207ba6766efb653d5e4d3b7d5893d345b79718b5513bd5a87d5bf8256fa895c58d0029` ,对它的标注如下:

```txt

    a1                ;  Flag : experimental = False
    65627a7a72305820  ;  CBOREncodeHash
    7ba6766efb653d5e4d3b7d5893d345b79718b5513bd5a87d5bf8256fa895c58d  ;  BZZA hash
    0029              ;  hash 长度

```  
  
  弄懂solidity 的编译过程和为什么会编译出这样的结果之后,现在回来解答前面提出的两个问题:

```txt

    1.为什么会有合约代码结束分界线,多出来的代码究竟是什么
    多出来的代码是CBOR 元数据编码
    
    2.为什么会有很多多余的跳转
    多余的跳转是因为appendFunctionSelector() 会帮助函数去构造预处理代码,参数提取代码和返回代码,最后才跳转到函数的主体代码.solidity 和x86 arm 等的汇编不同,它的对函数的参数和返回值处理都不是由函数主体来完成的.
    
    3.JUMPDEST 是无意思的字节码,为什么会多次出现
    编译器在生成编译代码时,可以看到JUMP 和JUMPI 指令会跳转到JUMPDEST 指令.JUMPDEST 指令是solidity 编译器用来标识汇编代码的区段声名(Tag).所以后面的示例汇编代码都会在JUMPDEST 前记录代码区段的意思标注.
    
```

  至此,本文还有一个疑问没有被解决:`ByteCode` 和`Runtime Bytecode` 分别表示什么意思呢?我们来回顾`Compiler::compileContract()` 的完整代码:

```cpp

    void Compiler::compileContract(
        ContractDefinition const& _contract,
        std::map<const ContractDefinition*, eth::Assembly const*> const& _contracts,
        bytes const& _metadata
    )
    {
        ContractCompiler runtimeCompiler(nullptr, m_runtimeContext, m_optimize);  //  编译runtime 代码
        runtimeCompiler.compileContract(_contract, _contracts);
        m_runtimeContext.appendAuxiliaryData(_metadata);  //  插入CBOR 编码

        // This might modify m_runtimeContext because it can access runtime functions at
        // creation time.
        ContractCompiler creationCompiler(&runtimeCompiler, m_context, m_optimize);  //  编译creation 代码
        m_runtimeSub = creationCompiler.compileConstructor(_contract, _contracts);

        m_context.optimise(m_optimize, m_optimizeRuns);  //  优化汇编代码
    }

```

  可以看到`Compiler::compileContract()` 里面分两部分来编译合约代码:runtime 的代码指的是合约编写逻辑的代码;creation 的代码指的是constructor() 的代码.我们回来看ByteCode 和RuntimeCode 的汇编代码来做对比

ByteCode 汇编:

```txt

    ----  Binary  ----
    6080604052348015600f57600080fd5b50608d8061001e6000396000f300608060405260043610603f576000357c0100000000000000000000000000000000000000000000000000000000900463ffffffff1680630dbe671f146044575b600080fd5b348015604f57600080fd5b5060566058565b005b6000607b9050505600a165627a7a72305820026e5fd8c2f6fe4103dba9baf9c48c052e35ca60d9cdee42faca258c284224430029
    
    ----  Constructor() Code  ----
    PUSH1 0x80 
    PUSH1 0x40 
    MSTORE 
    CALLVALUE 
    DUP1 
    ISZERO 
    PUSH1 0xF 
    JUMPI 
    PUSH1 0x0 
    DUP1 
    REVERT 
    JUMPDEST 
    POP 
    PUSH1 0x8D 
    DUP1 
    PUSH2 0x1E 
    PUSH1 0x0 
    CODECOPY 
    PUSH1 0x0 
    RETURN 
    STOP 
    ----  Contract Code  ----
    PUSH1 0x80 
    PUSH1 0x40 
    MSTORE 
    PUSH1 0x4 
    CALLDATASIZE 
    LT 
    PUSH1 0x3F 
    JUMPI 
    PUSH1 0x0 
    CALLDATALOAD 
    PUSH29 0x100000000000000000000000000000000000000000000000000000000 
    SWAP1 
    DIV 
    PUSH4 0xFFFFFFFF 
    AND 
    DUP1 
    PUSH4 0xDBE671F 
    EQ 
    PUSH1 0x44 
    JUMPI 
    JUMPDEST 
    PUSH1 0x0 
    DUP1 
    REVERT 
    JUMPDEST 
    CALLVALUE 
    DUP1 
    ISZERO 
    PUSH1 0x4F 
    JUMPI 
    PUSH1 0x0 
    DUP1 
    REVERT 
    JUMPDEST 
    POP 
    PUSH1 0x56 
    PUSH1 0x58 
    JUMP 
    JUMPDEST 
    STOP 
    JUMPDEST 
    PUSH1 0x0 
    PUSH1 0x7B 
    SWAP1 
    POP 
    POP 
    JUMP 
    STOP 

```

RuntimeCode 汇编:

```txt

    ----  Binary  ----
    608060405260043610603f576000357c0100000000000000000000000000000000000000000000000000000000900463ffffffff1680630dbe671f146044575b600080fd5b348015604f57600080fd5b5060566058565b005b6000607b9050505600a165627a7a72305820026e5fd8c2f6fe4103dba9baf9c48c052e35ca60d9cdee42faca258c284224430029
    
    ----  Contract Code  ----
    PUSH1 0x80 
    PUSH1 0x40 
    MSTORE 
    PUSH1 0x4 
    CALLDATASIZE 
    LT 
    PUSH1 0x3F 
    JUMPI 
    PUSH1 0x0 
    CALLDATALOAD 
    PUSH29 0x100000000000000000000000000000000000000000000000000000000 
    SWAP1 
    DIV 
    PUSH4 0xFFFFFFFF 
    AND 
    DUP1 
    PUSH4 0xDBE671F 
    EQ 
    PUSH1 0x44 
    JUMPI 
    JUMPDEST 
    PUSH1 0x0 
    DUP1 
    REVERT 
    JUMPDEST 
    CALLVALUE 
    DUP1 
    ISZERO 
    PUSH1 0x4F 
    JUMPI 
    PUSH1 0x0 
    DUP1 
    REVERT 
    JUMPDEST 
    POP 
    PUSH1 0x56 
    PUSH1 0x58 
    JUMP 
    JUMPDEST 
    STOP 
    JUMPDEST 
    PUSH1 0x0 
    PUSH1 0x7B 
    SWAP1 
    POP 
    POP 
    JUMP 
    STOP

```

  所以,我们可以明白在部署合约的时候,EVM 执行`Constructor()` 函数的代码初始化合约数据,后续用户通过Web3 调用节点上的合约函数时,是直接在RuntimeCode 中开始执行合约代码.TXDATA 中是带有用户希望调用函数的哈希和函数的参数数据的,接下来合约代码初始EVM Memory 之后,根据TXDATA 里面指向用户希望调用的函数哈希来进行代码跳转.执行函数主体代码前做一些预检测并从TXDATA 中提取函数参数到栈,最后执行函数主体代码并退出.

