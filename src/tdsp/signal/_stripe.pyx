# Automatically generated on 2016-05-11T15:46:21.243293
# by https://github.com/rec/make_pyx/make_pyx.py

cdef extern from "<tdsp/signal/stripe.h>" namespace "tdsp":
    struct Stripe:
        int begin, skip
        size_t repeats
        bool reflect


cdef class _Stripe(_Wrapper):
    cdef Stripe _instance;

    def __cinit__(self):
        clearStruct(self._instance)

    def clear(self):
        clearStruct(self._instance)

    def __str__(self):
        return "(begin=%s, skip=%s, repeats=%s, reflect=%s)" % (
            self.begin, self.skip, self.repeats, self.reflect)

    property begin:
        def __get__(self):
            return self._instance.begin
        def __set__(self, int x):
            self._instance.begin = x

    property skip:
        def __get__(self):
            return self._instance.skip
        def __set__(self, int x):
            self._instance.skip = x

    property repeats:
        def __get__(self):
            return self._instance.repeats
        def __set__(self, size_t x):
            self._instance.repeats = x

    property reflect:
        def __get__(self):
            return self._instance.reflect
        def __set__(self, bool x):
            self._instance.reflect = x
