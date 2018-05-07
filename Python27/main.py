from sympy import *

def foo(f, b):
    t = Symbol('t')
    return integrate(f, (t, 0, b))
