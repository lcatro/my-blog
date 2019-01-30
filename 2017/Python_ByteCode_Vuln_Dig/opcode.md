
[Python Stack 结构]
typedef struct _frame {
    PyObject_VAR_HEAD
    struct _frame *f_back;	/* previous frame, or NULL */
    PyCodeObject *f_code;	/* code segment */  Python Opcode 代码..
    PyObject *f_builtins;	/* builtin symbol table (PyDictObject) */
    PyObject *f_globals;	/* global symbol table (PyDictObject) */  Python 全局变量
    PyObject *f_locals;		/* local symbol table (any mapping) */
    PyObject **f_valuestack;	/* points after the last local */
    /* Next free slot in f_valuestack.  Frame creation sets to f_valuestack.
       Frame evaluation usually NULLs it, but a frame that yields sets it
       to the current stack top. */
    PyObject **f_stacktop;  Python 本地栈
    PyObject *f_trace;		/* Trace function */

    /* If an exception is raised in this frame, the next three are used to
     * record the exception info (if any) originally in the thread state.  See
     * comments before set_exc_info() -- it's not obvious.
     * Invariant:  if _type is NULL, then so are _value and _traceback.
     * Desired invariant:  all three are NULL, or all three are non-NULL.  That
     * one isn't currently true, but "should be".
     */
    PyObject *f_exc_type, *f_exc_value, *f_exc_traceback;

    PyThreadState *f_tstate;
    int f_lasti;		/* Last instruction if called */
    /* Call PyFrame_GetLineNumber() instead of reading this field
       directly.  As of 2.3 f_lineno is only valid when tracing is
       active (i.e. when f_trace is set).  At other times we use
       PyCode_Addr2Line to calculate the line from the current
       bytecode index. */
    int f_lineno;		/* Current line number */
    int f_iblock;		/* index in f_blockstack */
    PyTryBlock f_blockstack[CO_MAXBLOCKS]; /* for try and loop blocks */
    PyObject *f_localsplus[1];	/* locals+stack, dynamically sized */
} PyFrameObject;

GETLOCAL() 和SETLOCAL() 是在对像数组上(PyFrameObject.f_localsplus )获取和设置一个本地变量
  GETLOCAL(variant_index) 代码:  ==>  OOB Read
    PyFrameObject.f_localsplus[variant_index]
  SETLOCAL(variant_index,variant_value) 代码:  ==>  OOB Write
    PyObject *tmp=GETLOCAL(variant_index);
    GETLOCAL(variant_index)=variant_value;
    Py_XDECREF(tmp);
    
stack_pointer 是Python 栈指针,来源于PyFrameObject.f_stacktop ,一些指令会使用到PUSH() ,和POP() 来实现数据的操作,这些数据都会保存到stack_pointer 中,和stack_pointer 的操作有:
  PUSH(variant_value) => BASIC_PUSH(variant_value) => *stack_pointer++ = (variant_value)
  POP() => BASIC_POP() => *--stack_pointer
  STACKADJ(adjust_number) => BASIC_STACKADJ(adjust_number) => stack_pointer += adjust_number  ==>  OOB Read/Write
  TOP() => stack_pointer[-1]
  SECOND() => stack_pointer[-2]
  THIRD() => stack_pointer[-3]
  FOURTH() => stack_pointer[-4]
  PEEK(index) => stack_pointer[-(index)]  ==>  OOB Read
  SET_TOP(value) => stack_pointer[-1] = (value)
  SET_SECOND(value) => stack_pointer[-2] = (value)
  SET_THIRD(value) => stack_pointer[-3] = (value)
  SET_FOURTH(value) => stack_pointer[-4] = (value)
  SET_VALUE(index,value) => stack_pointer[-(index)] = (value)  ==>  OOB Write

Py_INCREF(op) => ((PyObject*)(op))->ob_refcnt++)

Py_DECREF(op) 
 if (--((PyObject*)(op))->ob_refcnt != 0)
  _Py_CHECK_REFCNT(op)
 else
  _Py_Dealloc((PyObject *)(op));

oparg 是int 类型,可以通过EXTENDED_ARG 填充高16 位变负数..


[NOP]
直接调用fast_dispatch()

[POP_TOP]
value=POP()

[ROT_TWO,ROT_THREE,ROT_FOUR]
把stack_pointer 几个数据交换

[DUP_TOP]
value=TOP() ,PUSH(value)

[DUP_TOPX]
和ROT_THREE,ROT_FOUR 原理一样

[LOAD_FAST]
value=GETLOCAL(oparg) ,PUSH(value)

[STORE_FAST]
value=POP() ,SETLOCAL(oparg,value)

[LOAD_CONST]
从co_consts 中获取对象,然后PUSH() 到stack_pointer

WARNING ! 可以通过LOAD_CONST 加载一个对象到内存..

[LOAD_NAME,LOAD_GLOBAL]
从co_names 中获取对象,然后PUSH() 到stack_pointer

[LIST_APPEND]
value1=POP() ,value2=stack_pointer[-oparg] ,PyList_Append(value2,value1)
typeof value2==PyList

[SET_ADD]
value1=POP() ,value2=stack_pointer[-oparg] 
typeof value1==PySet ,typeof value1==PyFrozenSet
typeof value2==PyString
call value1.lookup(value1,value2,hash) => 这是个C 函数调用..

[RETURN_VALUE]
retval=POP() ,goto fast_block_end

[YIELD_VALUE]
retval=POP() ,f->f_stacktop=stack_pointer ,goto fast_yield

[POP_BLOCK]
PyFrame_BlockPop(PyFrameObject) ,value=POP()

[LOAD_CLOSURE]
value=freevars[oparg] ,PUSH(value)

[LOAD_ATTR]
value1=GETITEM(names,oparg) ,value2=TOP() ,value3=PyObject_GetAttr(value2,value1) ,SET_TOP(value3)

[FOR_ITER]
value1=TOP() ,value2=(*value1->ob_type->tp_iteranext)(value1) 

[CALL_FUNCTION]
value=Call_function() ,PUSH(value)

[CALL_FUNCTION_VAR_KW]


[EXTENDED_ARG]
默认的指令只能保存2 字节的数据,EXTENDED_ARG 可以使用4 字节
opcode=NEXTOP() ,oparg=oparg << 16 | NEXTARG()
比如函数有很多的情况下,应该这样调用
EXTENDED_ARG 0x12
CALL_FUNCTION 0x3456
等价于
CALL_FUNCTION 0x123456




