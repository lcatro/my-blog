
[MAKE_FUNCTION]
v = POP(); /* code object */
x = PyFunction_New(v, f->f_globals);
Py_DECREF(v);
/* XXX Maybe this should be a separate opcode? */
if (x != NULL && oparg > 0) {
    v = PyTuple_New(oparg);
    if (v == NULL) {
        Py_DECREF(x);
        x = NULL;
        break;
    }
    while (--oparg >= 0) {
        w = POP();  <==  这里可以直接构造假对象
        PyTuple_SET_ITEM(v, oparg, w);
    }
    err = PyFunction_SetDefaults(x, v);
    Py_DECREF(v);
}
PUSH(x);
















