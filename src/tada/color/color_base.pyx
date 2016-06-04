import math, numbers

cdef extern from "<tada/color/names_inl.h>" namespace "tada":
    cdef cppclass Base:
        pass

cdef extern from "<tada/color/names_inl.h>" namespace "tada::Base":
    cdef Base normal
    cdef Base integer

cdef extern from "<tada/color/names_inl.h>" namespace "tada":
    cdef cppclass ColorS:
        float red, green, blue

        ColorS()
        ColorS(float, float, float)

    # These next operations might make sense with any time data.
    void minInto(ColorS&, ColorS&);
    void maxInto(ColorS&, ColorS&);

    float invert(float, float)
    float normalize(float, float)
    ColorS rotate(ColorS, int)
    bool cmpToRichcmp(float cmp, int richcmp)
    float distance2(ColorS&, ColorS&)
    float distance(ColorS&, ColorS&)
    float cmp(ColorS&, ColorS&)

    # Color-only operations
    bool stringToColor(const char*, ColorS&, Base)
    string colorToString(ColorS&, Base)
    vector[string] colorNames()
    ColorS colorFromHex(uint32_t hex, Base base);
    uint32_t hexFromColor(ColorS&, Base base);


cdef extern from "<tada/color/hsv.h>" namespace "tada":
    ColorS hsvToRgb(ColorS, Base)
    ColorS rgbToHsv(ColorS, Base)
